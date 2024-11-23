cmake_minimum_required(VERSION 3.20)

# Project setup
project(poker_engine LANGUAGES CXX)

# Enable compile commands for tools like clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include Conan-generated toolchain
include(${CMAKE_BINARY_DIR}/Release/generators/conan_toolchain.cmake)

# Add include directories
include_directories(src/cards src/game)

# Add main executable
add_executable(poker_engine
    src/main.cpp
    src/cards/Card.cpp
    src/game/HandEvaluation.cpp
)

# Link libraries
target_link_libraries(poker_engine PRIVATE GTest::gtest)

