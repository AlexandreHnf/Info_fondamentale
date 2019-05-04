cmake_minimum_required(VERSION 3.13)  # CMake version check
project(LightUp)               # Create project "simple_example"
set(CMAKE_CXX_STANDARD 14)            # Enable c++14 standard

# Add main.cpp file of project root directory as source file
file(GLOB source_files
        "*.hpp"
        "*.cpp"
        )

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(LightUp ${source_files})