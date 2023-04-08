#ifndef __RETURN__HOLDER__
#define __RETURN__HOLDER__

#include <memory>

template <typename T>
class return_holder 
{
public:
    std::shared_ptr<T> m_ptr;
public:
    return_holder(std::shared_ptr<T> ptr) : m_ptr(ptr) {}    
    operator T()
    {
        return *m_ptr;
    }
};

#endif