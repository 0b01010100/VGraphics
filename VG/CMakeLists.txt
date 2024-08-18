cmake_minimum_required(VERSION 3.22)

project(VG LANGUAGES C CXX)

# Collect source files for the main project
file(GLOB_RECURSE VG_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
file(GLOB_RECURSE VUtils_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/VUtils/*.c)

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

# Generate the .def file content as a string
set(DEF_FILE_CONTENT "LIBRARY VG\nEXPORTS\n    VG_ctor\n    VG_LoadResource\n    VG_SetResources\n    VG_UnLoadResources\n    VG_Render\n    VG_Clear_Buffer\n    VG_dtor\n")

# Write the .def file to the output directory during configuration
file(GENERATE OUTPUT "${CMAKE_SOURCE_DIR}/bin/VG.def" CONTENT "${DEF_FILE_CONTENT}")

# Link the .def file for MSVC
if(MSVC)
    target_link_options(VG PRIVATE "/DEF:${CMAKE_SOURCE_DIR}/bin/VG.def")
endif()
