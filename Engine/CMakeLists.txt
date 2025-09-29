cmake_minimum_required(VERSION 3.20)
project(SparkyEngine)

# Create the engine library
set(SPARKY_ENGINE_SOURCES
    src/Animation.cpp
    src/AnimationComponent.cpp
    src/AnimationController.cpp
    src/AssetManager.cpp
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
    src/ParticleComponent.cpp
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

# Add audio components only if audio is enabled
if(ENABLE_AUDIO)
    list(APPEND SPARKY_ENGINE_SOURCES
        src/AudioEngine.cpp
        src/AudioComponent.cpp
    )
endif()

# Create the engine library
add_library(SparkyEngine STATIC ${SPARKY_ENGINE_SOURCES})

# Include directories
target_include_directories(SparkyEngine PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${VULKAN_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIRS}
    ${JSON_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../external
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

# Add ENABLE_AUDIO definition if audio is enabled
if(ENABLE_AUDIO)
    target_compile_definitions(SparkyEngine PRIVATE ENABLE_AUDIO)
endif()