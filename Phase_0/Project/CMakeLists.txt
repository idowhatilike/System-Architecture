cmake_minimum_required(VERSION 3.16)

project(measure_time CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(measure_contextswitch measure_ctx_switch.cpp)