cmake_minimum_required(VERSION 3.20)
project(benchmark LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(benchmark lab/benchmark.cpp)

target_include_directories(benchmark PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lab)

target_compile_options(benchmark PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Wconversion
    -Wshadow
    -Werror
    -O0
    -g
    -fsanitize=address,undefined
)

target_link_options(benchmark PRIVATE
    -fsanitize=address,undefined
)





