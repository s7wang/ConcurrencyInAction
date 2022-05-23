#include <iostream>
#include <thread>
#include <string>
#include <mutex>

std::shared_ptr<std::string> string_ptr;
std::once_flag string_flag;

void init_string()
{
    string_ptr.reset(new std::string("Hi~"));
}

void foo()
{
    std::call_once(string_flag, init_string);
    std::cout << "string = " << *string_ptr << std::endl;
}

int main(int argc, const char** argv) {

    foo();
    return 0;
}