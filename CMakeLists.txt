cmake_minimum_required(VERSION 3.22)

project(VGraphics VERSION 0.1.0 LANGUAGES C CXX)

# Include current source directory for header files
include_directories(${CMAKE_CURRENT_SOURCE_DIR})


# Add VG subdirectory
add_subdirectory(VG)

# Define the executable
add_executable(VGraphics main.cpp)

# Link the VG library to the VGraphics executable
target_link_libraries(VGraphics PUBLIC VG)

add_subdirectory(Plugins/OGL_P)

target_link_libraries(VGraphics PUBLIC OGL_P)

# Set target properties for the executable
set_target_properties(VGraphics PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
  DEBUG_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)
