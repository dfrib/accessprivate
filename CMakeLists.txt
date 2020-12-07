cmake_minimum_required(VERSION 3.12.0)

project(accessprivate VERSION 0.1)

# P0692R1 (Access Checking on Specializations)
# Implemented for C++20, but as mentioned in the proposal,
# most compilers accept waiving of private access rules
# in explicit specializations even for earlier C++ versions.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE include/)

# /tests
enable_testing()
add_subdirectory(tests)
