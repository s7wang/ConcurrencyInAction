#ifndef _QUEUE_S_H_
#define _QUEUE_S_H_

#include <queue>
#include <mutex>
#include <condition_variable>

template<typename T>
class threadsafe_queue
{
private:
    mutable std::mutex queue_mutex;
    std::condition_variable data_cond;
    std::queue<T> data_queue;

public:
    threadsafe_queue() { }
    threadsafe_queue(const threadsafe_queue &other) 
    {
        std::lock_guard<std::mutex> lk(other.queue_mutex);
        this->data_queue = other.data_queue;
    }
    
    threadsafe_queue(threadsafe_queue &&) = delete;  
    threadsafe_queue &operator=(threadsafe_queue &&) = delete;
    threadsafe_queue &operator=(const threadsafe_queue &) = delete;
    ~threadsafe_queue() { }

public:
    void push(T new_value)
    {
        std::lock_guard<std::mutex> lk(queue_mutex);
        data_queue.push(new_value);
        data_cond.notify_one();
    }
    
    bool try_pop(T& value)
    {
        std::lock_guard<std::mutex> lk(queue_mutex);
        if (data_queue.empty())
            return false;
        value = data_queue.front();
        data_queue.pop();
        return true;
    }
    std::shared_ptr<T> try_pop()
    {
        std::lock_guard<std::mutex> lk(queue_mutex);
        if (data_queue.empty())
            return std::shared_ptr<T> ();
        std::shared_ptr<T> ret( std::make_shared<T>(data_queue.front()) );
        data_queue.pop();
        return ret;
    }

    void wait_and_pop(T& value)
    {
        std::unique_lock<std::mutex> lk(queue_mutex);
        data_cond.wait(lk, [&]{ return !data_queue.empty(); });
        value = data_queue.front();
        data_queue.pop();
    }
    std::shared_ptr<T> wait_and_pop()
    {
        std::unique_lock<std::mutex> lk(queue_mutex);
        data_cond.wait(lk, [&]{ return !data_queue.empty(); });
        std::shared_ptr<T> ret( std::make_shared<T>(data_queue.front()) );
        data_queue.pop();
        return ret;
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lk(queue_mutex);
        return data_queue.empty();
    }

};

#endif // !_QUEUE_S_H_

