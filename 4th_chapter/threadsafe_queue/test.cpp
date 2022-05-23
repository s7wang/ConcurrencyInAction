#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <unistd.h>




int main(int argc, const char** argv) {

    std::mutex mut;

    std::thread t1([&]{
        std::lock_guard<std::mutex> lk(mut);
        std::cout << "t1 -------------------------" << std::endl;
        sleep(10);
        std::cout << "t1 -------------------------over" << std::endl;
    });
    sleep(1);
    std::thread t2([&]{
        std::lock_guard<std::mutex> lk(mut);
        std::cout << "t2 -------------------------" << std::endl;
    });

    t1.join();
    t2.join();
    return 0;
}