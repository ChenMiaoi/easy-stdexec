cmake_minimum_required(VERSION 3.8.0)
project(easy-stdexec VERSION 0.1.0 LANGUAGES C CXX)

add_subdirectory(${CMAKE_SOURCE_DIR}/deps/stdexec)
include_directories(${CMAKE_SOURCE_DIR}/deps/stdexec/include)

add_subdirectory(${CMAKE_SOURCE_DIR}/usage)
