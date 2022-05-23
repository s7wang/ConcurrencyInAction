#include <iostream>
#include <thread>

void do_something()
{
    std::cout << "do_something()" << std::endl;
}
void do_something_else()
{
    std::cout << "do_something_else()" << std::endl;
}
class background_task
{

public:
    void operator()() const
    {
        do_something();
        do_something_else();
    }
};

int main(int argc, const char** argv) {
    
    std::thread thread1((background_task()));
    std::thread thread2{background_task()};
    thread1.join();
    thread2.join();
    return 0;
}