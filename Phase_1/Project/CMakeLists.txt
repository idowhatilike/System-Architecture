cmake_minimum_required(VERSION 3.20)

project(Phase1_Labs LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -Wpedantic)

#add_executable(run src/lab1_fork_exec.cpp)
#add_executable(run src/lab1_pipe_backpressure.cpp)
#add_executable(run src/lab1_fd_leak.cpp)
#add_executable(run src/pipeline_runner.cpp)

add_executable(run
    src/Mini-shell/mini_shell.cpp
    src/Mini-shell/Lexer/Lexer.cpp
)
target_include_directories(run PRIVATE src/Mini-shell)