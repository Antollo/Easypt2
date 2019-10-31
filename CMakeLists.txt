cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(Easypt2)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/O2 /W4 /MT /EHa /utf-8)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-O3 -static-libgcc -static-libstdc++)
    link_libraries(stdc++fs)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(-O3 -Wno-narrowing)
    link_libraries(libc++experimental)
endif()

enable_testing()
add_subdirectory(src)
add_subdirectory(tests)