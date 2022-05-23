#ifndef _QUEUE_P_H_
#define _QUEUE_p_H_

#include <queue>
#include <mutex>
#include <condition_variable>


template <typename T>
class ts_queue_p
{
private:
    mutable std::mutex mut;
    std::queue<std::shared_ptr<T> > data_queue;
    std::condition_variable data_cond;
public:
    ts_queue_p() {}
    ts_queue_p(ts_queue_p &) = delete;
    ts_queue_p(ts_queue_p &&) = delete;
    ts_queue_p &operator=(ts_queue_p &) = delete;
    ts_queue_p &operator=(ts_queue_p &&) = delete;
    
    void wait_and_pop(T& value)
    {   
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [this](){return !data_queue.empty();});
        value = std::move(*data_queue.front());
        data_queue.pop();
    }
    
    int try_pop(T& value)
    {
        std::lock_guard<std::mutex> lk(mut);
        if (data_queue.empty())
            return -1;
        value = std::move(*data_queue.front());
        data_queue.pop();
        return 0;
    }

    std::shared_ptr<T> wait_and_pop()
    {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [this](){return !data_queue.empty();});
        std::shared_ptr<T> res = data_queue.front();
        data_queue.pop();
        return res;
    }

    std::shared_ptr<T> try_pop()
    {
        std::lock_guard<std::mutex> lk(mut);
        if (data_queue.empty())
            return std::shared_ptr<T>();
        
        std::shared_ptr<T> res = data_queue.front();
        data_queue.pop();
        return res;
    }

    void push(T new_value)
    {
        std::shared_ptr<T> data(std::make_shared<T>(std::move(new_value)));
        std::lock_guard<std::mutex> lk(mut);
        data_queue.push(data);
        data_cond.notify_one();
    }

    bool empty()
    {
        std::lock_guard<std::mutex> lk(mut);
        return data_queue.empty();
    }

    virtual ~ts_queue_p() {}
};

#endif //_QUEUE_P_H_