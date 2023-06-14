cmake_minimum_required(VERSION 3.24)
project(CLion_cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CLion_cpp_SOURCE_DIR}/cmake")
find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})

add_executable(CLion_cpp white_window.cpp)
target_link_libraries(CLion_cpp ${SDL2_LIBRARY})
