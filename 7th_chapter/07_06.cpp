#include <iostream>
#include <memory>
#include <atomic>
#include <thread>
template<typename T>
class lock_free_stack
{

private:
    struct node;
    struct counted_node_ptr 
    {
        int external_count;
        node *ptr;
    };
    struct node 
    {
        std::shared_ptr<T> data;
        std::atomic<int> internal_count;
        counted_node_ptr next;

        node(T const& data_)
        {
            data = std::make_shared<T>(data_);
            internal_count = 0;
        }
    };
    std::atomic<counted_node_ptr> head;
public:
    lock_free_stack() 
    {
        // head.external_count = 0;
        // head.ptr = nullptr;
    }
    lock_free_stack(lock_free_stack &&) = default;
    lock_free_stack(const lock_free_stack &) = default;
    lock_free_stack &operator=(lock_free_stack &&) = default;
    lock_free_stack &operator=(const lock_free_stack &) = default;
    ~lock_free_stack() 
    {
        while (pop());
    }
    void push(T const& value)
    {
        counted_node_ptr new_node;
        new_node.ptr = new node(value);
        new_node.external_count = 1;
        new_node.ptr->next = head.load();
        while (!head.compare_exchange_weak(new_node.ptr->next, new_node));
    }

    std::shared_ptr<T> pop() 
    {
        counted_node_ptr old_head = head.load();
        for (;;)
        {
            increase_head_count(old_head);
            node* const ptr = old_head.ptr;
            if(!ptr)
            {
                return std::shared_ptr<T> ();
            }
            if (!head.compare_exchange_strong(old_head, ptr->next))
            {
                std::shared_ptr<T> res;
                res.swap(ptr->data);
                int const count_increase = old_head.external_count-2;
                if(ptr->internal_count.fetch_add(count_increase) == -count_increase)
                {
                    delete ptr;
                }
                return res;
            }
            else if (ptr->internal_count.fetch_sub(1) == 1)
            {
                delete ptr;
            }
        }
        
    }

private:
    void increase_head_count(counted_node_ptr& old_counter)
    {
        counted_node_ptr new_counter;
        do 
        {
            new_counter = old_counter;
            (new_counter.external_count)++;
        }
        while (!head.compare_exchange_strong(old_counter, new_counter));
        old_counter.external_count = new_counter.external_count;
    }
    
};


int main(int argc, char const *argv[])
{

    lock_free_stack<int> test;

    for (int i = 0; i < 20; i++)
    {
        test.push(i);
    }
    for (auto k = test.pop(); k != std::shared_ptr<int>(); k = test.pop())
    {
        std::cout << *k << std::endl;
    }
    
    
    return 0;
}

