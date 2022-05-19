#include "lazy.hpp"
#include <iostream>

std::optional<int> cor1()
{
    std::cout << "cor1" << std::endl;
    return 1;
}
std::optional<int> cor2(int x)
{
    std::cout << "cor2" << std::endl;
    return 5 + x;
}
std::optional<int> cor()
{
    
    int x = co_await cor1();
    int y = co_await cor2(x);
    co_return y;
}

int main()
{
    std::optional<int> lal = cor();
    if(lal.has_value())
        std::cout << lal.value() << std::endl;
    else
        std::cout << "No value" <<  std::endl;

    return 0;
}