cmake_minimum_required(VERSION 3.10)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# set the project name
project(TESTAAAAANDO)

include_directories(include)

# add library
add_library(mylib src/read_instance.cpp src/util.cpp)

# add the executable
add_executable(read_instance src/testing.cpp)

# link libraries to main
target_link_libraries(read_instance PUBLIC mylib)