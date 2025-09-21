# LLVM/Clang toolchain file
set(CMAKE_CONFIGURATION_TYPES Debug Profile Release Asan Tsan Ubsan Rtsan)
set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING "Build configuration types" FORCE)

# Force Clang specific compiler toolset
set(CMAKE_C_COMPILER   "clang" CACHE FILEPATH "C compiler")
set(CMAKE_CXX_COMPILER "clang++" CACHE FILEPATH "C++ compiler")

# Ensure C++23 standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
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

#
# Figure out the CPU count and parallel execution flags.  For a build of any
# given sub-project you can add "-- -j <n>" to the end of the build command to
# perform parallel builds.  This is an attempt to get the superbuild
# orchestration script to properly pass the parallel execution flags to all
# sub-builds.
#
include(ProcessorCount)
ProcessorCount(CPU_COUNT)
if(NOT CPU_COUNT EQUAL 0)
  message(STATUS "This system has ${CPU_COUNT} CPU(s).")
  if(CMAKE_GENERATOR STREQUAL "Ninja")
    # Try not to kill older Raspberry Pis
    execute_process(COMMAND cat /proc/device-tree/model OUTPUT_VARIABLE MODEL ERROR_QUIET)
    if(MODEL MATCHES "Raspberry Pi (.)")
      if(${CMAKE_MATCH_1} LESS 4)
        set(CPU_COUNT 2)
      endif()
    endif()
    # Building with ninja, 'cmake --build build -j <n>' works properly for cmake
    # based projects, but not for make based projects.  Use this variable to
    # pass the cpu count into make based projects.
    set(MAKE_JFLAG -j${CPU_COUNT})
    # Tell ninja that it can execute as many parallel compile jobs as there are
    # CPUs, but it can only execute one link job at a time. This is probably
    # only necessary for small memory systems, but set it for all systems. Make
    # this a cache variable so that the user can override it from the command
    # line.
    set(CMAKE_JOB_POOLS "compile=${CPU_COUNT};link=1")
    set(CMAKE_JOB_POOL_COMPILE "compile")
    set(CMAKE_JOB_POOL_LINK    "link")
    # Tell ninja that it can execute as many parallel jobs as there are CPUs.
    set(CTEST_BUILD_FLAGS -j${CPU_COUNT})
    set(ctest_test_args ${ctest_test_args} PARALLEL_LEVEL ${CPU_COUNT})
  else()
    # Building with unix makefiles, 'cmake --build build -j <n>' works properly.
  endif()
endif()

# Includes compiler settings
include(${CMAKE_CURRENT_LIST_DIR}/overrides-clang.cmake)
