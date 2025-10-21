cmake_minimum_required(VERSION 4.0)
project(cs2_sdk CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(${PROJECT_NAME} SHARED
    cs2/ctx/impl/ctx.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/sdk
)

target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall
    -Wpedantic
    -Wshadow
    -Wextra
)