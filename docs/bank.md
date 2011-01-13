Using Bank
==========

This document outlines the following in relation to the bank garbage collector:

 * Basic Usage
 * Advanced Usage
 * 


Usage
-----

The simplest way to use Bank is to inherit from the `bank::object` type. These types are automatically
allocated by Bank, and utilize the underlying memory pool, by overloading the object's `operator new` method.
If a type must be monitored, but cannot be allocated by the pool, use of the `bank::pointer<T>` type should be
considered. This works much like the deprecated `std::auto_ptr` from the C++ standard. Additionally, objects
can be allocated by using the `bank/new.hpp` header, which contains an overload for both `operator new` and
`operator delete`  

Bank also supplies allocators for the different C++ STL containers. There are previously made typedefs within
the `bank/stl` headers. (i.e., `bank/stl/vector.hpp`)  

Currently due to restrictions with the C++ standard, these types are used as `bank::vector<T>::type`. Once
the template aliases feature has been implemented on all platforms that Bank targets, these will be changed
so that they may be used as `bank::vector<T>`.
