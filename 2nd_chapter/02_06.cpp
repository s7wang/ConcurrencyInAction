#include <iostream>
#include <thread>
#include <chrono>

 
void foo()
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << std::hex << "id of thread foo() is " << std::this_thread::get_id() << std::endl;
}
 
int main(int argc, char** argv)
{
    std::thread t1(foo);
    std::thread::id t1_id = t1.get_id();
 
    std::thread t2;
    std::thread::id t2_id = t2.get_id();
 
    std::cout << std::hex << "t1's id: " << t1_id << '\n';
    std::cout << "t2's id: " << t2_id << '\n';
    if(t2_id == std::thread::id())
    {
        std::cout << "/* message */" << std::endl;
    }
    
 
    t1.join();
   
}