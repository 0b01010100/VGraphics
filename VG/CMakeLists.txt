cmake_minimum_required(VERSION 3.22)

project(VG LANGUAGES C CXX)

# Set the path for third-party dependencies
set(ThirdPartyFolder ${CMAKE_SOURCE_DIR}/ven)

# Add plugs subdirectory
add_subdirectory(plugs/OGL_P)

# Collect source files for the main project
file(GLOB_RECURSE VG_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
file(GLOB_RECURSE VUtils_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/VUtils/*.c)

# Define the main project library
add_library(VG ${VG_SOURCES} ${VUtils_SOURCES})

# Specify include directories for the main project
target_include_directories(VG PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/VUtils
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/plugs/OGL_P
)

# Link the plugin to the main project
target_link_libraries(VG PRIVATE OGL_P)

# Set output directories for different target types
set_target_properties(VG PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  DEBUG_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
