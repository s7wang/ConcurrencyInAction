#include <iostream>
#include <thread>
#include <mutex>

std::mutex some_mutex;

std::unique_lock<std::mutex> get_lock()
{
    
    std::unique_lock<std::mutex> lk(some_mutex, std::defer_lock);
    lk.lock();
    //some other operations
    std::cout << "prepare_data()" << std::endl;
    return lk; 
}

void process_data()
{
    std::unique_lock<std::mutex> lk(get_lock());
    lk.try_lock();
    //do something
    std::cout << "do_something()" << std::endl;
}
int main(int argc, const char** argv) {
    process_data();
    return 0;
}