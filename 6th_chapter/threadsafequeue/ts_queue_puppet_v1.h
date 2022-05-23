#ifndef _TS_QUEUE_PUPPET_H_
#define _TS_QUEUE_PUPPET_H_


#include <algorithm>
#include <memory>
#include <mutex>
#include <stdio.h>


template<typename T>
class ThreadSafeQueue
{
private:
    struct node
    {
        std::shared_ptr<T> data;
        std::unique_ptr<node> next;
    };

    std::mutex head_mutex;
    std::unique_ptr<node> head;
    std::mutex tail_mutex;
    node* tail;

    node* get_tail()
    {
        std::lock_guard<std::mutex> tail_lock(tail_mutex);
        return tail;
    }

    std::unique_ptr<node> pop_head()
    {
        std::lock_guard<std::mutex> head_lock(head_mutex);
        if (head.get() == get_tail())
        {
            return nullptr;
        }
        std::unique_ptr<node> old_head = std::move(head);
        head = std::move(old_head->next);
        return old_head;
    }
public:
    ThreadSafeQueue():head(new node()), tail(head.get())
    { }
    ThreadSafeQueue(ThreadSafeQueue &&) = default;
    ThreadSafeQueue(const ThreadSafeQueue &) = delete;
    ThreadSafeQueue &operator=(ThreadSafeQueue &&) = default;
    ThreadSafeQueue &operator=(const ThreadSafeQueue &) = delete;
    virtual ~ThreadSafeQueue()
    { }

    std::shared_ptr<T> try_pop()
    {
        std::unique_ptr<node> old_head = pop_head();
        return old_head ? old_head->data : std::shared_ptr<T> ();
    }

    void push(T new_value)
    {
        std::shared_ptr<T> new_data(std::make_shared<T>(std::move(new_value)));
        std::unique_ptr<node> p(new node());
        node* const new_tail = p.get();

        std::lock_guard<std::mutex> tail_lock(tail_mutex);
        tail->data = new_data;
        tail->next = std::move(p);
        tail = new_tail;
    }
};

#endif // _TS_QUEUE_PUPPET_H_