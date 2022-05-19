#include <optional>
#include <coroutine>
#include <memory>
#include <cstdlib>
#include <iostream>



template <typename T>
class my_shared 
{
public:
    std::shared_ptr<T> m_ptr;
    std::shared_ptr<bool> m_empty;
public:
    my_shared(std::shared_ptr<T> ptr) : m_ptr(ptr), m_empty(std::make_shared<bool>(true)) {}    
    operator std::optional<T>()
    {
        if(!*m_empty)
            return std::make_optional<T>(*m_ptr);
        return {};
    }
    void set_value(T value) {*m_ptr = value;*m_empty = false;}
};


template <typename T>
class optional_awaiter
{
public:
    std::optional<T> m_opt;
    bool await_ready() const
    {
        return m_opt.has_value(); 
    }
    void await_suspend(std::coroutine_handle<> h)
    {
        h.destroy();
    }
    T await_resume() const noexcept
    {
        return m_opt.value();
    }
};

template <typename T>
optional_awaiter<T> operator co_await(std::optional<T> opt) noexcept
{
    return optional_awaiter<T>{opt};
}

template <typename T, typename... Arguments>
class std::coroutine_traits<std::optional<T>, Arguments...>
{
public:
    struct promise_type
    {
        my_shared<T> ptr = std::make_shared<T>(); 
        std::suspend_never initial_suspend()
        {
            return {};
        }
        std::suspend_never final_suspend() noexcept
        {
            return {};
        }
        my_shared<T> get_return_object()
        {
            return ptr;
        }
        void return_value(T value)
        {
           ptr.set_value(value); 
        }
        void unhandled_exception() 
        {
            std::exit(1);
        }
    };
};