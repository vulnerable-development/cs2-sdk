cmake_minimum_required(VERSION 4.0)
project(cs2_sdk)

set(CMAKE_CXX_STANDARD 23)

add_library(cs2_sdk SHARED library.cpp)
