#include <bank/object.hpp>
#include <bank/bank.hpp>

#include <iostream>

struct object : public bank::object
{
    void print(void) { std::cout << "Address of object: " << this << std::endl; }
};

int main(void)
{
    std::cout << "Press Enter to start..." << std::hex << std::showbase << std::endl;
    std::cin.get();
    bank::open();
    std::cout << "new" << std::endl;
//    synk::sleep::seconds(5);
    object* x = new object();
    x->print();
    delete x;
    std::cout << "delete" << std::endl;;
}
