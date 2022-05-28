#include <optional>
#include <coroutine>
#include <utility>
#include <cstdlib>



template <typename T>
class my_shared 
{
public:
    T*& m_ptr;
public:
    my_shared(T*& ptr) : m_ptr(ptr) {}    
    operator std::optional<T>()
    {
        if(m_ptr)
            return std::make_optional<T>(*m_ptr);
        return {};
    }
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
        T* ptr = nullptr;
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
            ptr = new T(std::move(value));
        }
        void unhandled_exception() 
        {
            std::exit(1);
        }
    };
};