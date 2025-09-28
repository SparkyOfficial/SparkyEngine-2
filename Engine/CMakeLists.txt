cmake_minimum_required(VERSION 3.20)
project(SparkyEngine)

# Create the engine library
add_library(SparkyEngine STATIC
    src/VulkanRenderer.cpp
    src/WindowManager.cpp
    src/GameEntity.cpp
    src/InputManager.cpp
    src/Camera.cpp
    src/SparkyEngine.cpp
    src/ShaderUtils.cpp
    src/Mesh.cpp
    src/MeshRenderer.cpp
    src/AssetManager.cpp
    src/Component.cpp
    src/GameObject.cpp
    src/RenderComponent.cpp
    src/PhysicsComponent.cpp
    src/CollisionSystem.cpp
    src/Timer.cpp
    src/Logger.cpp
    src/FileUtils.cpp
    src/Config.cpp
    src/ResourceManager.cpp
    src/EventSystem.cpp
    src/StateMachine.cpp
    src/ExampleState.cpp
    src/Profiler.cpp
)

# Include directories
target_include_directories(SparkyEngine PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${Vulkan_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(SparkyEngine 
    ${Vulkan_LIBRARIES}
    glfw
)

# Compiler definitions
target_compile_definitions(SparkyEngine PRIVATE VK_PROTOTYPES)