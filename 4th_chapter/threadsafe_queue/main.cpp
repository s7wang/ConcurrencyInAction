#include <iostream>
#include <thread>
#include <unistd.h>
#include "threadsafe_queue.h"



int main(int argc, const char** argv) {
    
    threadsafe_queue<int> queue;
    volatile bool flag = true;
    std::thread t1([&]{  
        for (int i = 0; i < 1000; i++)
        {
            queue.push(i);
            std::cout << "push " << i << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        flag = false;     
    });
    // std::thread t2([&]{    
    //     while (flag)
    //     {
    //         int data = -1;
    //         queue.wait_and_pop(data);
    //         std::cout << "wait_and_pop " << data << std::endl;
    //         std::this_thread::sleep_for(std::chrono::milliseconds(5));
    //     }
    // });
    std::thread t3([&]{
        while (flag)
        {
            int data = -1;
            queue.try_pop(data);
            std::cout << "try_pop " << data << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    });   

    t1.join();
    // t2.join();
    t3.join();
    return 0;
}