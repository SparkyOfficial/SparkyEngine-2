cmake_minimum_required(VERSION 3.20)
project(SparkyEngine)

# Create the engine library
add_library(SparkyEngine STATIC
    src/Animation.cpp
    src/AnimationController.cpp
    src/AssetManager.cpp
    src/AudioEngine.cpp
    src/BehaviorTree.cpp
    src/BehaviorTreeExample.cpp
    src/Button.cpp
    src/Camera.cpp
    src/CharacterController.cpp
    src/CollisionSystem.cpp
    src/Component.cpp
    src/Config.cpp
    src/DirectionalLight.cpp
    src/EngineDemo.cpp
    src/EventSystem.cpp
    src/ExampleState.cpp
    src/FileUtils.cpp
    src/GUIElement.cpp
    src/GUIManager.cpp
    src/GameEntity.cpp
    src/GameObject.cpp
    src/InputManager.cpp
    src/Inventory.cpp
    src/Light.cpp
    src/LightManager.cpp
    src/Logger.cpp
    src/Material.cpp
    src/Mesh.cpp
    src/MeshRenderer.cpp
    src/ParticleSystem.cpp
    src/PhysicsComponent.cpp
    src/PhysicsWorld.cpp
    src/PointLight.cpp
    src/Profiler.cpp
    src/Quest.cpp
    src/QuestManager.cpp
    src/RenderComponent.cpp
    src/RenderSystem.cpp
    src/ResourceManager.cpp
    src/RigidBodyComponent.cpp
    src/ScriptComponent.cpp
    src/ShaderCompiler.cpp
    src/ShaderProgram.cpp
    src/ShaderUtils.cpp
    src/SparkyEngine.cpp
    src/SpotLight.cpp
    src/StateMachine.cpp
    src/Texture.cpp
    src/Timer.cpp
    src/VulkanRenderer.cpp
    src/WindowManager.cpp
    src/HealthComponent.cpp
    src/AIComponent.cpp
)

# Include directories
target_include_directories(SparkyEngine PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${VULKAN_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIRS}
    ${JSON_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(SparkyEngine 
    ${VULKAN_LIBRARIES}
    ${GLFW_LIBRARIES}
    ${glslang_LIBRARIES}
)

# Compiler definitions
target_compile_definitions(SparkyEngine PRIVATE 
    PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

# Add STB image implementation
target_compile_definitions(SparkyEngine PRIVATE 
    STB_IMAGE_IMPLEMENTATION
)

# Create physics test executable
add_executable(physics_test src/physics_test.cpp)
target_link_libraries(physics_test SparkyEngine 
    ${VULKAN_LIBRARIES}
    ${GLFW_LIBRARIES}
    ${glslang_LIBRARIES}
)
target_include_directories(physics_test PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Create animation test executable
add_executable(animation_test src/animation_test.cpp)
target_link_libraries(animation_test SparkyEngine 
    ${VULKAN_LIBRARIES}
    ${GLFW_LIBRARIES}
    ${glslang_LIBRARIES}
)
target_include_directories(animation_test PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Create particle test executable
add_executable(particle_test src/particle_test.cpp)
target_link_libraries(particle_test SparkyEngine 
    ${VULKAN_LIBRARIES}
    ${GLFW_LIBRARIES}
    ${glslang_LIBRARIES}
)
target_include_directories(particle_test PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Create texture test executable
add_executable(texture_test src/texture_test.cpp)
target_link_libraries(texture_test SparkyEngine 
    ${VULKAN_LIBRARIES}
    ${GLFW_LIBRARIES}
    ${glslang_LIBRARIES}
)
target_include_directories(texture_test PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Create STB image test executable
add_executable(stb_image_test src/stb_image_test.cpp)
target_link_libraries(stb_image_test SparkyEngine 
    ${VULKAN_LIBRARIES}
    ${GLFW_LIBRARIES}
    ${glslang_LIBRARIES}
)
target_include_directories(stb_image_test PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)