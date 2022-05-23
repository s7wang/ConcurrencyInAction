#include <iostream>
#include <thread>
#include <future>
#include <string>

struct X
{
    void foo(int, std::string const&);
    std::string bar(std::string const&);
};
X x;

auto f1 = std::async(&X::foo, &x, 42, "hello");
auto f2 = std::async(&X::bar, x, "goodbye");

struct Y
{
    double operator()(double);
};
Y y;

auto f3 = std::async(Y(), 3.14);
auto f4 = std::async(std::ref(y), 2.718);

X baz(X& a);

auto f5 = std::async(baz, std::ref(x));

class move_only
{
public:
    move_only();
    move_only(move_only &&) = default;
    move_only &operator=(move_only &&) = default;

    move_only(const move_only &) = delete;
    move_only &operator=(const move_only &) = delete;
    ~move_only();

    void operator()();  
};

auto f6 = std::async(move_only());



auto f7 = std::async(std::launch::async, Y(), 1.2);
auto f8 = std::async(std::launch::deferred, baz, std::ref(x));
auto f9 = std::async(
            std::launch::async | std::launch::deferred, 
            baz, std::ref(x));
auto f0 = std::async(baz, std::ref(x));

