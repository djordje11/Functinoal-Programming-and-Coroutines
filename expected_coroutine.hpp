#include <coroutine>

#include "expected.hpp"
#include "return_holder.hpp"


template<typename T, typename E>
class expected_awaiter
{
    using promise_type = typename std::coroutine_traits<tl::expected<T, E>>::promise_type;
public:
    tl::expected<T, E> m_exp;

    bool await_ready() const
    {
        return m_exp.has_value();
    }

    void await_suspend(std::coroutine_handle<promise_type> h)
    {
        *(h.promise().m_exp) = m_exp;
        h.destroy();
    }

    T await_resume() const
    {
        return m_exp.value();
    }
};

template<typename T, typename E>
expected_awaiter<T, E> operator co_await(tl::expected<T, E> exp) noexcept
{
    return expected_awaiter<T, E>{exp};
}

template<typename T, typename E, typename... Arguments>
class std::coroutine_traits<tl::expected<T, E>, Arguments...>
{
public:

    struct promise_type
    {
        std::shared_ptr<tl::expected<T, E>> m_exp = std::make_shared<tl::expected<T, E>>();

        std::suspend_never initial_suspend()
        {
            return {};
        }

        std::suspend_never final_suspend() noexcept
        {
            return {};
        }       

        return_holder<tl::expected<T, E>> get_return_object()
        {
            return m_exp;
        }

        void return_value(T value)
        {
            *m_exp = value;
        }

        void unhandled_exception() 
        {
            std::exit(1);
        }        
    };
};



