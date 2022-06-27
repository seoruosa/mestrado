cmake_minimum_required(VERSION 3.10)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fconcepts")

# set the project name
project(TESTAAAAANDO)

include_directories(include)


# add library
add_library(mylib src/read_instance.cpp src/permutation.cpp src/util_print.cpp src/random_generation.cpp src/util_vrp.cpp src/mmurp.cpp src/seeds.cpp)

# add the executable
add_executable(read_instance src/testing.cpp)
add_executable(playground src/playground.cpp)
# add_executable(NSGAII src/nsgaii.cpp)
add_executable(NSGAII_MMURP src/nsgaii_mmurp.cpp)
add_executable(NSGAII_REAL src/nsgaii_real.cpp)

# link libraries to main
target_link_libraries(read_instance PUBLIC mylib)
# target_link_libraries(NSGAII PUBLIC mylib)
target_link_libraries(NSGAII_MMURP PUBLIC mylib)
target_link_libraries(playground PUBLIC mylib)
target_link_libraries(NSGAII_REAL PUBLIC mylib)
