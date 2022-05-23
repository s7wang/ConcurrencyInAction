#include <iostream>
#include <thread>
#include <string>
#include <mutex>
#include <stack>


void swap(int& ldata, int& rdata)
{
    int temp;
    temp = ldata;
    ldata = rdata;
    rdata = temp;
}


class X
{
private:
    int _data;
    std::mutex m;
public:
    X(const int& data):_data(data){ }
    virtual ~X() {}

    friend void swap(X& lhs, X& rhs)
    {
        if (&lhs == &rhs)
        {
            return;
        }
        std::lock(lhs.m, rhs.m);
        std::lock_guard<std::mutex> lock_l(lhs.m, std::adopt_lock);
        std::lock_guard<std::mutex> lock_r(rhs.m, std::adopt_lock);
        swap(lhs._data, rhs._data);
    }
};

int main(int argc, const char** argv) {
    return 0;
}