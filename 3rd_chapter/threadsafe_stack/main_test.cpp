#include <iostream>
#include <thread>
#include <cstdlib>
#include <unistd.h>

#include "threadsafe_stack.h"

#define MAXNUM 20
int main(int argc, const char** argv) {
    threadsafe_stack<int> stack;
    bool state1 = true;
    bool state2 = true;

    std::thread add1([&]()
    {
        for (int i = 0; i < MAXNUM / 2; i++)
        {
            stack.push(i);
            std::cout << "thread add1::push " << i << " in the stack.\n" << std::endl;
            sleep(1);
        }
        state1 = false;

    });
        std::thread add2([&]()
    {
        for (int i = MAXNUM / 2; i < MAXNUM; i++)
        {
            stack.push(i);
            std::cout << "thread add2::push " << i << " in the stack.\n" << std::endl;
            sleep(1);
        }
        state2 = false;
    });

    std::thread del([&]()
    {
        std::shared_ptr<int> p;
        int value;
        int i = 0;
        while (state1 || state2)
        {
            if (stack.empty()) continue;
            if (i++ / 2 == 1)
            {
                p = stack.pop();
                std::cout << "std::shared_ptr<int> p = " << *p << std::endl;
            }
            else
            {
                stack.pop(value);
                std::cout << "value is " << value << std::endl;
            }
        }
        
    });

    add1.join();
    add2.join();
    del.join();

    return 0;
}