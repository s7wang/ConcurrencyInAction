#include <iostream>
#include <atomic>
#include <thread>
#include <memory>
#include <chrono>
#include <unistd.h>
#include <functional>

#define MAX_HAZARD_POINTERS     100

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

        std::atomic<void*>& hp = get_hazard_pointer_for_current_thread();
        node* old_head = head.load();
        do 
        {
            node* temp;
            do
            {
                temp = old_head;
                hp.store(old_head);
                old_head = head.load();
            } while (temp != old_head);
            
        } 
        while (old_head && !head.compare_exchange_strong(old_head, old_head->next));

        hp.store(nullptr);
        std::shared_ptr<T> res;
        if (old_head)
        {
            res.swap(old_head->data);
            if (outstanding_hazard_pointers_for(old_head))
            {
                reclaim_later(old_head);
            }
            else
            {
                delete old_head;
            }
        }
        delete_nodes_with_no_hazards();
        return res;
    }

    void print() {
        for (auto p = head.load(); p != nullptr; p = p->next) {
            std::cout << *(p->data) << std::endl;
        }
    }
private:
    struct hazard_pointer
    {
        std::atomic<std::thread::id> id;
        std::atomic<void*> pointer;
    };

    static hazard_pointer hazard_pointers[MAX_HAZARD_POINTERS];

    class hp_owner
    {
    private:
        hazard_pointer* hp;
    public:
        hp_owner():hp(nullptr)
        {
            for (unsigned i = 0; i < MAX_HAZARD_POINTERS; i++)
            {
                std::thread::id old_id;
                if (hazard_pointers[i].id.compare_exchange_strong(old_id, std::this_thread::get_id()))
                {
                    hp = &hazard_pointers[i];
                    break;
                }
                if (!hp)
                {
                    throw std::runtime_error("No hazard pointers available");
                }
            }
        }
        hp_owner(hp_owner &&) = default;
        hp_owner(const hp_owner &) = delete;
        hp_owner &operator=(hp_owner &&) = default;
        hp_owner &operator=(const hp_owner &) = delete;
        ~hp_owner() 
        {
            hp->pointer.store(nullptr);
            hp->id.store(std::thread::id());
        }
        std::atomic<void*>& get_pointer() 
        {
            return hp->pointer;
        }
        
    };

    std::atomic<void*>& get_hazard_pointer_for_current_thread()
    {
        thread_local static hp_owner hazard;
        return hazard.get_pointer();
    }
    
    void delete_nodes_with_no_hazards()
    {
        data_to_reclaim* current_p = nodes_to_reclaim.exchange(nullptr);
        while (current_p) 
        {
            data_to_reclaim* const next = current_p->next;
            if (outstanding_hazard_pointers_for(current_p->data))
            {
                delete current_p;
            }
            else
            {
                add_to_reclaim_list(current_p);
            }
            current_p = next;
        }
    }

    bool outstanding_hazard_pointers_for(void* p)
    {
        for (unsigned i = 0; i < MAX_HAZARD_POINTERS; i++)
        {
            if (hazard_pointers[i].pointer.load() == p)
            {
                return true;
            }
        }
        return false;
    }

    template<typename U>
    void do_delete(void* p)
    {
        delete static_cast<U*>(p);
    }


    struct data_to_reclaim
    {
        void* data;
        std::function<void(void*)> deleter;
        data_to_reclaim* next;
        
        template<typename V>
        data_to_reclaim(void* p)
        {
            data = p; 
            deleter = &do_delete<V>;
            next = nullptr;
        }
        ~data_to_reclaim()
        {
            deleter(data);
        }
    };

    std::atomic<data_to_reclaim*> nodes_to_reclaim;
    void add_to_reclaim_list(data_to_reclaim* node)
    {
        node->next = nodes_to_reclaim.load();
        while (!nodes_to_reclaim.compare_exchange_weak(node->next, node));
    }

    template<typename W>
    void reclaim_later(W* data)
    {
        add_to_reclaim_list(new data_to_reclaim(data));
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
