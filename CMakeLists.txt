cmake_minimum_required(VERSION 3.20)
project(SparkyEngine2)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add Vulkan support
set(VULKAN_SDK "E:/VulkanSDK/1.4.321.1")
set(VULKAN_INCLUDE_DIRS "${VULKAN_SDK}/Include")
set(VULKAN_LIBRARIES "${VULKAN_SDK}/Lib/vulkan-1.lib")

# Add GLFW support
set(GLFW_DIR "C:/glfw-install")
set(GLFW_INCLUDE_DIRS "${GLFW_DIR}/include")
find_library(GLFW_LIBRARIES NAMES glfw3 glfw PATHS "${GLFW_DIR}/lib")
if(NOT GLFW_LIBRARIES)
    message(FATAL_ERROR "GLFW library not found in ${GLFW_DIR}/lib")
endif()
message(STATUS "Found GLFW library: ${GLFW_LIBRARIES}")

# Add GLM support
set(GLM_INCLUDE_DIRS "C:/glm-install/include")

# Add JSON library support (nlohmann/json)
set(JSON_INCLUDE_DIRS "C:/json-install/include")

# Try to find glslang
find_package(glslang QUIET)

if(NOT glslang_FOUND)
    # If not found, try to find it in common locations
    find_library(GLSLANG_LIBRARY glslang PATHS ${VULKAN_SDK}/Lib)
    find_library(SPIRV_LIBRARY SPIRV PATHS ${VULKAN_SDK}/Lib)
    find_library(GLSLANG_DEFAULT_RESOURCE_LIMITS_LIBRARY glslang-default-resource-limits PATHS ${VULKAN_SDK}/Lib)
    find_library(SPIRV_TOOLS_LIBRARY SPIRV-Tools PATHS ${VULKAN_SDK}/Lib)
    find_library(SPIRV_TOOLS_OPT_LIBRARY SPIRV-Tools-opt PATHS ${VULKAN_SDK}/Lib)
    
    if(GLSLANG_LIBRARY AND SPIRV_LIBRARY AND GLSLANG_DEFAULT_RESOURCE_LIMITS_LIBRARY AND SPIRV_TOOLS_LIBRARY AND SPIRV_TOOLS_OPT_LIBRARY)
        set(glslang_LIBRARIES ${GLSLANG_LIBRARY} ${SPIRV_LIBRARY} ${GLSLANG_DEFAULT_RESOURCE_LIMITS_LIBRARY} ${SPIRV_TOOLS_LIBRARY} ${SPIRV_TOOLS_OPT_LIBRARY})
        message(STATUS "Found glslang libraries: ${glslang_LIBRARIES}")
    else()
        message(FATAL_ERROR "glslang libraries not found. Please install glslang development libraries.")
    endif()
endif()

# Option to enable/disable audio support
option(ENABLE_AUDIO "Enable audio support" OFF)

# Engine library
add_subdirectory(Engine)

# Game executable
add_subdirectory(Game)

# Test executable (commented out since test_engine.cpp is missing)
# add_executable(test_engine test_engine.cpp)
# target_link_libraries(test_engine SparkyEngine)
# target_include_directories(test_engine PRIVATE 
#     ${CMAKE_CURRENT_SOURCE_DIR}/Engine/include
#     ${VULKAN_INCLUDE_DIRS}
#     ${GLFW_INCLUDE_DIRS}
#     ${GLM_INCLUDE_DIRS}
#     ${JSON_INCLUDE_DIRS}
# )