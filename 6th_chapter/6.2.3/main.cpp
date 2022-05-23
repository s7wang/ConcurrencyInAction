#include <iostream>
#include "queue_1.h"
#include "queue_puppet.h"
//#include <cstdio>

int main(int argc, const char** argv) {
    
    //queue_1<int> q;
    queue_puppet<int> q;
    for (int i = 1; i < 5; i++)
    {
        int data = i;
        q.push(data);
    }

    for (int i = 0; i < 4 ;i++)
    {
        std::cout << *(q.try_pop()) << std::endl;   
    }

    return 0;
}