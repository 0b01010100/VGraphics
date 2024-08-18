cmake_minimum_required(VERSION 3.22)

project(VG LANGUAGES C CXX)

# Collect source files for the main project
file(GLOB_RECURSE VG_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
file(GLOB_RECURSE VUtils_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/VUtils/*.c)

# Export our VG_ functions to a .dll file
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Define the main project library as SHARED
add_library(VG SHARED ${VG_SOURCES} ${VUtils_SOURCES})

# Specify include directories for the main project
target_include_directories(VG PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/VUtils
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

# Set output directories for different target types
set_target_properties(VG PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)
