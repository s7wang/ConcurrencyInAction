#ifndef _TS_QUEUE_PUPPET_H_
#define _TS_QUEUE_PUPPET_H_


#include <algorithm>
#include <memory>
#include <mutex>


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
    std::condition_variable data_cond;

private:
    node* get_tail()
    {
        std::lock_guard<std::mutex> tail_lock(tail_mutex);
        return tail;
    }

    std::unique_ptr<node> pop_head()
    {
        std::unique_ptr<node> old_head = std::move(head);
        head = std::move(old_head->next);
        return old_head;
    }

    std::unique_lock<std::mutex> wait_for_data()
    {
        std::unique_lock<std::mutex> head_lock(head_mutex);
        data_cond.wait(head_lock, [&] { return head.get() != get_tail(); });
        return std::move(head_lock);
    }

    std::unique_ptr<node> try_pop_head()
    {
        std::lock_guard<std::mutex> head_lock(head_mutex);
        if (head.get() == get_tail())
        {
            return nullptr;
        }
        return pop_head();
    }

    std::unique_ptr<node> wait_pop_head()
    {
        std::unique_lock<std::mutex> head_lock(wait_for_data());
        return pop_head();
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

    void push(T new_value)
    {
        std::shared_ptr<T> new_data(std::make_shared<T>(std::move(new_value)));
        std::unique_ptr<node> p(new node());
        
        {
            std::lock_guard<std::mutex> tail_lock(tail_mutex);
            node* const new_tail = p.get();
            tail->data = new_data;
            tail->next = std::move(p);
            tail = new_tail;
        }
        data_cond.notify_one();
    }
    std::shared_ptr<T> try_pop()
    {
        std::unique_ptr<node> old_head = try_pop_head();
        return old_head ? old_head->data : std::shared_ptr<T> ();
    }

    std::shared_ptr<T> wait_and_pop()
    {
        std::shared_ptr<node> const old_head = wait_pop_head();
        return old_head->data;
    }

    bool empty()
    {
        std::lock_guard<std::mutex> head_lock(head_mutex);
        return (head.get() == get_tail());
    }

};

#endif // _TS_QUEUE_PUPPET_H_