cmake_minimum_required(VERSION 2.8)
project(b64_test)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")
include_directories("include/")
add_subdirectory("src/b64")
set(SOURCE_EXE main.cpp)

add_executable(b64_test ${SOURCE_EXE})
link_directories(${main_BINARY_DIR})

target_link_libraries(b64_test b64)
