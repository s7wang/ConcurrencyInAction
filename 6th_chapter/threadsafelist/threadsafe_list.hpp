#ifndef _THREADSAFE_LIST_H_
#define _THREADSAFE_LIST_H_

#include <memory>
#include <mutex>
#include <utility>

template <typename T>
class threadsafe_list
{

private:
    struct node
    {
        std::mutex m;
        std::shared_ptr<T> data;
        std::unique_ptr<node> next;

        node() : next(nullptr) { }
        node(T const& value) : data(std::make_shared<T>(value)){ } 
        node(node && old_node): data(std::move(old_node.data)), next(std::move(old_node.next)) { }
    };

    node head;
public:
    threadsafe_list()
    {

    }
    threadsafe_list(threadsafe_list &&) = default;
    threadsafe_list(const threadsafe_list &) = delete;
    threadsafe_list &operator=(threadsafe_list &&) = default;
    threadsafe_list &operator=(const threadsafe_list &) = delete;
    ~threadsafe_list()
    {
        remove_if([](node const&){return true;});
    }

    void push_front(T const& value) 
    {
        std::unique_ptr<node> new_node(new node(value));
        std::lock_guard<std::mutex> lock(head.m);
        new_node->next = std::move(head.next);
        head.next = std::move(new_node);
    }

    template<typename Function>
    void for_each(Function f)
    {
        node *current = &head;
        std::unique_lock<std::mutex> lock(head.m);
        while (node* const next = current->next.get())
        {
            std::unique_lock<std::mutex> next_lock(next->m);
            lock.unlock();
            f(*next->data);
            current = next;
            lock = std::move(next_lock);
        }
    }

    template<typename Predicate>
    std::shared_ptr<T> find_first_if(Predicate p)
    {
        node *current = &head;
        std::unique_lock<std::mutex> lock(head.m);
        while (node* const next = current->next.get())
        {
            std::unique_lock<std::mutex> next_lock(next->m);
            lock.unlock();
            if(p(*next->data))
            {
                return next->data;
            }           
            lock = std::move(next_lock);
        }
        return std::shared_ptr<T>();
    }

    template<typename Predicate>
    void remove_if(Predicate p)
    {
        node *current = &head;
        std::unique_lock<std::mutex> lock(head.m);
        while (node* const next = current->next.get())
        {
            std::unique_lock<std::mutex> next_lock(next->m);
            lock.unlock();
            if (p(*next->data))
            {
                std::unique_ptr<node> old_next = std::move(next->next);
                next_lock.unlock();
            }
            else
            {
                lock.unlock();
                current = next;
                lock = std::move(next_lock);
            }

        }        
    }
    
};




#endif // !_THREADSAFE_LIST_H_