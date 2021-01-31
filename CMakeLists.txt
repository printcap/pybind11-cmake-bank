project(example LANGUAGES CXX)
cmake_minimum_required(VERSION 3.10)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake/modules)
message("module path: ${CMAKE_MODULE_PATH}")

add_library(example SHARED src/example.cpp src/bank.cpp)
target_include_directories(example PUBLIC include)
set_target_properties(example PROPERTIES CXX_STANDARD 17 CXX_EXTENSIONS OFF)

include(CTest)
include(Catch)
enable_testing()

add_library(catch2 INTERFACE)
target_include_directories(catch2 INTERFACE test/)

add_executable(test_all tests/test_gcd.cpp tests/test_bank.cpp)
set_target_properties(test_all PROPERTIES CXX_STANDARD 17 CXX_EXTENSIONS OFF)
target_link_libraries(test_all PRIVATE example catch2)

catch_discover_tests(test_all)

