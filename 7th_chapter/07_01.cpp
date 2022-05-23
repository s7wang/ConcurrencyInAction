#include <iostream>
#include <atomic>
#include <thread>
#include <unistd.h>

template<typename T>
class lock_free_stack
{
private:
    struct node 
    {
        T data;
        node* next;
        node(T const& data_):data(data_)
        {

        }
    };
    std::atomic<node*> head;
public:
    lock_free_stack()
    {

    }
    lock_free_stack(lock_free_stack &&) = default;
    lock_free_stack(const lock_free_stack &) = default;
    lock_free_stack &operator=(lock_free_stack &&) = default;
    lock_free_stack &operator=(const lock_free_stack &) = default;
    ~lock_free_stack()
    {

    }

    void push(T const& data)
    {
        node* const new_node = new node(data);
        new_node->next = head.load();
    
        while (!head.compare_exchange_weak(new_node->next, new_node));
    }

    void print() {
        for (auto p = head.load(); p != nullptr; p = p->next) {
            std::cout << p->data << std::endl;
        }
    }

};


int main(int argc, char const *argv[])
{
    /* code */

    lock_free_stack<int> stack;

    std::thread t1([&]{
        
        for(int i = 0; i < 2000; i += 2) 
        {
            stack.push(i);
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    });
    std::thread t2([&]{
        for(int i = 1; i < 2000; i += 2) 
        {
            stack.push(i);
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    });
    t1.join();
    t2.join();
    sleep(2);
    stack.print();
    return 0;
}
