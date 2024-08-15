cmake_minimum_required(VERSION 3.22)

project(glad LANGUAGES C CXX)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(glad src/glad.c)


set_target_properties(glad PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
  DEBUG_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)