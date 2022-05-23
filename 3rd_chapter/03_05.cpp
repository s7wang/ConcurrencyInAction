#include <iostream>
#include <thread>
#include <string>
#include <mutex>
#include <stack>

#include "hierarchical_mutex.h"

using namespace std;

hierarchical_mutex hight_level_mutex(10000);
hierarchical_mutex low_level_mutex(5000);

int do_low_level_stuff()
{
    return 0;
}
int low_leve_func()
{
    std::lock_guard<hierarchical_mutex> lk(low_level_mutex);
    return do_low_level_stuff();
}

void do_high_level_stuff(int data)
{
    if (data == 0)
        std::cout << "do_high_level_stuff(int data):data == 0" << std::endl;
}

void high_level_func()
{
    std::lock_guard<hierarchical_mutex> lk(hight_level_mutex);
    do_high_level_stuff(low_leve_func());
}

void thread_a()
{
    high_level_func();
}

hierarchical_mutex other_level_mutex(100);
void do_other_level_stuff()
{
    std::cout << "do_other_level_stuff()" << std::endl;
}

void other_level_func()
{
    high_level_func();
    do_other_level_stuff();
}

void thread_b()
{
    std::lock_guard<hierarchical_mutex> lk(other_level_mutex);
    other_level_func();
}

int main(int argc, const char** argv) {

    thread ta(thread_a);
    thread tb(thread_b);

    ta.join();
    tb.join();

    return 0;
}