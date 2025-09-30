# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.20)

# Set the project name and version
project(SparkyEngineProject VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set the path to the Vulkan SDK
set(VULKAN_SDK_PATH "E:/VulkanSDK/1.4.321.1")

# Set the path to GLFW installation
set(GLFW_PATH "C:/glfw-install")

# Set the path to glslang installation
set(GLSLANG_PATH "E:/VulkanSDK/1.4.321.1")

# Find required packages
find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

# Set include directories
set(VULKAN_INCLUDE_DIRS "${VULKAN_SDK_PATH}/Include")
set(GLFW_INCLUDE_DIRS "${GLFW_PATH}/include")
set(GLM_INCLUDE_DIRS "C:/glm-install/include")
set(JSON_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/external/json/include")
set(STB_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/external/stb")

# Set library directories
set(VULKAN_LIBRARIES 
    "${VULKAN_SDK_PATH}/Lib/vulkan-1.lib"
    "${GLSLANG_PATH}/Lib/glslang.lib"
    "${GLSLANG_PATH}/Lib/SPIRV.lib"
    "${GLSLANG_PATH}/Lib/glslang-default-resource-limits.lib"
    "${GLSLANG_PATH}/Lib/SPIRV-Tools.lib"
    "${GLSLANG_PATH}/Lib/SPIRV-Tools-opt.lib"
)
set(GLFW_LIBRARIES "${GLFW_PATH}/lib/glfw3.lib")

# Add subdirectories
add_subdirectory(Engine)
add_subdirectory(Game)

# Copy assets and shaders to build directory
add_custom_command(TARGET SparkyEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/Engine/assets
    ${CMAKE_CURRENT_BINARY_DIR}/Engine/assets
)

add_custom_command(TARGET SparkyEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/Engine/shaders
    ${CMAKE_CURRENT_BINARY_DIR}/Engine/shaders
)

# Option to enable/disable audio support
option(ENABLE_AUDIO "Enable audio support" OFF)