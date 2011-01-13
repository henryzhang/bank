Using Bank
==========

This document outlines the following in relation to the bank garbage collector:

 * Basic Usage
 * Advanced Usage
 * A Warning Against Naivete

Basic Usage
-----------

Bank must be started via a call to `bank::open`. This function takes a `size_t`, equal to the number of chunks
required at collector initialization. Because Bank observes memory as allocated within a 64KB chunk, the
default starting size is 16MB, or 256 chunks. It is recommended that this call is the first within your
program.  

Likewise, the Bank collector must be closed with a call to `bank::close`. Both `bank::open` and `bank::close`
are located in the `bank/bank.hpp` header.  

The simplest way to use Bank after initialization is to inherit from the `bank::object` type. These types are
automatically allocated by Bank, and utilize the underlying memory pool, by overloading the object's
`operator new` method.  

Additionally memory can be manually allocated and freed via `bank::alloc` and `bank::free` located within the
`bank/bank.hpp` header.  

The only time that Bank will ever throw an exception (of type `bank::error`, located in `bank/error.hpp`) is
during `bank::open` if any of the essential components cannot be started (Namely the collector, as well as
the memory pool being unable to allocate the beginning memory block), *or* if the memory pool is no longer to
allocate memory due to all of the chunks in the pool being considered used, and the system is unable continue
to allocate any memory for use. The latter situation is an End of the World scenario and will only occur
if the operating system is unable to allocate anything to the application.  

More information on the types of internal situations that can cause a `bank::error` to be thrown can be found
in [hacking.md][1], as well as the reasoning for the use of an exception in the collector.

Advanced Usage
--------------

Bank also comes with several additional "advanced" features. These are:

 * C++03 STL Compliant Allocator
 * A "smart" pointer which uses the `bank::alloc` and `bank::free` functions for allocation and deallocation.

The `bank::allocator` type works and acts like the C++ STL `std::allocator`, and can be substituted in with
little to no problem. Additionally, all of its functions are labeled as `inline`, so some modification may be
necessary if a user of Bank would like to avoid any potential code bloat. The `bank::allocator` is located
within the `bank/allocator.hpp` header.  

Additionally a "smart" pointer is also available under the name of `bank::pointer` (located in the
`bank/pointer.hpp` header). This header works like `std::auto_ptr` (which is to be deprecated in C++0x).
The `bank::pointer` is used in the same way, with one minor change however. If the pointer is declared like
so:

    #include <bank/pointer.hpp>
    bank::pointer<type> my_pointer;

The `bank::pointer` will call the default constructor of `type`, and allocate enough memory via `bank::alloc`
However there is one caveat. The destructor of the `bank::pointer` will call the destructor of type, and then
`bank::free` on its internal pointer. As such, any pointer passed to `bank::pointer` ***must*** be allocated
at some level via `bank::alloc`.

A Warning For Naivete
---------------------

While it may seem like a good idea to override the global `operator new` and `operator delete` functions,
this can cause many serious problems with your applications, as well as bank itself. While bank has tried to
protect the overriding of all internal allocations and calls to new and delete, this cannot be guaranteed.  

As such, a strong warning is given to those who think that overloading operator new and delete will solve all
problems with an application's memory management. This can affect static libraries that overload new and
delete themselves and cause undefined behavior. You have been warned.

[1]: hacking.md
