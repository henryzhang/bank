#include <bank/object.hpp>
#include <bank/bank.hpp>
#include <bank/allocator.hpp>

#include <vector>

struct object : public bank::object
{
    object() : buffer(bank::alloc(65536)) { }
    ~object() { bank::free(this->buffer); }
    void* buffer;
};

typedef std::vector<int, bank::allocator<int>> int_vector;

void func(void)
{
    int_vector y;
    for (size_t idx = 0; idx < 100000; ++idx) { y.push_back(idx); }
}

int main(void)
{
    bank::open(1);
    object* x = new object();
    func();
    delete x;
    bank::close();
}
