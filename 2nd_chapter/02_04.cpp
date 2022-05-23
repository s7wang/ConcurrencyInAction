#include <iostream>
#include <thread>

class scoped_thread
{
    std::thread t;
public:
    explicit scoped_thread(std::thread t_):t(std::move(t_))
    {
        if (!t.joinable())
        {
            throw std::logic_error("No thread");
        }
    } 

    virtual ~scoped_thread()
    {
        t.join();
    }

    scoped_thread(scoped_thread &&) = delete;
    scoped_thread(const scoped_thread &) = delete;
    scoped_thread &operator=(scoped_thread &&) = delete;
    scoped_thread &operator=(const scoped_thread &) = delete;

};

void fun(int state)
{
    for (int i = 0; i < 1000; i++)
        std::cout << "Now in fun, state is " << i << "::" << state << std::endl;
}

void f()
{
    int some_local_state = 666;
    scoped_thread t(std::thread(fun, some_local_state));
    // do something else.
}

int main(int argc, const char** argv) {
    f();
    return 0;
}