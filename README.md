LOOT API C Wrapper
==================

A wrapper library for LOOT's C++ API that provides it as a C API.

This library is at a very early stage of development and wraps an API that is itself still a work-in-progress.

## Build Instructions

The library's build system uses [CMake](https://cmake.org/). Most of LOOT's C++ dependencies are managed by CMake, but the following must be obtained manually:

* [Boost](http://www.boost.org/) v1.55+

To build the wrapper, run CMake, and build the generated solution file. Only Windows support has been tested, though Linux builds should also be possible. A `loot_c_api.dll` is produced: this statically links the C++ API, so only one DLL is required.

**Note:** The module is currently built against revision [5982136](https://github.com/loot/loot/tree/dc3a93ecf331465b17a28ac39e6c17370438331b) of the API, which is post-0.9.2 and pre-0.10.0, but uses the v0.10 metadata syntax (as specified at that revision, in case there are further changes before the v0.10 release).
