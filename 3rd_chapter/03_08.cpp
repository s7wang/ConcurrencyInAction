#include <iostream>
#include <memory>
#include <thread>
#include <mutex>

class X
{
private:
    
    int num;
    X() { };
public:
    static X* get_instance()
    {
        static X* instance;
        static std::once_flag flag;
        std::call_once(flag, []{instance = new X;});
        
        return instance;
    }
    void set_num(int new_num)
    {
        num = new_num;
    } 
    void show_num()
    {
        std::cout << "num = " << num << std::endl;
    }
    X(const X &) = delete;
    X &operator=(const X &) = delete;
    ~X(){};    
};

int main(int argc, const char** argv) {

    X* a = X::get_instance();
    std::cout << "a set num:2333333" << std::endl;
    a->set_num(2333333);
    std::cout << "a.show() num is " ;
    a->show_num();
    X* b = X::get_instance();
    std::cout << "b.show() num is " ;
    b->show_num();
    std::cout << "b set num:55555" << std::endl;
    b->set_num(55555);
    std::cout << "a.show() num is " ;
    a->show_num();

    std::thread t([]{
        X* c = X::get_instance();
        std::cout << "[Thread]c.show() num is " ;
        c->show_num();
    });
    t.join();
    return 0;
}


