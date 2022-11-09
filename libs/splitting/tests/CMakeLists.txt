cmake_minimum_required(VERSION 3.14)
project(tests)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Download biblioteca googletest
# include(FetchContent)
# FetchContent_Declare(
#     googletest
#     GIT_REPOSITORY https://github.com/google/googletest.git
#     GIT_TAG release-1.12.1
# )

# FetchContent_MakeAvailable(googletest)

# enable_testing()

add_library(testlib util.cpp)

# add_library(GTest::GTest INTERFACE IMPORTED)
# target_link_libraries(GTest::GTest INTERFACE gtest_main)

# add_executable(${PROJECT_NAME} label_test.cc splitting_test.cc)

# target_link_libraries(${PROJECT_NAME} PRIVATE
#   GTest::GTest
#   splitLib
#   testlib)

# add_test(splitting_gtests ${PROJECT_NAME})
