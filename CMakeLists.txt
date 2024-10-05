# Root CMakeLists.txt
cmake_minimum_required(VERSION 3.10)

# Project name and C++ standard
project(own_git_project VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Option to build tests
option(BUILD_TESTS "Enable building tests" OFF)

# Include the main build configuration
add_subdirectory(src)

# Conditionally include the test build configuration if BUILD_TESTS is ON
if (BUILD_TESTS)
    add_subdirectory(tests)
endif()
