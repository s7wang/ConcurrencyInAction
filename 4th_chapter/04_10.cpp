#include <iostream>
#include <list>
#include <future>
#include <algorithm>

template<class F, class A>
std::future<typename std::result_of<F(A&&)>::type>
	spawn_task(F&& f, A&& a)
{
	typedef typename std::result_of<F(A&&)>::type result_type;
	std::packaged_task<result_type(A&&)>
		task(std::move(f));
		std::future<result_type> res(task.get_future());
		std::thread t(std::move(task), std::move(a));
		t.detach();
		return res;
}

template<typename T>
std::list<T> parallel_quick_sort(std::list<T> input)
{
    if (input.empty())
    {
        return input;
    }
    
    std::list<T> result;
    result.splice(result.begin(), input, input.begin());
    T const& pivot = *result.begin();
    
    auto divide_point = std::partition(input.begin(), input.end(), [&](T const& t){ return t < pivot; });
    std::list<T> lower_part;
    lower_part.splice(lower_part.end(), input, input.begin(), divide_point);
    
    //std::future<std::list<T> > future_lower = std::async(&parallel_quick_sort<T>, std::move(lower_part));
    auto future_lower = spawn_task(&parallel_quick_sort<T>, std::move(lower_part));
    auto new_higher( parallel_quick_sort(std::move(input)) );
    auto new_lower = future_lower.get();
    
    result.splice(result.end(), new_higher);
    result.splice(result.begin(), new_lower);

    return result;
}



int main(int argc, const char** argv) {

    std::list<int> a = {5, 6, 2, 7, 3, 95, 45, 68};
    std::list<int> b = parallel_quick_sort(a);

    for (int i : b)
    {
        std::cout << i << "  ";
    }
    std::cout << " " << std::endl;
    return 0;
}



