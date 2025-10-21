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
set(GLFW_PATH "${CMAKE_CURRENT_SOURCE_DIR}/glfw-3.4")

# Set the path to glslang installation
set(GLSLANG_PATH "E:/VulkanSDK/1.4.321.1")

# Set the path to Bullet Physics installation
set(BULLET_PHYSICS_PATH "C:/bullet3-install")

# Try to find Vulkan, but make it optional
find_package(Vulkan QUIET)

# Try to find GLFW, but make it optional
# First try with the specified path, then try in PATH
find_package(glfw3 QUIET HINTS "${GLFW_PATH}")

# Try to find Bullet Physics, but make it optional
find_package(Bullet QUIET HINTS "${BULLET_PHYSICS_PATH}")

# If not found in the specified path, try to find it in PATH
if (NOT glfw3_FOUND)
    find_package(glfw3 QUIET)
endif()

# If not found in the specified path, try to find Bullet in PATH
if (NOT Bullet_FOUND)
    find_package(Bullet QUIET)
endif()

# Set include directories
set(GLM_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/external/glm")
set(JSON_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/external/json/include")
set(STB_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/external/stb")

# Only set Vulkan libraries if Vulkan was found
if (Vulkan_FOUND)
    set(VULKAN_INCLUDE_DIRS "${VULKAN_SDK_PATH}/Include")
    set(VULKAN_LIBRARIES 
        "${VULKAN_SDK_PATH}/Lib/vulkan-1.lib"
        "${GLSLANG_PATH}/Lib/glslang.lib"
        "${GLSLANG_PATH}/Lib/SPIRV.lib"
        "${GLSLANG_PATH}/Lib/glslang-default-resource-limits.lib"
        "${GLSLANG_PATH}/Lib/SPIRV-Tools.lib"
        "${GLSLANG_PATH}/Lib/SPIRV-Tools-opt.lib"
    )
else()
    message(WARNING "Vulkan not found. Building without Vulkan support.")
endif()

# Set GLFW libraries based on how it was found
if (glfw3_FOUND)
    # If GLFW was found via find_package, use the variables it provides
    if (TARGET glfw)
        # GLFW found as a target (modern CMake)
        set(GLFW_LIBRARIES glfw)
    elseif(DEFINED glfw3_INCLUDE_DIRS AND DEFINED glfw3_LIBRARIES)
        # Use the found GLFW variables (older CMake)
        set(GLFW_INCLUDE_DIRS ${glfw3_INCLUDE_DIRS})
        set(GLFW_LIBRARIES ${glfw3_LIBRARIES})
    else()
        # Fallback to manual path specification
        set(GLFW_INCLUDE_DIRS "${GLFW_PATH}/include")
        set(GLFW_LIBRARIES "${GLFW_PATH}/lib/glfw3.lib")
    endif()
else()
    # Check if GLFW is in the root directory
    if (EXISTS "${GLFW_PATH}/CMakeLists.txt")
        set(GLFW_INCLUDE_DIRS "${GLFW_PATH}/include")
        # We'll need to build GLFW as part of the project
        message(STATUS "Using GLFW from ${GLFW_PATH}")
        # Add GLFW as a subdirectory
        add_subdirectory(glfw-3.4)
        set(GLFW_LIBRARIES glfw)
    else()
        # Fallback to manual path specification
        set(GLFW_INCLUDE_DIRS "${GLFW_PATH}/include")
        set(GLFW_LIBRARIES "${GLFW_PATH}/lib/glfw3.lib")
        message(WARNING "GLFW not found. Building without window support.")
    endif()
endif()

# Set Bullet Physics libraries based on how it was found
if (Bullet_FOUND)
    # If Bullet was found via find_package, use the variables it provides
    set(BULLET_INCLUDE_DIRS ${BULLET_INCLUDE_DIRS})
    set(BULLET_LIBRARIES ${BULLET_LIBRARIES})
else()
    # Check if Bullet Physics is in the external directory
    if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/external/bullet3")
        set(BULLET_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/external/bullet3/src")
        # We'll need to build Bullet Physics as part of the project
        message(STATUS "Using Bullet Physics from external/bullet3")
        # Add Bullet Physics as a subdirectory
        add_subdirectory(external/bullet3)
        set(BULLET_LIBRARIES BulletDynamics BulletCollision LinearMath)
    else()
        # Fallback to manual path specification
        set(BULLET_INCLUDE_DIRS "${BULLET_PHYSICS_PATH}/include")
        set(BULLET_LIBRARIES 
            "${BULLET_PHYSICS_PATH}/lib/BulletDynamics.lib"
            "${BULLET_PHYSICS_PATH}/lib/BulletCollision.lib"
            "${BULLET_PHYSICS_PATH}/lib/LinearMath.lib"
        )
        message(WARNING "Bullet Physics not found. Using manual path specification.")
    endif()
endif()

# Add subdirectories
add_subdirectory(Engine)
add_subdirectory(Game)

# Option to enable/disable audio support
option(ENABLE_AUDIO "Enable audio support" OFF)