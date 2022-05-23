#include <iostream>
#include <thread>
#include <unistd.h>
#include "queue_s.h"
#include "queue_p.h"
#include "ts_queue_puppet_v2.h"



int main(int argc, const char** argv) {
    
    //threadsafe_queue<int> queue;
    //ts_queue_p<int> queue;
    ThreadSafeQueue<int> queue;
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
            //int data = -1;
            //queue.try_pop(data);
            std::shared_ptr<int> data = queue.try_pop();
            if (data != nullptr)
                std::cout << "try_pop " << *data << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    });   

    t1.join();
    // t2.join();
    t3.join();
    return 0;
}