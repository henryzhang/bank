Bank
====

Overview
--------

Bank is a "dumb" chunk-based concurrent/deferred garbage collector and is released under a 3-Clause BSD
License. Bank uses CMake 2.8 to build, and is compiled and tested on OS X, FreeBSD, Linux and Windows.

Documentation
-------------

Documentation on Bank is available within the `docs` directory. Using bank is detailed in `bank.md`.
Information on porting is detailed in `porting.md`. Possible performance gains and improvements (for each
platform) are detailed in `hacking.md`.  

In addition, `hackind.md` goes into detail on some of the choices taken for code expression, as well as why
certain optimizations were avoided. Of course, users of Bank are free to disregard these choices and modify
it to their needs.
