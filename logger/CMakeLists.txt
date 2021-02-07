# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.0)

# Project's name
project(rhlogger)
# Set the output folder where your program will be created
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# C++ Flags
set(GCC_DEBUG_FLAGS "-g -ggdb")
set(GCC_LINK_FLAGS "-L/usr/local/lib")
set(CMAKE_C_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_DEBUG_FLAGS} ${GCC_LINK_FLAGS}")

# eqresolver particular directories
set(RHLOGGER_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
#set(RHLOGGER_UNIT_TESTS_DIR ${PROJECT_SOURCE_DIR}/test)
  
# The following folder will be included
include_directories("${RHLOGGER_SOURCE_DIR}")

# The following subdirectories are included in the build path
add_subdirectory("${RHLOGGER_SOURCE_DIR}")
#add_subdirectory("${RHLOGGER_UNIT_TESTS_DIR}")
