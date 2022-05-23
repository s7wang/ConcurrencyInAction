#include <iostream>
#include <string>
#include "threadsafe_list.hpp"
#include <thread>

int main(int argc, char const *argv[])
{

    threadsafe_list<std::string> test;

    test.push_front("a");
    // test.push_front("b");
    // test.push_front("c");

    // std::shared_ptr<std::string> b_ptr =  test.find_first_if([](std::string const& it){return (it ==  "b");});

    // std::cout << "b_ptr = " << *b_ptr << std::endl;

    // test.remove_if([](std::string const& it){return (it ==  "b");});

    // b_ptr =  test.find_first_if([](std::string const& it){return (it ==  "b");});
    // if (b_ptr == nullptr){
    //     std::cout << "/* message */" << std::endl;
    // }
    // else
    // {
    //     std::cout << "b_ptr = " << *b_ptr << std::endl;
    // }


    // std::shared_ptr<std::string> a_ptr =  test.find_first_if([](std::string const& it){return (it ==  "a");});

    // std::cout << "a_ptr = " << *a_ptr << std::endl;
    return 0;
}