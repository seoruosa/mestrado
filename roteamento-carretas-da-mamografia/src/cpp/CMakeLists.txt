cmake_minimum_required(VERSION 3.10)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fconcepts")

# set the project name
project(TESTAAAAANDO)

include_directories(include)


# add library
add_library(mylib src/read_instance.cpp src/permutation.cpp src/util_print.cpp src/random_generation.cpp)

# add the executable
add_executable(read_instance src/testing.cpp)
add_executable(playground src/playground.cpp)
add_executable(NSGAII src/nsgaii.cpp)

# link libraries to main
target_link_libraries(read_instance PUBLIC mylib)
target_link_libraries(NSGAII PUBLIC mylib)
