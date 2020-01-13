cmake_minimum_required(VERSION 3.10.2)

#give the project a name
project(minesweeper)

set(CMAKE_CXX_FLAGS "-g -Wall")

find_package(OpenGL REQUIRED)
#Bring the headers into the project
include_directories(include)
include_directories(libs/src/)

link_directories(libs/src)

#add all cpp files in the src directory to the project
file(GLOB SOURCES "src/*.cpp")

#name of the executable
add_executable(minesweeper ${SOURCES})
target_link_libraries(minesweeper ${OPENGL_gl_LIBRARY} libglfw3.a -lpthread -lX11 ${CMAKE_DL_LIBS})
