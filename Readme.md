Bank
====

Overview
--------

Bank is a "dumb" chunk-based concurrent/deferred memory manager/allocator and is released under a 3-Clause BSD
License. Bank uses CMake 2.8 to build, and is compiled and tested on OS X, FreeBSD, Linux and Windows.  

Bank has no concept of references (cyclic or otherwise), nor does it understand an unreachable object.
It only knows how to allocate memory, and to later reuse that memory once it is no longer needed.

Due to my getting all my words mixed up, I ended up calling this a garbage collector for a while. I apologize.

Documentation
-------------

Documentation on Bank is available within the `docs` directory. Using bank is detailed in `bank.md`.
Information on porting is detailed in `porting.md`. Possible performance gains and improvements (for each
platform) are detailed in `hacking.md`.  

In addition, `hacking.md` goes into detail on some of the choices taken for code expression, as well as why
certain optimizations were avoided. Of course, users of Bank are free to disregard these choices and modify
it to their needs.
