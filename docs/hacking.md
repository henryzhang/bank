Modifying the Code for Bank
===========================

This document discusses some possible optimizations that can be made to bank, in the event that some form of
speed increase may be needed. In addition, reasons for variable names, as well as reasoning for certain idioms
or a lack thereof are discussed as well. The document also discusses how the actual allocation and collection
cycle works.  

The Allocator and Collector
---------------------------

Bank consists of two primary internal components. The pool, and the collector. The pool is what keeps track
of each allocation, and ensures that the memory is easily collected by the collector. When the pool is created
it calculates the maximum amount of memorythat the system has (not what it can allocate). It then divides this
amount into a set of 64KB chunks, which is then allocated into the underlying `array`, which contains the
`chunk` type that tracks the usage of these blocks of memory. By default, the pool starts with 256 chunks
(or 16 MB of memory). This number is changed by the call to `bank::open`. The pool does not keep track of the
actual allocated address, but instead relies on a given `chunk` to be able to return a requested `size`
(where `size` can be a maximum of `uint32_t::max + 1`, or 4GB). When the given address is deallocated,
the address is pushed onto a queue located in the collector. While the queue is not full, the collector will
constantly yield any or all of its execution time. (Note: On systems that do not have `pthread_yield`, 
`pthread_yield_np` is used instead). The queue is of a sufficiently large enough size that it would take
several thread deallocating at once to break it. (NOTE: The queue size can be tweaked for less memory usage).  

The collector then talks to the pool's array, and requests the chunks that contain the address to decrement
their count. This act is not threadsafe, but can be "saved" by changing the counter to an atomic type
(see Atomic Operations below).

This deferrment of deallocating resolves the possible (and well known) "Stop the World" scenario for most
garbage collectors, while still allowing the primary thread to allocate during deallocation.

Memory
------

Internally, bank uses `std::calloc` for all of its allocations. Most platforms that Bank has been tested on
showed a very slight increase in performance. However, since this increase is not guaranteed, it may be
necessary to replace each `std::calloc` call with `std::malloc`.  

Bank also allocates almost all memory in 64KB chunks, or multiples thereof. The only exception to this is
the overloading of `bank::detail::pool::operator new` and `bank::detail::collector::operator new`. These are
both overloaded due to the possible ignorance of the warning outlined in the primary documentation file.

Atomic Operations
-----------------

One of the primary goals of bank was to write as little platform specific code as possible. Obviously, not
all of this is 100% guaranteed, but one of the sacrifices made was the use of atomic types for two basic
operations. It is recommended that if you would like guaranteed stability in your application on a weak
ordered CPU (such as PowerPC), that a memory barrier, or atomic integral type is used for all types who have
the variable modifier `volatile` within the source code. Specifically, these are located in the `bank::queue`
and consist of a `size_t*` (that is, the pointer is volatile, not what it points to).  

Checking for Null
-----------------

Throughout Bank, there are checks to see if a `void*` is `NULL` or not. This was done to show intent, and to
allow newbies to better understand what is happening where (Think explicit over implicit). It is safe to
change these if you so wish.
