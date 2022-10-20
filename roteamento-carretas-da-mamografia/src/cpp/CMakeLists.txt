cmake_minimum_required(VERSION 3.10)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3 -march=native -fconcepts")

# set the project name
project(MMURP)

include_directories(include)


# add library
add_library(mylib 
            src/read_instance.cpp 
            src/permutation.cpp 
            src/util_print.cpp 
            src/random_generation.cpp 
            src/util_vrp.cpp 
            src/mmurp.cpp 
            src/seeds.cpp
            src/mommurp.cpp
            src/label.cpp
            src/nsgaii_mmurp.cpp
            )

add_executable(playground src/playground.cpp)
target_link_libraries(playground PUBLIC mylib)

add_executable(mmurp_post_process src/playground.cpp)
target_link_libraries(mmurp_post_process PUBLIC mylib)

# add_executable(NSGAII_MMURP src/nsgaii_mmurp.cpp)
# target_link_libraries(NSGAII_MMURP PUBLIC mylib)

# add_executable(NSGAII_REAL src/nsgaii_real.cpp)
# target_link_libraries(NSGAII_REAL PUBLIC mylib)

# add_executable(NSGAII src/nsgaii.cpp)
# target_link_libraries(NSGAII PUBLIC mylib)
