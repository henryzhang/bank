#include <bank/bank.hpp>
#include <bank/new.hpp>

#include <iostream>

int main(void)
{
    std::cout << "Press Enter to start..." << std::endl;
    std::cin.get();
    bank::open();
    std::cout << "Now Open" << std::endl;
    std::cout << "new" << std::endl;
    int* x = new int;
    *x = 4;
    std::cout << *x << std::endl;
    
    delete x;
    std::cout << "delete" << std::endl;;
}
