cmake_minimum_required(VERSION 3.14)
project(read_instance)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3 -march=native -fconcepts")

add_executable(
    play_instance
    playground.cpp
)

add_custom_command(TARGET play_instance 
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy 
                   ${CMAKE_CURRENT_BINARY_DIR}/play_instance 
                   ${PROJECT_SOURCE_DIR}/play_instance)

add_library(instancelib mmurp.cpp util.cpp)

target_link_libraries(
    play_instance
    instancelib
)
