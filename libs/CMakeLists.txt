cmake_minimum_required(VERSION 3.14)
project(libs)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3 -march=native -fconcepts")

add_executable(
    playground
    splitting/playground.cpp
)

add_custom_command(TARGET playground 
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy 
                   ${CMAKE_CURRENT_BINARY_DIR}/playground 
                   ${PROJECT_SOURCE_DIR}/playground)


target_link_libraries(
    playground
    instancelib
    splitLib
)

add_subdirectory(read_instance)
add_subdirectory(splitting)