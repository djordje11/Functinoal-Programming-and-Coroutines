#include <optional>
#include <coroutine>
#include <utility>
#include <cstdlib>

#include "return_holder.hpp"

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
        std::shared_ptr<std::optional<T>> m_opt = std::make_shared<std::optional<T>>();
        std::suspend_never initial_suspend()
        {
            return {};
        }
        std::suspend_never final_suspend() noexcept
        {
            return {};
        }
        return_holder<std::optional<T>> get_return_object()
        {
            return m_opt;
        }
        void return_value(T value)
        {
            *m_opt = value;
        }
        void unhandled_exception() 
        {
            std::exit(1);
        }
    };
};