cmake_minimum_required(VERSION 3.20)

project(poker_engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Configuring with Debug symbols")
    add_compile_options(-g) 
    add_link_options(-g)
endif()

include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake)

find_package(GTest REQUIRED)
find_package(pybind11 REQUIRED)

message("GTest include directories: ${GTest_INCLUDE_DIRS}")
message("GTest library directories: ${GTest_LIB_DIRS}")

file(GLOB_RECURSE SOURCES src/*.cpp)

add_executable(poker_engine ${SOURCES})
target_include_directories(poker_engine PRIVATE src src/cards src/game src/players)
target_link_libraries(poker_engine PRIVATE pybind11::pybind11)

set_target_properties(poker_engine PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB_RECURSE TEST_SOURCES tests/*.cpp)
file(GLOB_RECURSE IMPLEMENTATION_SOURCES src/cards/*.cpp src/game/*.cpp src/players/*.cpp)

if(TEST_SOURCES)
    add_executable(poker_engine_tests ${TEST_SOURCES} ${IMPLEMENTATION_SOURCES})
    
    target_include_directories(poker_engine_tests PRIVATE 
        src 
        src/cards 
        src/game 
        src/players 
        ${GTest_INCLUDE_DIRS}
    )
    
    target_link_directories(poker_engine_tests PRIVATE ${GTest_LIB_DIRS})
    target_link_libraries(poker_engine_tests PRIVATE 
        GTest::gtest 
        GTest::gtest_main 
        pybind11::pybind11 
        pthread
    )
    
    set_target_properties(poker_engine_tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    
    enable_testing()
    add_test(NAME poker_engine_tests COMMAND poker_engine_tests)
else()
    message(WARNING "No test files found in the 'tests/' directory.")
endif()

