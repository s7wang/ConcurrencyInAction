#include <iostream>
#include <thread>
#include <unistd.h>

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
        for (int j = 0; j < 1000000; j++)
        {   
            std::cout << j << "::";
            do_something(i);
        }
        
    }
};

void oops()
{
    int stat = 0;
    func myfunc(stat);
    std::thread my_thread(myfunc);
    my_thread.detach();
    usleep(10);
}
int main(int argc, const char** argv) {
    
    oops();
    usleep(10);
    return 0;
}