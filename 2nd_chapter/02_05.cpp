#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include <functional>
#include <string>
#include <unistd.h>

void do_work(unsigned int id)
{
    std::cout << "thread::" + std::to_string(id) + " start!++++++++++++++\n" << std::endl;
    sleep(3);
    std::cout << "thread::" + std::to_string(id) + " stop!---------------\n" << std::endl;
}

void f()
{
    std::vector<std::thread> threads;
    for (unsigned int i = 0; i < 20 ;i++)
    {
        threads.push_back(std::thread(do_work, i));
    }
    std::vector<std::thread>::iterator it = threads.begin();
    for ( ;it != threads.end(); it++)
    {
        it->join();
    }
    //std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
}
int main(int argc, const char** argv) {
    std::cout << "hardware_concurrency is " << std::thread::hardware_concurrency() << std::endl;
    return 0;
}


