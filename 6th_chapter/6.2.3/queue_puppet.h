// 使用傀儡节点的简单队列
#ifndef _QUEUE_PUPPET_H_
#define _QUEUE_PUPPET_H_


#include <algorithm>
#include <memory>

template<typename T>
class queue_puppet
{
private:
    struct node
    {
        std::shared_ptr<T> data;
        std::unique_ptr<node> next;
    };

    std::unique_ptr<node> head;
    node* tail;

public:
    queue_puppet():head(new node()), tail(head.get())
    { }
    queue_puppet(queue_puppet &&) = default;
    queue_puppet(const queue_puppet &) = delete;
    queue_puppet &operator=(queue_puppet &&) = default;
    queue_puppet &operator=(const queue_puppet &) = delete;
    virtual ~queue_puppet()
    { }

    std::shared_ptr<T> try_pop()
    {
        if (head.get() == tail)
        {
            return std::shared_ptr<T> ();
        }
        std::shared_ptr<T> const res(head->data);
        std::unique_ptr<node> old_head = std::move(head);
        head = std::move(old_head->next);
        return res;
    }

    void push(T& new_value)
    {
        std::shared_ptr<T> new_data(std::make_shared<T>(std::move(new_value)));
        std::unique_ptr<node> p(new node());
        tail->data = new_data;
        node* const new_tail = p.get();
        tail->next = std::move(p);
        tail = new_tail;
    }
};

#endif // !_QUEUE_PUPPET_H_