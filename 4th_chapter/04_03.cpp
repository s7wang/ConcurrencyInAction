#include <iostream>
#include <thread>
#include <future>
#include <mutex>
#include <unistd.h>

int find_the_answer()
{
    std::cout << "find_the_answer start---------------------------" << std::endl;
    sleep(5);
    std::cout << "find_the_answer over---------------------------" << std::endl;
    return 0;
}

void do_something()
{
    std::cout << "do_something start---------------------------" << std::endl;
    sleep(1);
    std::cout << "do_something over---------------------------" << std::endl;
}

int main(int argc, const char** argv) {
    
    std::future<int> ft = std::async(find_the_answer);
    do_something();
    int res = ft.get();
    std::cout << "answer is " << res << std::endl;

    return 0;
}