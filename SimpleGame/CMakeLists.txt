cmake_minimum_required(VERSION 3.20)
project(SimpleGame)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Create a simple executable
add_executable(SimpleGame SimpleGame.cpp)

# Set output directory
set_target_properties(SimpleGame PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)