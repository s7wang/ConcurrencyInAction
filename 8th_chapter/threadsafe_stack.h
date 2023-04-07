#ifndef EFFF33AC_540D_4158_811C_8F49E93022C8
#define EFFF33AC_540D_4158_811C_8F49E93022C8
#ifndef _THREADSAFE_STACK_
#define _THREADSAFE_STACK_

#include <stack>
#include <mutex>
#include <exception>
#include <typeinfo>
#include <memory> //For std::shared_ptr<>

struct empty_stack: std::exception
{
    const char* what() const throw(){return "This is a empty stack!\n";};
};

template<typename T>
class threadsafe_stack
{
private:
    std::stack<T> data;
    mutable std::mutex m;
public:
    threadsafe_stack();
    threadsafe_stack(const threadsafe_stack &);
    threadsafe_stack &operator=(threadsafe_stack &&) = delete;
    threadsafe_stack &operator=(const threadsafe_stack &) = delete;
    virtual ~threadsafe_stack();

    void push(T new_value);
    void pop(T& value);
    std::shared_ptr<T> pop();
    bool empty() const;

};

template<typename T>
threadsafe_stack<T>::threadsafe_stack() { }

template<typename T>
threadsafe_stack<T>::threadsafe_stack(const threadsafe_stack &other)
{
    std::lock_guard<std::mutex> lock(other.m);
    data = other.data;
}

template<typename T>
threadsafe_stack<T>::~threadsafe_stack() { }

template<typename T>
void threadsafe_stack<T>::push(T new_value)
{
    std::lock_guard<std::mutex> lock(m);
    data.push(new_value);
}

template<typename T>
void threadsafe_stack<T>::pop(T& value)
{
    std::lock_guard<std::mutex> lock(m);
    if (data.empty()) throw empty_stack();
    value = data.top();
    data.pop();
}

template<typename T>
std::shared_ptr<T> threadsafe_stack<T>::pop()
{
    std::lock_guard<std::mutex> lock(m);
    if (data.empty()) throw empty_stack();
    std::shared_ptr<T> const res(std::make_shared<T>(data.top()));
    data.pop();
    return res;
}

template<typename T>
bool threadsafe_stack<T>::empty() const
{
    std::lock_guard<std::mutex> lock(m);
    return data.empty();
}

#endif // !_THREADSAFE_STACK_


#endif /* EFFF33AC_540D_4158_811C_8F49E93022C8 */
