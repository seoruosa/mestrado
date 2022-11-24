cmake_minimum_required(VERSION 3.14)
project(NSGA)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(NSGA
            nsgaii.cpp
            util.cpp
            ../utils/performance.cpp
            )
target_include_directories(NSGA PUBLIC include)

add_executable(
    play
    playground.cpp
)

target_link_libraries(
    play
    NSGA
    )