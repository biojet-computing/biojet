#----------------------------------------------------------------------
# GNU toolchain file
set(CMAKE_CONFIGURATION_TYPES Debug Profile Release Asan Tsan Ubsan)
set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}"
  CACHE STRING "Build configuration types" FORCE
)

# Force GNU specific compiler toolset
set(CMAKE_C_COMPILER   "gcc" CACHE FILEPATH "C compiler")
set(CMAKE_CXX_COMPILER "g++" CACHE FILEPATH "C++ compiler")

# Ensure C++23 standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)  # No GNU extensions
message(STATUS "Using C compiler   : ${CMAKE_C_COMPILER}")
message(STATUS "Using C++ compiler : ${CMAKE_CXX_COMPILER}")
message(STATUS "Using C Standard   : ${CMAKE_C_STANDARD}")
message(STATUS "Using C++ Standard : ${CMAKE_CXX_STANDARD}")

# Don't search sysroot for build executables.
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# Only look in the sysroot for others (i.e. libraries, includes).
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
# Test compiler without linking.
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

# Boost compilation speed
set(CMAKE_UNITY_BUILD ON)
set(CMAKE_UNITY_BUILD_BATCH_SIZE 16)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION OFF)
set(CMAKE_JOB_POOLS "compile=4;link=2")
set(CMAKE_JOB_POOL_COMPILE "compile")
set(CMAKE_JOB_POOL_LINK    "link")

# Includes compiler settings
include(${CMAKE_CURRENT_LIST_DIR}/overrides-gcc.cmake)

#----------------------------------------------------------------------