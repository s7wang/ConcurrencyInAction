#include <iostream>
#include <thread>
#include <list>
#include <mutex>
#include <algorithm>

std::list<int> some_list;
std::mutex some_mutex;

void add_to_list(int new_value)
{
    std::lock_guard<std::mutex> gurad(some_mutex);
    some_list.push_back(new_value);
}

void list_contains(int value_to_find)
{
    std::lock_guard<std::mutex> guard(some_mutex);
    if (std::find(some_list.begin(), some_list.end(), value_to_find)
        != some_list.end())
    {
        std::cout << "have found value = " << value_to_find << std::endl;
    }
    else
    {
        std::cout << "have not found value = " << value_to_find << std::endl;
    }
}

int main(int argc, const char** argv) {
    std::thread t1(add_to_list, 5);
    std::thread t2(add_to_list, 7);
    std::thread t3(list_contains, 5);
    std::thread t4(list_contains, 8);
    std::thread t5(add_to_list, 8);
    std::thread t6(add_to_list, 9);

    t1.join(); t2.join(); t3.join(); 
    t4.join(); t5.join(); t6.join();

    for (auto it = some_list.begin(); it != some_list.end(); it++)
        std::cout << *it << std::endl;
    
    return 0;
}