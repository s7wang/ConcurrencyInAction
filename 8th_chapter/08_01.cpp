#include <iostream>
#include <algorithm>
#include <stack>
#include <list>
#include <vector>
#include <future>
#include <chrono>

#include "threadsafe_stack.h"

template<typename T>
class chunk_to_sort
{
public:
    std::list<T> data;
    std::promise<std::list<T> > promise;

    chunk_to_sort() {}
    chunk_to_sort(const chunk_to_sort &other)
    {
        data = other.data;
    }

};

template<typename T>
class sorter
{
private:
    threadsafe_stack<chunk_to_sort<T>> chunks;
    std::vector<std::thread> threads;
    unsigned const max_thread_count;
    std::atomic<bool> end_of_data;

public:
    sorter() :
        max_thread_count(std::thread::hardware_concurrency() - 1),
        end_of_data(false)
    {}
    ~sorter()
    {
        end_of_data = true;
        for (unsigned i = 0; i < threads.size(); ++i)
        {
            threads[i].join();
        }
    }
    void try_sort_chunk()
    {
        if (chunks.empty()) 
            return;
        std::shared_ptr<chunk_to_sort<T> > chunk = chunks.pop();
        if (chunk)
        {
            sort_chunk(chunk);
        }
    }
    std::list<T> do_sort(std::list<T>& chunk_data)
    {
        if (chunk_data.empty())
        {
            return chunk_data;
        }
        std::list<T> result;
        // 将chunk_data中第一个元素取出放入result
        result.splice(result.begin(), chunk_data, chunk_data.begin());
        // 将该元素值作为base值
        T const& partition_val = *result.begin();
        
        // 将chunk_data中剩余元素根据base值分区
        typename std::list<T>::iterator divide_point =
            std::partition(chunk_data.begin(), chunk_data.end(),
                [&](T const& val) {return val < partition_val; });
        // 小于base值的部分保存至new_lower_chunk，splice之后的chunk_data即为大于base值的部分
        chunk_to_sort<T> new_lower_chunk;
        new_lower_chunk.data.splice(new_lower_chunk.data.end(),
            chunk_data, chunk_data.begin(),
            divide_point);
        // 小于base部分的list压入全局类型栈
        std::future<std::list<T> > new_lower =
            new_lower_chunk.promise.get_future();
        chunks.push(std::move(new_lower_chunk));
        // 线程数组添加线程 ？？如果超过最大线程数会怎样？？
        if (threads.size() < max_thread_count)
        {
            threads.push_back(std::thread(&sorter<T>::sort_thread, this));

        }

        // 大于base值的部分递归调用自身，继续进行分区排序
        std::list<T> new_higher(do_sort(chunk_data));
        // 将排好序的大于base部分的list拼接到result
        result.splice(result.end(), new_higher);
        // 小于base部分的list如果未完成则调用try_sort_chunk
        while (new_lower.wait_for(std::chrono::seconds(0)) !=
            std::future_status::ready)
        {
            try_sort_chunk();
        }
        // 将排好序的小于base部分list拼接到result，形成完整排好序的list
        result.splice(result.begin(), new_lower.get());
        // 返回结果
        return result;
    }
    void sort_chunk(std::shared_ptr<chunk_to_sort<T> > const& chunk)
    {
        chunk->promise.set_value(do_sort(chunk->data));
    }
    void sort_thread()
    {
        while (!end_of_data)
        {
            try_sort_chunk();
            std::this_thread::yield();
        }
    }
};
 
template<typename T>
std::list<T> parallel_quick_sort(std::list<T> input) //代表了sorter类的大部分功能
{
    if (input.empty())
    {
        return input;
    }
    sorter<T> s;
    return s.do_sort(input);
}


int main(int argc, char const *argv[])
{
    auto print = [](const int& n) {std::cout << " " << n; };
    std::list<int> table = {52, 63, 8, -3, 0, 999, 66, 128, -60};
    std::for_each(table.begin(), table.end(), print);
    std::cout << std::endl;

    std::list<int> res = parallel_quick_sort(table);

    

    std::for_each(res.begin(), res.end(), print);
    std::cout << std::endl;

    return 0;
}
