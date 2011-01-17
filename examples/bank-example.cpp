#include <bank/object.hpp>
#include <bank/bank.hpp>
#include <bank/allocator.hpp>

#include <vector>
#include <deque>

struct object : public bank::object
{
    object() : buffer(bank::alloc(65536)) { }
    ~object() { bank::free(this->buffer); }
    void* buffer;
};

typedef std::vector<int, bank::allocator<int>> int_vector;
//typedef std::vector<int> int_vector;

void func(void)
{
    int_vector y(200);
}

int main(void)
{
    bank::open(1);
    object* x = new object();
    for(size_t idx = 0; idx < 1000000; ++idx) { func(); }
    delete x;
    bank::close();
}
