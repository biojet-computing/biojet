cmake_minimum_required(VERSION 3.22)
project(carbio)

include("${CMAKE_CURRENT_LIST_DIR}/cmake/Init.cmake")

add_subdirectory(source)

option(BIOJET_BUILD_EXAMPLES "Build examples" OFF)
if(BIOJET_BUILD_EXAMPLES)
add_subdirectory(examples)
endif()

option(BIOJET_BUILD_TESTS "Build unit tests" OFF)
if(BIOJET_BUILD_TESTS)
enable_testing()
add_subdirectory(tests/unit)
endif()

option(BIOJET_BUILD_INTEG "Build integration tests" OFF)
if(BIOJET_BUILD_INTEG)
enable_testing()
add_subdirectory(tests/integration)
endif()

option(BIOJET_BUILD_BENCHMARKS "Build performance tests" OFF)
if(BIOJET_BUILD_BENCHMARKS)
enable_testing()
add_subdirectory(tests/performance)
endif()