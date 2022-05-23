#include <iostream>
#include <atomic>
#include <thread>
#include <mutex>


class spinlock_mutex
{
    std::atomic_flag flag;
public:
    spinlock_mutex():flag(ATOMIC_FLAG_INIT) { }

    void lock()
    {
        while (flag.test_and_set(std::memory_order_acquire));
    }

    void unlock()
    {
        flag.clear(std::memory_order_release);
    }    

};

int main(int argc, const char** argv) {
    spinlock_mutex m;
    std::thread t1([&]{
        m.lock();
        std::cout << "---------------------t1 get the mutex.------------------\n" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        m.unlock();
        std::cout << "+++++++++++++++t1 sleep 5s and release mutex++++++++++++\n" << std::endl;
    });

    std::thread t2([&]{
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        std::cout << "t2 try get mutex----------------------------------------\n" << std::endl;
        m.lock();
        std::cout << "---------------------t2 get the mutex.------------------\n" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        m.unlock();
        std::cout << "+++++++++++++++t2 sleep 3s and release mutex++++++++++++\n" << std::endl;
    });

    t1.join();
    t2.join();
    return 0;
}