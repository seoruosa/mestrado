cmake_minimum_required(VERSION 3.14)
project(libs)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3 -march=native -fconcepts")

add_executable(
    playground
    splitting/playground.cpp
)

add_executable(
    NSGAII_MMURP 
    nsga_mmurp/nsgaii_mmurp.cpp
    )

target_link_libraries(
    NSGAII_MMURP PUBLIC 
    NSGA_MMURPlib 
    splitLib
    utilslib
    )

add_custom_command(TARGET NSGAII_MMURP 
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy 
                   ${CMAKE_CURRENT_BINARY_DIR}/NSGAII_MMURP 
                   ${PROJECT_SOURCE_DIR}/NSGAII_MMURP)


target_link_libraries(
    playground
    instancelib
    splitLib
)

add_subdirectory(read_instance)
add_subdirectory(splitting)
add_subdirectory(nsga_mmurp)
add_subdirectory(utils)