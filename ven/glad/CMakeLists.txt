cmake_minimum_required(VERSION 3.22)

project(glad LANGUAGES C CXX)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(glad src/glad.c)
