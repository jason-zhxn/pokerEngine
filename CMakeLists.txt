cmake_minimum_required(VERSION 3.20)

# Project setup
project(poker_engine LANGUAGES CXX)

# Set C++ Standard and enable Conan toolchain
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include Conan-generated toolchain file
include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake)

# Find dependencies
find_package(GTest REQUIRED)
find_package(pybind11 REQUIRED)

# Source and include files for main application
file(GLOB_RECURSE SOURCES src/*.cpp)

# Add main application
add_executable(poker_engine ${SOURCES})
target_include_directories(poker_engine PRIVATE src src/cards src/game src/players)
target_link_libraries(poker_engine PRIVATE pybind11::pybind11)

# Place the main executable in the `bin/` directory
set_target_properties(poker_engine PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Add test executable
file(GLOB_RECURSE TEST_SOURCES tests/*.cpp)  # Assuming tests are in a separate `tests/` directory
file(GLOB_RECURSE IMPLEMENTATION_SOURCES src/cards/*.cpp src/game/*.cpp src/players/*.cpp)  # Include source files for tests

if(TEST_SOURCES)
    add_executable(poker_engine_tests ${TEST_SOURCES} ${IMPLEMENTATION_SOURCES})
    target_include_directories(poker_engine_tests PRIVATE src src/cards src/game src/players)
    target_link_libraries(poker_engine_tests PRIVATE GTest::gtest GTest::gtest_main pybind11::pybind11)
    
    # Place the test executable in the `bin/` directory
    set_target_properties(poker_engine_tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

    # Enable testing and register tests
    enable_testing()
    add_test(NAME poker_engine_tests COMMAND poker_engine_tests)
else()
    message(WARNING "No test files found in the 'tests/' directory.")
endif()
