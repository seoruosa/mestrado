cmake_minimum_required(VERSION 3.14)
project(NSGA_MMURP)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3 -march=native -fconcepts")

add_library(NSGA_MMURPlib
            permutation.cpp 
            random_generation.cpp 
            util_print.cpp
            util_vrp.cpp
            nsgaii_mmurp.cpp
            )

target_link_libraries(NSGA_MMURPlib splitLib instancelib)