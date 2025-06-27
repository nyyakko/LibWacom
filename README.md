# enum library.

# installation

you may install it with [CPM](https://github.com/cpm-cmake/CPM.cmake) or install directly into your system with the following:

* ``py install.py``

and then include it with cmake into your project

```cmake
cmake_minimum_required_version(VERSION 3.25)

project(CoolProject LANGUAGES CXX)

find_package(LibWacom CONFIG REQUIRED)
add_executable(CoolProject source.cpp)
target_link_libraries(CoolProject PRIVATE LibWacom::LibWacom)
```

# examples

```c++
#include <libwacom/Device.hpp>

#include <iostream>

int main()
{
    auto devices = libwacom::get_available_devices();

    if (!devices.has_value())
    {
        std::cerr << "No devices were found!";
        return EXIT_FAILURE;
    }

    for (auto const& device : devices.value())
    {
        std::cout
            << "device name: " << device.name << '\n'
            << "device id: " << device.id << '\n'
            << "device kind: " << device.kind.to_string() << '\n';
    }
}
```

i recommend you to simply explore the code and see what you can do with it. seriously. do it.
