#include <iostream>
#include <thread>
#include <mutex>

bool flag = false;
std::mutex m;

void wait_for_flag()
{
    std::unique_lock<std::mutex> lk(m);
    
    while (!flag)
    {
        lk.unlock();
        std::cout << "waiting target finish...... ......" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        lk.lock();
    }
    std::cout << "target has finished!" << std::endl;
    
}

void target()
{
    std::cout << "/* target is running...... */" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    
    std::unique_lock<std::mutex> lk(m);
    flag = true;
    std::cout << "/* target is end...... */" << std::endl;

}

int main(int argc, const char** argv) {
    
    std::thread t1(wait_for_flag);
    std::thread t2(target);

    t1.join();
    t2.join();

    return 0;
}