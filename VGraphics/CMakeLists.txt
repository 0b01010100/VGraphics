cmake_minimum_required(VERSION 3.15)
# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


project("test")
add_definitions(-D_UNICODE -DUNICODE)
add_subdirectory(VGraphics)
add_subdirectory(VWindow)
add_executable("test" "testMain/testMain.cpp")
target_link_libraries("test" PUBLIC VGraphics )
target_link_libraries("test" PUBLIC VWindow )