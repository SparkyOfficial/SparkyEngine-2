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
    src/Pickup.cpp
    src/InteractiveObject.cpp
    src/SaveGameManager.cpp
    src/Enemy.cpp
    src/FastEnemy.cpp
    src/TankEnemy.cpp
    src/RangedEnemy.cpp
    src/BossEnemy.cpp
    src/WeaponItem.cpp
    src/AmmoItem.cpp
    src/WeaponSystem.cpp
    src/SoundManager.cpp
    src/DamageFeedbackComponent.cpp
    src/LevelProgressionSystem.cpp
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

# Create a mouse look test executable
add_executable(test_mouse_look
    src/mouse_look_test.cpp
)

# Create a PBR material test executable
add_executable(test_pbr_material
    src/pbr_material_test.cpp
)

# Create a physics test executable
add_executable(test_physics
    src/physics_test.cpp
)

# Create an audio test executable
add_executable(test_audio
    src/audio_test.cpp
)

# Create a save/load test executable
add_executable(test_save_load
    src/save_load_test.cpp
)

# Create an enemy test executable
add_executable(test_enemy
    src/enemy_test.cpp
)

# Create a level test executable
add_executable(test_level
    src/level_test.cpp
)

# Create a weapon test executable
add_executable(test_weapon
    src/weapon_test.cpp
)

# Create a sound test executable
add_executable(test_sound
    src/sound_test.cpp
)

# Create a damage feedback test executable
add_executable(test_damage_feedback
    src/damage_feedback_test.cpp
)

# Create a health and damage feedback integration test executable
add_executable(test_health_damage
    src/health_damage_test.cpp
)

# Create a damage feedback demo executable
add_executable(damage_demo
    src/damage_demo.cpp
)

# Create a simple damage feedback test executable
add_executable(simple_damage_test
    src/simple_damage_test.cpp
)

# Create a level progression system test executable
add_executable(test_level_progression
    src/level_progression_test.cpp
)

# Create a progression demo executable
add_executable(progression_demo
    src/progression_demo.cpp
)

target_include_directories(test_level PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GLM_INCLUDE_DIRS}
    ${JSON_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../external
)

target_include_directories(test_weapon PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GLM_INCLUDE_DIRS}
    ${JSON_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../external
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

target_include_directories(test_mouse_look PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GLM_INCLUDE_DIRS}
    ${JSON_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../external
)

target_include_directories(test_pbr_material PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GLM_INCLUDE_DIRS}
    ${JSON_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../external
)

target_include_directories(test_physics PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GLM_INCLUDE_DIRS}
    ${JSON_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../external
)

target_include_directories(test_audio PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GLM_INCLUDE_DIRS}
    ${JSON_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../external
)

target_include_directories(test_save_load PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GLM_INCLUDE_DIRS}
    ${JSON_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../external
)

target_include_directories(test_enemy PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GLM_INCLUDE_DIRS}
    ${JSON_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../external
)

target_include_directories(test_sound PRIVATE
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
    
    target_include_directories(test_mouse_look PRIVATE
        ${VULKAN_INCLUDE_DIRS}
    )
    
    target_include_directories(test_pbr_material PRIVATE
        ${VULKAN_INCLUDE_DIRS}
    )
    
    target_include_directories(test_physics PRIVATE
        ${VULKAN_INCLUDE_DIRS}
    )
    
    target_include_directories(test_audio PRIVATE
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
    
    target_link_libraries(test_mouse_look SparkyEngine)
    
    target_link_libraries(test_mouse_look SparkyEngine
        ${VULKAN_LIBRARIES}
    )
    
    target_link_libraries(test_pbr_material SparkyEngine
        ${VULKAN_LIBRARIES}
    )
    
    target_link_libraries(test_physics SparkyEngine
        ${VULKAN_LIBRARIES}
    )
    
    target_link_libraries(test_audio SparkyEngine
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

target_link_libraries(test_save_load SparkyEngine)

# Only link Vulkan libraries if Vulkan was found
if (Vulkan_FOUND)
    target_link_libraries(test_save_load
        ${VULKAN_LIBRARIES}
    )
endif()

# Only link GLFW libraries if GLFW was found
if (glfw3_FOUND)
    if (TARGET glfw)
        # GLFW found as target
        target_link_libraries(test_save_load glfw)
    else()
        # GLFW found via manual configuration
        target_include_directories(test_save_load PRIVATE
            ${GLFW_INCLUDE_DIRS}
        )
        target_link_libraries(test_save_load
            ${GLFW_LIBRARIES}
        )
    endif()
endif()

target_link_libraries(test_enemy SparkyEngine)

# Only link Vulkan libraries if Vulkan was found
if (Vulkan_FOUND)
    target_link_libraries(test_enemy
        ${VULKAN_LIBRARIES}
    )
endif()

# Only link GLFW libraries if GLFW was found
if (glfw3_FOUND)
    if (TARGET glfw)
        # GLFW found as target
        target_link_libraries(test_enemy glfw)
    else()
        # GLFW found via manual configuration
        target_include_directories(test_enemy PRIVATE
            ${GLFW_INCLUDE_DIRS}
        )
        target_link_libraries(test_enemy
            ${GLFW_LIBRARIES}
        )
    endif()
endif()

target_link_libraries(test_level SparkyEngine)

# Only link Vulkan libraries if Vulkan was found
if (Vulkan_FOUND)
    target_link_libraries(test_level
        ${VULKAN_LIBRARIES}
    )
endif()

# Only link GLFW libraries if GLFW was found
if (glfw3_FOUND)
    if (TARGET glfw)
        # GLFW found as target
        target_link_libraries(test_level glfw)
    else()
        # GLFW found via manual configuration
        target_include_directories(test_level PRIVATE
            ${GLFW_INCLUDE_DIRS}
        )
        target_link_libraries(test_level
            ${GLFW_LIBRARIES}
        )
    endif()
endif()

target_link_libraries(test_weapon SparkyEngine)

# Only link Vulkan libraries if Vulkan was found
if (Vulkan_FOUND)
    target_link_libraries(test_weapon
        ${VULKAN_LIBRARIES}
    )
endif()

# Only link GLFW libraries if GLFW was found
if (glfw3_FOUND)
    if (TARGET glfw)
        # GLFW found as target
        target_link_libraries(test_weapon glfw)
    else()
        # GLFW found via manual configuration
        target_include_directories(test_weapon PRIVATE
            ${GLFW_INCLUDE_DIRS}
        )
        target_link_libraries(test_weapon
            ${GLFW_LIBRARIES}
        )
    endif()
endif()

target_link_libraries(test_sound SparkyEngine)

# Only link Vulkan libraries if Vulkan was found
if (Vulkan_FOUND)
    target_link_libraries(test_sound
        ${VULKAN_LIBRARIES}
    )
endif()

target_link_libraries(test_damage_feedback SparkyEngine)
target_link_libraries(test_health_damage SparkyEngine)
target_link_libraries(damage_demo SparkyEngine)
target_link_libraries(simple_damage_test SparkyEngine)
target_link_libraries(test_level_progression SparkyEngine)
target_link_libraries(progression_demo SparkyEngine)

# Only link Vulkan libraries if Vulkan was found
if (Vulkan_FOUND)
    target_link_libraries(test_damage_feedback
        ${VULKAN_LIBRARIES}
    )
    target_link_libraries(test_health_damage
        ${VULKAN_LIBRARIES}
    )
    target_link_libraries(damage_demo
        ${VULKAN_LIBRARIES}
    )
    target_link_libraries(simple_damage_test
        ${VULKAN_LIBRARIES}
    )
    target_link_libraries(test_level_progression
        ${VULKAN_LIBRARIES}
    )
    target_link_libraries(progression_demo
        ${VULKAN_LIBRARIES}
    )
endif()

# Only link GLFW libraries if GLFW was found
if (glfw3_FOUND)
    if (TARGET glfw)
        # GLFW found as target
        target_link_libraries(test_sound glfw)
        target_link_libraries(test_damage_feedback glfw)
    else()
        # GLFW found via manual configuration
        target_include_directories(test_sound PRIVATE
            ${GLFW_INCLUDE_DIRS}
        )
        target_link_libraries(test_sound
            ${GLFW_LIBRARIES}
        )
        target_include_directories(test_damage_feedback PRIVATE
            ${GLFW_INCLUDE_DIRS}
        )
        target_link_libraries(test_damage_feedback
            ${GLFW_LIBRARIES}
        )
        
        target_include_directories(test_health_damage PRIVATE
            ${GLFW_INCLUDE_DIRS}
        )
        target_link_libraries(test_health_damage
            ${GLFW_LIBRARIES}
        )
        
        target_include_directories(damage_demo PRIVATE
            ${GLFW_INCLUDE_DIRS}
        )
        target_link_libraries(damage_demo
            ${GLFW_LIBRARIES}
        )
        
        target_include_directories(simple_damage_test PRIVATE
            ${GLFW_INCLUDE_DIRS}
        )
        target_link_libraries(simple_damage_test
            ${GLFW_LIBRARIES}
        )
        
        target_include_directories(test_level_progression PRIVATE
            ${GLFW_INCLUDE_DIRS}
        )
        target_link_libraries(test_level_progression
            ${GLFW_LIBRARIES}
        )
        
        target_include_directories(progression_demo PRIVATE
            ${GLFW_INCLUDE_DIRS}
        )
        target_link_libraries(progression_demo
            ${GLFW_LIBRARIES}
        )
    endif()
endif()
