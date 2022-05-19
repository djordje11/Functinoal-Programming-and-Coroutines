#include "lazy.hpp"
#include <iostream>

my_optional<int> cor1()
{
    std::cout << "cor1" << std::endl;
    return 1;
}
my_optional<int> cor2(int x)
{
    std::cout << "cor2" << std::endl;
    return {};
}
my_optional<int> cor()
{
    
    int x = co_await cor1();
    int y = co_await cor2(x);
    co_return y;
}

int main()
{
    my_optional<int> lal = cor();
    if(lal.m_opt.has_value())
        std::cout << lal.m_opt.value() << std::endl;
    else
        std::cout << "No value" <<  std::endl;

    return 0;
}