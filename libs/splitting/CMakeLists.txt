cmake_minimum_required(VERSION 3.14)
project(Splitting)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3 -march=native -fconcepts")

add_executable(
    playground
    playground.cpp
)

add_custom_command(TARGET playground 
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy 
                   ${CMAKE_CURRENT_BINARY_DIR}/playground 
                   ${PROJECT_SOURCE_DIR}/playground)

add_library(splitLib mommurp.cpp label.cpp util.cpp)

target_link_libraries(
    playground
    splitLib
)


include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)

FetchContent_MakeAvailable(googletest)

add_subdirectory(tests)
