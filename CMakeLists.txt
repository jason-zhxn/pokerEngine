cmake_minimum_required(VERSION 3.20)

project(intern LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

find_package(Python3 3.12 COMPONENTS Interpreter Development EXACT REQUIRED)
find_package(pybind11 REQUIRED)

find_package(GTest REQUIRED)
enable_testing()

file(GLOB INTERN_PROJECT_MAIN src/cppsrc/main.cpp)
file(GLOB_RECURSE INTERN_PROJECT_HEADERS src/cppsrc/*.hpp)
file(GLOB_RECURSE INTERN_PROJECT_TESTS src/cppsrc/test/*.cpp)

add_executable(intern_project ${INTERN_PROJECT_MAIN} ${INTERN_PROJECT_HEADERS})

add_executable(intern_tests ${INTERN_PROJECT_TESTS} ${INTERN_PROJECT_HEADERS})
target_include_directories(intern_tests PUBLIC src)

target_link_libraries(intern_project PRIVATE Python3::Python)
target_link_libraries(intern_project PRIVATE pybind11::pybind11)
target_link_libraries(intern_project PRIVATE ${Python3_LIBRARIES})

target_link_libraries(intern_tests PRIVATE Python3::Python)
target_link_libraries(intern_tests PRIVATE pybind11::pybind11)
target_link_libraries(intern_tests PRIVATE ${Python3_LIBRARIES})
target_link_libraries(intern_tests PRIVATE gtest::gtest)

pybind11_add_module(intern src/cppsrc/main.cpp)
install(TARGETS intern DESTINATION intern_project)
