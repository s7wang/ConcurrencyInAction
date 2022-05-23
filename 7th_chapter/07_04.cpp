#include <iostream>
#include <atomic>
#include <thread>
#include <memory>
#include <chrono>
#include <unistd.h>

template<typename T>
class lock_free_stack
{
private:
    struct node 
    {
        std::shared_ptr<T> data;
        node* next;
        node(T const& data_)
        {
            data = std::make_shared<T>(data_);
        }
    };
    std::atomic<node*> head;

public:
    lock_free_stack() {}
    lock_free_stack(lock_free_stack &&) = default;
    lock_free_stack(const lock_free_stack &) = default;
    lock_free_stack &operator=(lock_free_stack &&) = default;
    lock_free_stack &operator=(const lock_free_stack &) = default;
    ~lock_free_stack() {}

    void push(T const& data)
    {
        node* const new_node = new node(data);
        new_node->next = head.load();
    
        while (!head.compare_exchange_weak(new_node->next, new_node));
    }

    std::shared_ptr<T> pop() {
        ++thread_in_pop;
        node* old_head = head.load();
        while (old_head && !head.compare_exchange_weak(old_head, old_head->next));
        std::shared_ptr<T> res;
        if (old_head)
        {
            res.swap(old_head->data);
        }
        try_reclaim(old_head);
        return res;
    }

    void print() {
        for (auto p = head.load(); p != nullptr; p = p->next) {
            std::cout << *(p->data) << std::endl;
        }
    }

private:
    std::atomic<unsigned> thread_in_pop;
    std::atomic<node*> to_be_deleted;

    static void delete_nodes(node* nodes) {
        while (nodes)
        {
            node* next = nodes->next;
            delete nodes;
            nodes = next;
        }
    }

    void chain_pending_nodes(node* nodes) {
        node* last = nodes;
        while (node* const next = last->next) 
        {
            last = next;
        }
        chain_pending_nodes(nodes, last);
    }

    void chain_pending_nodes(node* first, node* last) {
        last->next = to_be_deleted;
        while (!to_be_deleted.compare_exchange_weak(last->next, first));
    }

    void chain_pending_node(node* n) {
        chain_pending_nodes(n, n);
    }

    void try_reclaim(node* old_head) {
        if (thread_in_pop == 1) 
        {
            node* nodes_to_delete = to_be_deleted.exchange(nullptr);
            if (!--thread_in_pop)
            {
                delete_nodes(nodes_to_delete);
            }
            else if (nodes_to_delete) 
            {
                chain_pending_nodes(nodes_to_delete);
            }
            delete old_head;
        }
        else
        {
            chain_pending_node(old_head);
            --thread_in_pop;
        }
    }

};


int main(int argc, char const *argv[])
{
    /* code */

    lock_free_stack<int> stack;

    for (int i = 0; i < 20000; i++) 
    {
        stack.push(i);
    }

    std::thread t1([&]{
        int n;
        for(int i = 0; i < 20000; i += 2) 
        {   
            std::cout << *(stack.pop()) << std::endl;
            //std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    });
    std::thread t2([&]{
        int n;
        for(int i = 0; i < 20000; i += 2) 
        {   
            std::cout << *(stack.pop()) << std::endl;
            //std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    });
    t1.join();
    t2.join();

    return 0;
}
