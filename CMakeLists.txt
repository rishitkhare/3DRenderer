cmake_minimum_required(VERSION 3.24)
project(3dRenderer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${3dRenderer_SOURCE_DIR}/cmake")
find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})

add_executable(3dRenderer SDL_loop.cpp VertexArrayObject.cpp VertexArrayObject.h main.cpp render.h render.cpp SDL_loop.h linalg.h linalg.cpp)
target_link_libraries(3dRenderer ${SDL2_LIBRARY})
