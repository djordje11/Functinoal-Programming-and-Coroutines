#include <iostream>
#include <string>
#include "optional_coroutine.hpp"
#include "expected_coroutine.hpp"
#include "expected.hpp"
std::optional<int> cor1()
{
    std::cout << "cor1" << std::endl;
    return 1;
}
std::optional<int> cor2(int x)
{
    std::cout << "cor2" << std::endl;
    return 3;
}
std::optional<int> cor3(int x)
{
    std::cout << "cor3" << std::endl;
    return 3 + x;
}
std::optional<int> cor()
{
    
    int x = co_await cor1();
    int y = co_await cor2(x);
    int z = co_await cor3(y);
    co_return z;
}

tl::expected<int, std::string> exp_cor1()
{
    int k;
    std::cout << "Enter k less than 100" << std::endl;
    std::cin >> k;
    return k >= 100 ? tl::unexpected(std::string("greater than 100 is entered in exp_cor1")) : tl::expected<int, std::string>(k);
}

tl::expected<int, std::string> exp_cor2(int x)
{
    int k;
    std::cout << "Enter k less than 100" << std::endl;
    std::cin >> k;
    return k >= 100 ? tl::unexpected(std::string("greater than 100 is entered in exp_cor2")) : tl::expected<int, std::string>(k + x);
}

tl::expected<int, std::string> exp_cor3(int x)
{
    int k;
    std::cout << "Enter k less than 100" << std::endl;
    std::cin >> k;
    return k >= 100 ? tl::unexpected(std::string("greater than 100 is entered in exp_cor3")) : tl::expected<int, std::string>(k + x);
}

tl::expected<int, std::string> exp_cor()
{
    int x = co_await exp_cor1();
    int y = co_await exp_cor2(x);
    int z = co_await exp_cor3(y);
    co_return z;
}

int main()
{
    std::shared_ptr<tl::expected<int, std::string>> ptr = std::make_shared<tl::expected<int, std::string>>();
    std::optional<int> lal = cor();
    if(lal.has_value())
        std::cout << lal.value() << std::endl;
    else
        std::cout << "No value" <<  std::endl;


    std::cout << "Expected " << std::endl;

    tl::expected<int, std::string> exp = exp_cor();

    if(exp.has_value())
        std::cout << exp.value() << std::endl;
    else 
        std::cout << exp.error() << std::endl; 

    return 0;
}