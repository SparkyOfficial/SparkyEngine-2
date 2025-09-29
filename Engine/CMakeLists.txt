cmake_minimum_required(VERSION 3.20)
project(SparkyEngine)

# Create the engine library
set(ENGINE_SOURCES
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
    src/RigidBodyComponent.cpp
    src/PhysicsWorld.cpp
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
    src/ShaderCompiler.cpp
    src/Light.cpp
    src/DirectionalLight.cpp
    src/PointLight.cpp
    src/SpotLight.cpp
    src/LightManager.cpp
    src/GUIElement.cpp
    src/Button.cpp
    src/GUIManager.cpp
    src/CharacterController.cpp
    src/Inventory.cpp
    src/Quest.cpp
    src/QuestManager.cpp
    src/ParticleSystem.cpp
    src/Animation.cpp
    src/AnimationController.cpp
    src/ScriptComponent.cpp
    src/BehaviorTree.cpp
    src/BehaviorTreeExample.cpp
    src/RenderSystem.cpp
    src/Texture.cpp
)

# Only include AudioEngine if audio is enabled
if(ENABLE_AUDIO)
    list(APPEND ENGINE_SOURCES src/AudioEngine.cpp)
endif()

add_library(SparkyEngine STATIC ${ENGINE_SOURCES})

# Pass the project source directory to the code
target_compile_definitions(SparkyEngine PRIVATE PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}")

# Include directories
target_include_directories(SparkyEngine PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../external
    E:/VulkanSDK/1.4.321.1/Include
    C:/glm-install/include
    C:/glfw-install/include
)

# Check for glslang headers
if(EXISTS "${VULKAN_SDK}/Include/glslang/Public/ShaderLang.h")
    set(HAS_GLSLANG TRUE)
    message(STATUS "Found glslang headers")
else()
    set(HAS_GLSLANG FALSE)
    message(STATUS "glslang headers not found, shader compilation will be disabled")
endif()

# Link libraries
target_link_libraries(SparkyEngine 
    ${VULKAN_LIBRARIES}
    ${GLFW_LIBRARIES}
    ${glslang_LIBRARIES}
)

# Only link OpenAL if audio is enabled
if(ENABLE_AUDIO)
    # Try to find OpenAL
    find_package(OpenAL QUIET)
    if(OpenAL_FOUND)
        target_link_libraries(SparkyEngine ${OPENAL_LIBRARY})
        target_include_directories(SparkyEngine PRIVATE ${OPENAL_INCLUDE_DIR})
    else()
        # Try to find OpenAL in common locations
        find_path(OPENAL_INCLUDE_DIR AL/al.h)
        find_library(OPENAL_LIBRARY OpenAL32)
        if(OPENAL_INCLUDE_DIR AND OPENAL_LIBRARY)
            target_link_libraries(SparkyEngine ${OPENAL_LIBRARY})
            target_include_directories(SparkyEngine PRIVATE ${OPENAL_INCLUDE_DIR})
        else()
            message(WARNING "OpenAL not found, audio support will be disabled")
            # Remove AudioEngine from sources if OpenAL not found
            list(REMOVE_ITEM ENGINE_SOURCES src/AudioEngine.cpp)
        endif()
    endif()
endif()

# Compiler definitions
target_compile_definitions(SparkyEngine PRIVATE VK_PROTOTYPES)

# Define HAS_GLSLANG if glslang headers are found
if(HAS_GLSLANG)
    target_compile_definitions(SparkyEngine PRIVATE HAS_GLSLANG)
endif()

# Define ENABLE_AUDIO if audio is enabled and OpenAL is found
if(ENABLE_AUDIO)
    if(OPENAL_INCLUDE_DIR AND OPENAL_LIBRARY)
        target_compile_definitions(SparkyEngine PRIVATE ENABLE_AUDIO)
    endif()
endif()

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