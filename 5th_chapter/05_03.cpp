#include <sstream>
#include <locale>
#include <iostream>
#include <atomic>
#include <thread>
#include <mutex>


int main(int argc, const char** argv) {

    bool flag;
    int exp =0;
    std::atomic<int> data;
    data = 0;

    std::thread t1([&]{
        while (true)
        {
            data+=1;
            std::cout << data.load() << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        
    });

    std::thread t2([&]{
        while (true)
        {
            data+=1;
            std::cout << data.load() << std::endl;
            
        }
    }); 
   

    t1.join();
    t2.join();

    return 0;
}