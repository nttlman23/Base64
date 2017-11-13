cmake_minimum_required(VERSION 2.8)
project(b64)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories("../../include")

set(SOURCE_LIB base64.cpp)

add_library(b64 SHARED ${SOURCE_LIB})

#target_link_libraries(clientmap)

