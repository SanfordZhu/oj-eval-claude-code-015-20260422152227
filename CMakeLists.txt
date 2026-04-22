cmake_minimum_required(VERSION 3.10)
project(problem_015_file_storage)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

add_compile_options(-O2 -pipe)

add_executable(code src/main.cpp)
