#include <iostream>
#include <thread>
#include <unistd.h>

class ThreadGuard
{

    std::thread& t;

public:
    ThreadGuard(std::thread &t_):t(t_) {}

    ~ThreadGuard()
    {
        if (t.joinable())
        {
            t.join();
        }
    }
    ThreadGuard(ThreadGuard const &) = delete;
    ThreadGuard &operator=(ThreadGuard const &) = delete;
private:
    ThreadGuard(ThreadGuard &&) = default;

    ThreadGuard &operator=(ThreadGuard &&) = default;

    
};

void do_something(int& i)
{

    std::cout << i++ << "::" << "do_something()" << std::endl;
}

class func
{
public:
    int& i;
    func(int& i_):i(i_)
    {

    }
    void operator()() const
    {
        std::cout << "thread start--------------------." << std::endl;
        for (int j = 0; j < 100; j++)
        {   
            std::cout << j << "::";
            do_something(i);
            usleep(2);
        }
        
    }
};

void oops()
{
    int stat = 0;
    func myfunc(stat);
    std::thread my_thread(myfunc);
    ThreadGuard g(my_thread);
    std::cout << "start usleep in thread." << std::endl;
    usleep(5);
    std::cout << "stop usleep in thread." << std::endl;
}



int main(int argc, const char** argv) {

    oops();
    std::cout << "main-------------------." << std::endl;
    usleep(10);
    return 0;
}