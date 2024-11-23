cmake_minimum_required(VERSION 3.20)

# Project setup
project(poker_engine LANGUAGES CXX)

# Set C++ Standard and enable Conan toolchain
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include Conan-generated toolchain file
include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake)

# Find GTest and other dependencies
find_package(GTest REQUIRED)
find_package(pybind11 REQUIRED)

# Source and include files
file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE HEADERS src/*.hpp src/*.h)

# Add executable
add_executable(poker_engine ${SOURCES} ${HEADERS})

# Link dependencies
target_link_libraries(poker_engine PRIVATE GTest::gtest GTest::gtest_main pybind11::pybind11)
# Add include directories
target_include_directories(poker_engine PRIVATE src src/cards src/game src/players)

# Enable testing
enable_testing()
add_test(NAME poker_engine_tests COMMAND poker_engine)

