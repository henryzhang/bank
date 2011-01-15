#include <bank/object.hpp>
#include <bank/bank.hpp>
#include <bank/allocator.hpp>

#include <iostream>
#include <vector>

struct object : public bank::object
{
    object() : buffer(bank::alloc(65536)) { }
    ~object() { bank::free(this->buffer); }
    void print(void) { std::cout << "Address of object: " << this << std::endl; }
    void* buffer;
};

typedef std::vector<int, bank::allocator<int>> int_vector;

void func(void)
{
    int_vector y;
    for (size_t idx = 0; idx < 1000; ++idx) { y.push_back(idx); }
}

int main(void)
{
    std::cout << "Press Enter to start..." << std::hex << std::showbase << std::endl;
    std::cin.get();
    bank::open(5);
    std::cout << "new" << std::endl;
    object* x = new object();
    func();
    x->print();
    delete x;
    std::cout << "delete" << std::endl;
    bank::close();
}
