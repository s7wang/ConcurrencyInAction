#include <sstream>
#include <locale>
#include <iostream>
#include <atomic>
#include <thread>
#include <mutex>


int main(int argc, const char** argv) {

    std::atomic<bool> flag;
    
    std::thread t1([&]{
        flag = false;
        std::cout << "t1 set flag = FALSE       by = operator.\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        flag.store(true);
        std::cout << "t1 set flag = TRUE        by store().\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        bool old_flag = flag.exchange(false, std::memory_order_acq_rel);
        std::cout << "t1 set flag = FALSE       by exchange().\n";
        std::cout << "t1::old_flag = " << std::boolalpha << old_flag << std::endl;

    });

    std::thread t2([&]{
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        for (size_t i = 0; i < 5; i++)
        {
            if (flag.load(std::memory_order_acquire))
            {
                std::cout << "t2 read flag is TURE  by load().\n";
            }
            else
            {
                std::cout << "t2 read flag is FALSE by load().\n";
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        }  

    });

    t1.join();
    t2.join();
    return 0;
}