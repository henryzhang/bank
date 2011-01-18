Bank
====

Overview
--------

Bank is a "dumb" chunk-based concurrent/deferred memory manager/allocator and is released under a 3-Clause BSD
License. Bank uses CMake 2.8 to build, and is compiled and tested on OS X, FreeBSD, Linux and Windows.  

Bank has no concept of references (cyclic or otherwise), nor does it understand an unreachable object.
It only knows how to allocate memory, and to later reuse that memory once it is no longer needed.

Currently Bank does not perform better in terms of speed compared to the C++ `std::allocator`, however it has
a less likely chance of creating fragmented memory over a large period of time.

Documentation
-------------

Documentation on Bank is available within the `docs` directory. Using bank is detailed in `bank.md`.
Information on porting is detailed in `porting.md`. Possible performance gains and improvements (for each
platform) are detailed in `hacking.md`.  

In addition, `hacking.md` goes into detail on some of the choices taken for code expression, as well as why
certain optimizations were avoided. Of course, users of Bank are free to disregard these choices and modify
it to their needs.
