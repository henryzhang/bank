Porting Bank to Other Platforms
===============================

This document discusses the possible issues or changes that must be made to port bank to another platform.

Threading
---------

Bank is concurrent by design, and requires the ability to create a thread at some level of execution. In the
event that this is not possible, the removal of the thread from the `bank::detail::collector` may be necessary
as the deferrment of a collection cycle will not be concurrent, but rather more like a stop the world scenario.  

For bank to utilize a threading API on a given platform, it must support the following:

 * Ability to adjust the thread stack size
 * Ability to join the thread
 * Ability to yield the current thread's time slice to the operating system.

Memory Allocation
-----------------

Bank currently uses `std::calloc` for nearly all of its allocations, and this was done as a possible speed
improvement. Some platforms may get a better performance from a platform specific heap allocation function,
or from the `std::malloc` function. Milage may vary, so some form of testing may be required to get the best
performance out of the port.
