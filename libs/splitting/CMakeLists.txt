cmake_minimum_required(VERSION 3.14)
project(Splitting)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(splitLib mommurp.cpp label.cpp util.cpp)

add_subdirectory(tests)
