#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <unistd.h>

std::mutex mut;
std::queue<int> data_queue;
std::condition_variable data_cond;

void data_preparation_thread()
{
    
    for (int i = 0; i < 10; i++)
    {
        std::lock_guard<std::mutex> lk(mut,std::adopt_lock);
        data_queue.push(i);
        std::cout << "----------------------" << std::endl;
        data_cond.notify_one();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        sleep(1);
    } 
}

void data_processing_thread()
{
    while (true)
    {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [&]{
            return !data_queue.empty();
        });
        int data = data_queue.front();
        data_queue.pop();
        lk.unlock();
        std::cout << data << std::endl;
        if(data == 9)
        {
            break;
        }
    }
    
}

int main(int argc, const char** argv) {
    std::thread t1(data_processing_thread);
    sleep(1);
    std::thread t2(data_preparation_thread);
    // std::thread t3([&]{
    //     while (1)
    //     {
    //         sleep(1);
    //         mut.lock();
    //         std::cout << "lockec mut-----------------" << std::endl;
    //         sleep(1);
    //     } 
    // });
    // t3.join();

    t1.join();
    t2.join();
    
    return 0;
}