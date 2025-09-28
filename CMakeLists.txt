cmake_minimum_required(VERSION 3.20)
project(SparkyEngine2)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add Vulkan support
find_package(Vulkan REQUIRED)

# Add GLFW support
find_package(glfw3 REQUIRED)

# Add GLM support
find_package(glm REQUIRED)

# Engine library
add_subdirectory(Engine)

# Game executable
add_subdirectory(Game)