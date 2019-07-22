# Test project - hello world
project (test)

# Test project - simple libzmq server
project (hwserver)
# Test project - simple libzmq client
project (hwclient)
# Simple Reader from the Vicon computer
project (reader)

# preliminaries
cmake_minimum_required (VERSION 2.8)
# C++ 11 command - requires Cmake 3.1 or greater
#set(CMAKE_CXX_STANDARD 11)
# old C++ 11 command
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
# Put exectables in the build directory
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} )

add_subdirectory(examples)
add_subdirectory(src)
