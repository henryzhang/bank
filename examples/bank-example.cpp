#include <bank/object.hpp>
#include <bank/bank.hpp>
#include <bank/new.hpp>

#include <synk/utility.hpp>

#include <iostream>

struct object : public bank::object
{
    void print(void) { std::cout << std::hex << std::showbase << this << std::endl; }
};

int main(void)
{
    std::cout << "Press Enter to start..." << std::endl;
    std::cin.get();
    bank::open();
    std::cout << "Now Open" << std::endl;
    std::cout << "new" << std::endl;
    synk::sleep::seconds(5);
    object* x = new object();
    x->print();
    delete x;
    std::cout << "delete" << std::endl;;
}
