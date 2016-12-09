LOOT API C Wrapper
==================

A wrapper library for LOOT's C++ API that provides it as a C API.

## Build Instructions

The library's build system uses [CMake](https://cmake.org/). Most of LOOT's C++ dependencies are managed by CMake, but the following must be obtained manually:

* [Boost](http://www.boost.org/) v1.55+

To build the wrapper, run CMake, and build the generated solution file. Only Windows support has been tested, though Linux builds should also be possible. A `loot_c_api.dll` is produced: this statically links the C++ API, so only one DLL is required.
