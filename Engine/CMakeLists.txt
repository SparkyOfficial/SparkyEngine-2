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
    src/OBJLoader.cpp
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
    src/Skybox.cpp
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

# Create a simple test executable
add_executable(simple_test
    src/simple_test.cpp
)

# Create a Unicode test executable
add_executable(test_unicode
    src/test_unicode.cpp
)

# Create a skybox test executable
add_executable(test_skybox
    src/skybox_test.cpp
)

# Create a fileutils test executable
add_executable(test_fileutils
    src/fileutils_test.cpp
)

# Create a simple engine test executable
add_executable(test_simple_engine
    src/simple_engine_test.cpp
)

# Include directories
target_include_directories(SparkyEngine PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GLM_INCLUDE_DIRS}
    ${JSON_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../external
)

# Only include Vulkan include directories if Vulkan was found
if (Vulkan_FOUND)
    target_include_directories(SparkyEngine PUBLIC 
        ${VULKAN_INCLUDE_DIRS}
    )
endif()

# GLFW linking will be handled in the unified section below

target_include_directories(simple_test PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GLM_INCLUDE_DIRS}
    ${JSON_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../external
)

target_include_directories(test_unicode PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GLM_INCLUDE_DIRS}
    ${JSON_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../external
)

target_include_directories(test_skybox PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GLM_INCLUDE_DIRS}
    ${JSON_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../external
)

target_include_directories(test_fileutils PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GLM_INCLUDE_DIRS}
    ${JSON_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../external
)

target_include_directories(test_simple_engine PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GLM_INCLUDE_DIRS}
    ${JSON_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../external
)

# Only include Vulkan include directories for test executables if Vulkan was found
if (Vulkan_FOUND)
    target_include_directories(simple_test PRIVATE
        ${VULKAN_INCLUDE_DIRS}
    )
    
    target_include_directories(test_unicode PRIVATE
        ${VULKAN_INCLUDE_DIRS}
    )
    
    target_include_directories(test_skybox PRIVATE
        ${VULKAN_INCLUDE_DIRS}
    )
    
    target_include_directories(test_fileutils PRIVATE
        ${VULKAN_INCLUDE_DIRS}
    )
    
    target_include_directories(test_simple_engine PRIVATE
        ${VULKAN_INCLUDE_DIRS}
    )
endif()

# Link libraries
# Only link Vulkan libraries if Vulkan was found
if (Vulkan_FOUND)
    target_link_libraries(SparkyEngine PRIVATE
        ${VULKAN_LIBRARIES}
    )
    
    target_link_libraries(simple_test SparkyEngine
        ${VULKAN_LIBRARIES}
    )
    
    target_link_libraries(test_unicode SparkyEngine
        ${VULKAN_LIBRARIES}
    )
    
    target_link_libraries(test_skybox SparkyEngine
        ${VULKAN_LIBRARIES}
    )
    
    target_link_libraries(test_fileutils SparkyEngine
        ${VULKAN_LIBRARIES}
    )
    
    target_link_libraries(test_simple_engine SparkyEngine
        ${VULKAN_LIBRARIES}
    )
endif()

# Only link GLFW libraries if GLFW was found
if (glfw3_FOUND)
    if (TARGET glfw)
        # GLFW found as target
        target_link_libraries(SparkyEngine PRIVATE glfw)
        target_link_libraries(simple_test SparkyEngine glfw)
        target_link_libraries(test_unicode SparkyEngine glfw)
        target_link_libraries(test_skybox SparkyEngine glfw)
        target_link_libraries(test_fileutils SparkyEngine glfw)
        target_link_libraries(test_simple_engine SparkyEngine glfw)
    else()
        # GLFW found via manual configuration
        target_include_directories(SparkyEngine PUBLIC 
            ${GLFW_INCLUDE_DIRS}
        )
        target_link_libraries(SparkyEngine PRIVATE ${GLFW_LIBRARIES})
        target_link_libraries(simple_test SparkyEngine
            ${GLFW_LIBRARIES}
        )
        target_link_libraries(test_unicode SparkyEngine
            ${GLFW_LIBRARIES}
        )
        target_link_libraries(test_skybox SparkyEngine
            ${GLFW_LIBRARIES}
        )
        target_link_libraries(test_fileutils SparkyEngine
            ${GLFW_LIBRARIES}
        )
        target_link_libraries(test_simple_engine SparkyEngine
            ${GLFW_LIBRARIES}
        )
    endif()
endif()

# Compiler definitions
target_compile_definitions(SparkyEngine PRIVATE 
    PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

# Add ENABLE_AUDIO definition if audio is enabled
if(ENABLE_AUDIO)
    target_compile_definitions(SparkyEngine PRIVATE ENABLE_AUDIO)
endif()

# Add a definition to indicate if GLFW is available
if (glfw3_FOUND)
    target_compile_definitions(SparkyEngine PRIVATE HAS_GLFW)
endif()

# Add a definition to indicate if Vulkan is available
if (Vulkan_FOUND)
    target_compile_definitions(SparkyEngine PRIVATE HAS_VULKAN)
endif()

# Copy assets and shaders to build directory
add_custom_command(TARGET SparkyEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/assets
    ${CMAKE_CURRENT_BINARY_DIR}/assets
)

add_custom_command(TARGET SparkyEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders
    ${CMAKE_CURRENT_BINARY_DIR}/shaders
)