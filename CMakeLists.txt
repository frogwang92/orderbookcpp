cmake_minimum_required(VERSION 3.7)
project(orderbookcpp)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES OrderBookMgmt.cpp OrderBookMgmt.hpp Global.hpp)
add_executable(orderbookcpp ${SOURCE_FILES} main.cpp)

add_executable(orderbooktest ${SOURCE_FILES} tests.cpp)