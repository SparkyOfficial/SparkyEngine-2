cmake_minimum_required(VERSION 3.20)
project(SparkyEngine)

# Create the engine library
set(ENGINE_SOURCES
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
    src/Gun.cpp
    src/GunImpl.cpp
    src/SoundManager.cpp
    src/DamageFeedbackComponent.cpp
    src/LevelProgressionSystem.cpp
    src/BallisticsSystem.cpp
    src/ShadowMap.cpp
    src/PostProcessPipeline.cpp
    src/PBRRenderer.cpp
    # Bullet Physics source files
    src/Bullet/BulletRigidBodyComponent.cpp
    src/Bullet/BulletPhysicsWorld.cpp
    src/Bullet/BulletCollisionShapeComponent.cpp
    src/Bullet/BulletConstraintComponent.cpp
    src/AdvancedParticleSystem.cpp
    src/AdvancedAI.cpp
    src/AdvancedWeaponSystem.cpp
    src/AdvancedAnimationSystem.cpp
    src/LevelEditor.cpp
    src/PBRMaterial.cpp
    src/Scene.cpp
    src/AssetManager.cpp
)

set(ENGINE_HEADERS
    include/Animation.h
    include/AnimationComponent.h
    include/AnimationController.h
    include/AssetManager.h
    include/BehaviorTree.h
    include/BehaviorTreeExample.h
    include/Button.h
    include/Camera.h
    include/CharacterController.h
    include/CollisionSystem.h
    include/Component.h
    include/Config.h
    include/DirectionalLight.h
    include/EngineDemo.h
    include/EventSystem.h
    include/ExampleState.h
    include/FileUtils.h
    include/GUIElement.h
    include/GUIManager.h
    include/GameEntity.h
    include/GameObject.h
    include/InputManager.h
    include/Inventory.h
    include/Light.h
    include/LightManager.h
    include/Logger.h
    include/Material.h
    include/Mesh.h
    include/MeshRenderer.h
    include/OBJLoader.h
    include/ParticleSystem.h
    include/ParticleComponent.h
    include/PhysicsComponent.h
    include/PhysicsWorld.h
    include/PointLight.h
    include/Profiler.h
    include/Quest.h
    include/QuestManager.h
    include/RenderComponent.h
    include/RenderSystem.h
    include/ResourceManager.h
    include/RigidBodyComponent.h
    include/ScriptComponent.h
    include/ShaderCompiler.h
    include/ShaderProgram.h
    include/ShaderUtils.h
    include/Skybox.h
    include/SparkyEngine.h
    include/SpotLight.h
    include/StateMachine.h
    include/Texture.h
    include/Timer.h
    include/VulkanRenderer.h
    include/WindowManager.h
    include/HealthComponent.h
    include/AIComponent.h
    include/Pickup.h
    include/InteractiveObject.h
    include/SaveGameManager.h
    include/Enemy.h
    include/FastEnemy.h
    include/TankEnemy.h
    include/RangedEnemy.h
    include/BossEnemy.h
    include/WeaponItem.h
    include/AmmoItem.h
    include/WeaponSystem.h
    include/Gun.h
    include/GunImpl.h
    include/SoundManager.h
    include/DamageFeedbackComponent.h
    include/LevelProgressionSystem.h
    include/BallisticsSystem.h
    include/ShadowMap.h
    include/PostProcessPipeline.h
    include/PBRRenderer.h
    include/AdvancedParticleSystem.h
    include/AdvancedAI.h
    include/AdvancedWeaponSystem.h
    include/AdvancedAnimationSystem.h
    include/LevelEditor.h
)

# Add audio components only if audio is enabled
if(ENABLE_AUDIO)
    list(APPEND ENGINE_SOURCES
        src/AudioEngine.cpp
        src/AudioComponent.cpp
        src/AdvancedAudioEngine.cpp
    )
endif()

# Create the engine library
add_library(SparkyEngine STATIC ${ENGINE_SOURCES})

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

# Create a Bullet Physics test executable
add_executable(test_bullet_physics
    src/bullet_physics_test.cpp
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

# Create a simple physics demo executable
add_executable(simple_physics_demo
    src/simple_physics_demo.cpp
)

# Create a level progression system test executable
add_executable(test_level_progression
    src/level_progression_test.cpp
)

# Create a progression demo executable
add_executable(progression_demo
    src/progression_demo.cpp
)

# Create a ballistics test executable
add_executable(ballistics_test
    src/ballistics_test.cpp
)

target_include_directories(ballistics_test PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GLM_INCLUDE_DIRS}
    ${JSON_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../external
)

target_link_libraries(ballistics_test SparkyEngine)

# Create a weapon ballistics demo executable
add_executable(weapon_ballistics_demo
    src/weapon_ballistics_demo.cpp
)

target_include_directories(weapon_ballistics_demo PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GLM_INCLUDE_DIRS}
    ${JSON_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../external
)

target_link_libraries(weapon_ballistics_demo SparkyEngine)

# Create an advanced audio demo executable
add_executable(advanced_audio_demo
    src/advanced_audio_demo.cpp
)

target_include_directories(advanced_audio_demo PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GLM_INCLUDE_DIRS}
    ${JSON_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../external
)

target_link_libraries(advanced_audio_demo SparkyEngine)

# Create an advanced particle demo executable
add_executable(advanced_particle_demo
    src/advanced_particle_demo.cpp
)

target_include_directories(advanced_particle_demo PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GLM_INCLUDE_DIRS}
    ${JSON_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../external
)

target_link_libraries(advanced_particle_demo SparkyEngine)

# Create a level editor demo executable
add_executable(level_editor_demo
    src/level_editor_demo.cpp
)

# Create an asset manager demo executable
add_executable(asset_manager_demo
    src/asset_manager_demo.cpp
)

target_include_directories(asset_manager_demo PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GLM_INCLUDE_DIRS}
    ${JSON_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../external
)

target_link_libraries(asset_manager_demo SparkyEngine)

# Only link Vulkan libraries if Vulkan was found
if (Vulkan_FOUND)
    target_link_libraries(asset_manager_demo
        ${VULKAN_LIBRARIES}
    )
endif()

# Only link Bullet Physics libraries if Bullet was found
if (Bullet_FOUND)
    target_link_libraries(asset_manager_demo
        ${BULLET_LIBRARIES}
    )
endif()

# Only link GLFW libraries if GLFW was found
if (glfw3_FOUND)
    if (TARGET glfw)
        # GLFW found as target
        target_link_libraries(asset_manager_demo glfw)
    else()
        # GLFW found via manual configuration
        target_include_directories(asset_manager_demo PRIVATE
            ${GLFW_INCLUDE_DIRS}
        )
        target_link_libraries(asset_manager_demo
            ${GLFW_LIBRARIES}
        )
    endif()
    
    # Add definition for HAS_GLFW
    target_compile_definitions(asset_manager_demo PRIVATE HAS_GLFW)
endif()

# Create a logger dependency injection demo executable
add_executable(logger_di_demo
    src/logger_di_demo.cpp
)

target_include_directories(logger_di_demo PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GLM_INCLUDE_DIRS}
    ${JSON_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../external
)

target_link_libraries(logger_di_demo SparkyEngine)

# Only link Vulkan libraries if Vulkan was found
if (Vulkan_FOUND)
    target_link_libraries(logger_di_demo
        ${VULKAN_LIBRARIES}
    )
endif()

# Only link Bullet Physics libraries if Bullet was found
if (Bullet_FOUND)
    target_link_libraries(logger_di_demo
        ${BULLET_LIBRARIES}
    )
endif()

# Only link GLFW libraries if GLFW was found
if (glfw3_FOUND)
    if (TARGET glfw)
        # GLFW found as target
        target_link_libraries(logger_di_demo glfw)
    else()
        # GLFW found via manual configuration
        target_include_directories(logger_di_demo PRIVATE
            ${GLFW_INCLUDE_DIRS}
        )
        target_link_libraries(logger_di_demo
            ${GLFW_LIBRARIES}
        )
    endif()
    
    # Add definition for HAS_GLFW
    target_compile_definitions(logger_di_demo PRIVATE HAS_GLFW)
endif()

target_include_directories(level_editor_demo PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GLM_INCLUDE_DIRS}
    ${JSON_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../external
)

target_link_libraries(level_editor_demo SparkyEngine)

# Create an advanced engine demo executable
add_executable(advanced_engine_demo
    src/advanced_engine_demo.cpp
)

target_include_directories(advanced_engine_demo PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GLM_INCLUDE_DIRS}
    ${JSON_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../external
)

target_link_libraries(advanced_engine_demo SparkyEngine)

# Only link Vulkan libraries if Vulkan was found
if (Vulkan_FOUND)
    target_link_libraries(advanced_engine_demo
        ${VULKAN_LIBRARIES}
    )
endif()

# Only link Bullet Physics libraries if Bullet was found
if (Bullet_FOUND)
    target_link_libraries(advanced_engine_demo
        ${BULLET_LIBRARIES}
    )
endif()

# Only link GLFW libraries if GLFW was found
if (glfw3_FOUND)
    if (TARGET glfw)
        # GLFW found as target
        target_link_libraries(advanced_engine_demo glfw)
    else()
        # GLFW found via manual configuration
        target_include_directories(advanced_engine_demo PRIVATE
            ${GLFW_INCLUDE_DIRS}
        )
        target_link_libraries(advanced_engine_demo
            ${GLFW_LIBRARIES}
        )
    endif()
    
    # Add definition for HAS_GLFW
    target_compile_definitions(advanced_engine_demo PRIVATE HAS_GLFW)
endif()

# Create a PBR demo executable
add_executable(pbr_demo
    src/pbr_demo.cpp
)

target_include_directories(pbr_demo PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GLM_INCLUDE_DIRS}
    ${JSON_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../external
)

target_link_libraries(pbr_demo SparkyEngine)

# Create a scene demo executable
add_executable(scene_demo
    src/scene_demo.cpp
)

target_include_directories(scene_demo PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GLM_INCLUDE_DIRS}
    ${JSON_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../external
)

target_link_libraries(scene_demo SparkyEngine)

# Only link Vulkan libraries if Vulkan was found
if (Vulkan_FOUND)
    target_link_libraries(scene_demo
        ${VULKAN_LIBRARIES}
    )
endif()

# Only link Bullet Physics libraries if Bullet was found
if (Bullet_FOUND)
    target_link_libraries(scene_demo
        ${BULLET_LIBRARIES}
    )
endif()

# Only link GLFW libraries if GLFW was found
if (glfw3_FOUND)
    if (TARGET glfw)
        # GLFW found as target
        target_link_libraries(scene_demo glfw)
    else()
        # GLFW found via manual configuration
        target_include_directories(scene_demo PRIVATE
            ${GLFW_INCLUDE_DIRS}
        )
        target_link_libraries(scene_demo
            ${GLFW_LIBRARIES}
        )
    endif()
    
    # Add definition for HAS_GLFW
    target_compile_definitions(scene_demo PRIVATE HAS_GLFW)
endif()

# Create a PhysicsWorld DI demo executable
add_executable(physicsworld_di_demo
    src/physicsworld_di_demo.cpp
)

target_include_directories(physicsworld_di_demo PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GLM_INCLUDE_DIRS}
    ${JSON_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../external
)

target_link_libraries(physicsworld_di_demo SparkyEngine)

# Only link Vulkan libraries if Vulkan was found
if (Vulkan_FOUND)
    target_link_libraries(physicsworld_di_demo
        ${VULKAN_LIBRARIES}
    )
endif()

# Only link Bullet Physics libraries if Bullet was found
if (Bullet_FOUND)
    target_link_libraries(physicsworld_di_demo
        ${BULLET_LIBRARIES}
    )
endif()

# Only link GLFW libraries if GLFW was found
if (glfw3_FOUND)
    if (TARGET glfw)
        # GLFW found as target
        target_link_libraries(physicsworld_di_demo glfw)
    else()
        # GLFW found via manual configuration
        target_include_directories(physicsworld_di_demo PRIVATE
            ${GLFW_INCLUDE_DIRS}
        )
        target_link_libraries(physicsworld_di_demo
            ${GLFW_LIBRARIES}
        )
    endif()
    
    # Add definition for HAS_GLFW
    target_compile_definitions(physicsworld_di_demo PRIVATE HAS_GLFW)
endif()

# Create a Config DI demo executable
add_executable(config_di_demo
    src/config_di_demo.cpp
)

target_include_directories(config_di_demo PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GLM_INCLUDE_DIRS}
    ${JSON_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../external
)

target_link_libraries(config_di_demo SparkyEngine)

# Only link Vulkan libraries if Vulkan was found
if (Vulkan_FOUND)
    target_link_libraries(config_di_demo
        ${VULKAN_LIBRARIES}
    )
endif()

# Only link Bullet Physics libraries if Bullet was found
if (Bullet_FOUND)
    target_link_libraries(config_di_demo
        ${BULLET_LIBRARIES}
    )
endif()

# Only link GLFW libraries if GLFW was found
if (glfw3_FOUND)
    if (TARGET glfw)
        # GLFW found as target
        target_link_libraries(config_di_demo glfw)
    else()
        # GLFW found via manual configuration
        target_include_directories(config_di_demo PRIVATE
            ${GLFW_INCLUDE_DIRS}
        )
        target_link_libraries(config_di_demo
            ${GLFW_LIBRARIES}
        )
    endif()
    
    # Add definition for HAS_GLFW
    target_compile_definitions(config_di_demo PRIVATE HAS_GLFW)
endif()

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

# Only include Bullet Physics include directories if Bullet was found
if (Bullet_FOUND)
    target_include_directories(SparkyEngine PUBLIC 
        ${BULLET_INCLUDE_DIRS}
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

target_include_directories(test_bullet_physics PRIVATE
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

# Only include Bullet Physics include directories for test executables if Bullet was found
if (Bullet_FOUND)
    target_include_directories(simple_test PRIVATE
        ${BULLET_INCLUDE_DIRS}
    )
    
    target_include_directories(test_unicode PRIVATE
        ${BULLET_INCLUDE_DIRS}
    )
    
    target_include_directories(test_skybox PRIVATE
        ${BULLET_INCLUDE_DIRS}
    )
    
    target_include_directories(test_fileutils PRIVATE
        ${BULLET_INCLUDE_DIRS}
    )
    
    target_include_directories(test_simple_engine PRIVATE
        ${BULLET_INCLUDE_DIRS}
    )
    
    target_include_directories(test_mouse_look PRIVATE
        ${BULLET_INCLUDE_DIRS}
    )
    
    target_include_directories(test_pbr_material PRIVATE
        ${BULLET_INCLUDE_DIRS}
    )
    
    target_include_directories(test_physics PRIVATE
        ${BULLET_INCLUDE_DIRS}
    )
    
    target_include_directories(test_audio PRIVATE
        ${BULLET_INCLUDE_DIRS}
    )
    
    target_include_directories(test_bullet_physics PRIVATE
        ${BULLET_INCLUDE_DIRS}
    )
    
    target_include_directories(simple_physics_demo PRIVATE
        ${BULLET_INCLUDE_DIRS}
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

# Only link Bullet Physics libraries if Bullet was found
if (Bullet_FOUND)
    target_link_libraries(SparkyEngine PRIVATE
        ${BULLET_LIBRARIES}
    )
    
    target_link_libraries(simple_test SparkyEngine
        ${BULLET_LIBRARIES}
    )
    
    target_link_libraries(test_unicode SparkyEngine
        ${BULLET_LIBRARIES}
    )
    
    target_link_libraries(test_skybox SparkyEngine
        ${BULLET_LIBRARIES}
    )
    
    target_link_libraries(test_fileutils SparkyEngine
        ${BULLET_LIBRARIES}
    )
    
    target_link_libraries(test_simple_engine SparkyEngine
        ${BULLET_LIBRARIES}
    )
    
    target_link_libraries(test_mouse_look SparkyEngine
        ${BULLET_LIBRARIES}
    )
    
    target_link_libraries(test_pbr_material SparkyEngine
        ${BULLET_LIBRARIES}
    )
    
    target_link_libraries(test_physics SparkyEngine
        ${BULLET_LIBRARIES}
    )
    
    target_link_libraries(test_audio SparkyEngine
        ${BULLET_LIBRARIES}
    )
    
    target_link_libraries(test_bullet_physics SparkyEngine
        ${BULLET_LIBRARIES}
    )
    
    target_link_libraries(simple_physics_demo SparkyEngine
        ${BULLET_LIBRARIES}
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
        target_include_directories(SparkdEngine PUBLIC 
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

# Add a definition to indicate if Bullet Physics is available
if (Bullet_FOUND)
    target_compile_definitions(SparkyEngine PRIVATE HAS_BULLET)
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

# Only link Bullet Physics libraries if Bullet was found
if (Bullet_FOUND)
    target_link_libraries(test_save_load
        ${BULLET_LIBRARIES}
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

# Only link Bullet Physics libraries if Bullet was found
if (Bullet_FOUND)
    target_link_libraries(test_enemy
        ${BULLET_LIBRARIES}
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

# Only link Bullet Physics libraries if Bullet was found
if (Bullet_FOUND)
    target_link_libraries(test_level
        ${BULLET_LIBRARIES}
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

# Only link Bullet Physics libraries if Bullet was found
if (Bullet_FOUND)
    target_link_libraries(test_weapon
        ${BULLET_LIBRARIES}
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

# Only link Bullet Physics libraries if Bullet was found
if (Bullet_FOUND)
    target_link_libraries(test_sound
        ${BULLET_LIBRARIES}
    )
endif()

target_link_libraries(test_damage_feedback SparkyEngine)
target_link_libraries(test_health_damage SparkyEngine)
target_link_libraries(damage_demo SparkyEngine)
target_link_libraries(simple_damage_test SparkyEngine)
target_link_libraries(test_level_progression SparkyEngine)
target_link_libraries(progression_demo SparkyEngine)
target_link_libraries(simple_physics_demo SparkyEngine)
target_link_libraries(test_bullet_physics SparkyEngine)

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
    target_link_libraries(simple_physics_demo
        ${VULKAN_LIBRARIES}
    )
    target_link_libraries(test_bullet_physics
        ${VULKAN_LIBRARIES}
    )
endif()

# Only link Bullet Physics libraries if Bullet was found
if (Bullet_FOUND)
    target_link_libraries(test_damage_feedback
        ${BULLET_LIBRARIES}
    )
    target_link_libraries(test_health_damage
        ${BULLET_LIBRARIES}
    )
    target_link_libraries(damage_demo
        ${BULLET_LIBRARIES}
    )
    target_link_libraries(simple_damage_test
        ${BULLET_LIBRARIES}
    )
    target_link_libraries(test_level_progression
        ${BULLET_LIBRARIES}
    )
    target_link_libraries(progression_demo
        ${BULLET_LIBRARIES}
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

# Create a behavior tree demo executable
add_executable(behavior_tree_demo
    src/behavior_tree_demo.cpp
)

# Create a shadow mapping demo executable
add_executable(shadow_demo
    src/shadow_demo.cpp
)

# Create a post-processing demo executable
add_executable(postprocess_demo
    src/postprocess_demo.cpp
)

# Create a Vulkan compatibility test executable
# add_executable(vulkan_test
#     src/vulkan_test.cpp
# )

target_include_directories(behavior_tree_demo PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GLM_INCLUDE_DIRS}
    ${JSON_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../external
)

target_link_libraries(behavior_tree_demo SparkyEngine)

# Shadow demo configuration
target_include_directories(shadow_demo PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GLM_INCLUDE_DIRS}
    ${JSON_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../external
)

target_link_libraries(shadow_demo SparkyEngine)

# Post-process demo configuration
target_include_directories(postprocess_demo PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GLM_INCLUDE_DIRS}
    ${JSON_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../external
)

target_link_libraries(postprocess_demo SparkyEngine)

# Vulkan test configuration
# target_include_directories(vulkan_test PRIVATE
#     ${CMAKE_CURRENT_SOURCE_DIR}/include
#     ${GLM_INCLUDE_DIRS}
#     ${JSON_INCLUDE_DIRS}
#     ${CMAKE_CURRENT_SOURCE_DIR}/../external
# )
#
# target_link_libraries(vulkan_test SparkyEngine)

# Create a ballistics demo executable
# Commenting out the ballistics_demo target since src/ballistics_demo.cpp doesn't exist
# add_executable(ballistics_demo
#     src/ballistics_demo.cpp
# )

# target_include_directories(ballistics_demo PRIVATE
#     ${CMAKE_CURRENT_SOURCE_DIR}/include
#     ${GLM_INCLUDE_DIRS}
#     ${JSON_INCLUDE_DIRS}
#     ${CMAKE_CURRENT_SOURCE_DIR}/../external
# )

# target_link_libraries(ballistics_demo SparkyEngine)

# Create an advanced audio demo executable
# Commenting out duplicate target
# add_executable(advanced_audio_demo
#     src/advanced_audio_demo.cpp
# )

# target_include_directories(advanced_audio_demo PRIVATE
#     ${CMAKE_CURRENT_SOURCE_DIR}/include
#     ${GLM_INCLUDE_DIRS}
#     ${JSON_INCLUDE_DIRS}
#     ${CMAKE_CURRENT_SOURCE_DIR}/../external
# )

# target_link_libraries(advanced_audio_demo SparkyEngine)

# Create an advanced particle demo executable
# Commenting out duplicate target
# add_executable(advanced_particle_demo
#     src/advanced_particle_demo.cpp
# )

# target_include_directories(advanced_particle_demo PRIVATE
#     ${CMAKE_CURRENT_SOURCE_DIR}/include
#     ${GLM_INCLUDE_DIRS}
#     ${JSON_INCLUDE_DIRS}
#     ${CMAKE_CURRENT_SOURCE_DIR}/../external
# )

# target_link_libraries(advanced_particle_demo SparkyEngine)

# Create a level editor demo executable
# Commenting out duplicate target
# add_executable(level_editor_demo
#     src/level_editor_demo.cpp
# )

# target_include_directories(level_editor_demo PRIVATE
#     ${CMAKE_CURRENT_SOURCE_DIR}/include
#     ${GLM_INCLUDE_DIRS}
#     ${JSON_INCLUDE_DIRS}
#     ${CMAKE_CURRENT_SOURCE_DIR}/../external
# )

# target_link_libraries(level_editor_demo SparkyEngine)

# Create a scene enhanced test executable
add_executable(scene_enhanced_test
    src/scene_enhanced_test.cpp
)

target_include_directories(scene_enhanced_test PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GLM_INCLUDE_DIRS}
    ${JSON_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../external
)

target_link_libraries(scene_enhanced_test SparkyEngine)

# Only link Vulkan libraries if Vulkan was found
if (Vulkan_FOUND)
    target_link_libraries(scene_enhanced_test
        ${VULKAN_LIBRARIES}
    )
endif()

# Only link Bullet Physics libraries if Bullet was found
if (Bullet_FOUND)
    target_link_libraries(scene_enhanced_test
        ${BULLET_LIBRARIES}
    )
endif()

# Only link GLFW libraries if GLFW was found
if (glfw3_FOUND)
    if (TARGET glfw)
        # GLFW found as target
        target_link_libraries(scene_enhanced_test glfw)
    else()
        # GLFW found via manual configuration
        target_include_directories(scene_enhanced_test PRIVATE
            ${GLFW_INCLUDE_DIRS}
        )
        target_link_libraries(scene_enhanced_test
            ${GLFW_LIBRARIES}
        )
    endif()
    
    # Add definition for HAS_GLFW
    target_compile_definitions(scene_enhanced_test PRIVATE HAS_GLFW)
endif()

# Create an abstraction demo executable
add_executable(abstraction_demo
    src/abstraction_demo.cpp
)

target_include_directories(abstraction_demo PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GLM_INCLUDE_DIRS}
    ${JSON_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../external
)

target_link_libraries(abstraction_demo SparkyEngine)

# Only link Vulkan libraries if Vulkan was found
if (Vulkan_FOUND)
    target_link_libraries(abstraction_demo
        ${VULKAN_LIBRARIES}
    )
endif()

# Only link Bullet Physics libraries if Bullet was found
if (Bullet_FOUND)
    target_link_libraries(abstraction_demo
        ${BULLET_LIBRARIES}
    )
endif()

# Only link GLFW libraries if GLFW was found
if (glfw3_FOUND)
    if (TARGET glfw)
        # GLFW found as target
        target_link_libraries(abstraction_demo glfw)
    else()
        # GLFW found via manual configuration
        target_include_directories(abstraction_demo PRIVATE
            ${GLFW_INCLUDE_DIRS}
        )
        target_link_libraries(abstraction_demo
            ${GLFW_LIBRARIES}
        )
    endif()
    
    # Add definition for HAS_GLFW
    target_compile_definitions(abstraction_demo PRIVATE HAS_GLFW)
endif()

# Create a renderer demo executable
add_executable(renderer_demo
    src/renderer_demo.cpp
)

target_include_directories(renderer_demo PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GLM_INCLUDE_DIRS}
    ${JSON_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../external
)

target_link_libraries(renderer_demo SparkyEngine)

# Only link Vulkan libraries if Vulkan was found
if (Vulkan_FOUND)
    target_link_libraries(renderer_demo
        ${VULKAN_LIBRARIES}
    )
endif()

# Only link Bullet Physics libraries if Bullet was found
if (Bullet_FOUND)
    target_link_libraries(renderer_demo
        ${BULLET_LIBRARIES}
    )
endif()

# Only link GLFW libraries if GLFW was found
if (glfw3_FOUND)
    if (TARGET glfw)
        # GLFW found as target
        target_link_libraries(renderer_demo glfw)
    else()
        # GLFW found via manual configuration
        target_include_directories(renderer_demo PRIVATE
            ${GLFW_INCLUDE_DIRS}
        )
        target_link_libraries(renderer_demo
            ${GLFW_LIBRARIES}
        )
    endif()
    
    # Add definition for HAS_GLFW
    target_compile_definitions(renderer_demo PRIVATE HAS_GLFW)
endif()

# Create a comprehensive demo executable
add_executable(comprehensive_demo
    src/comprehensive_demo.cpp
)

target_include_directories(comprehensive_demo PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GLM_INCLUDE_DIRS}
    ${JSON_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../external
)

target_link_libraries(comprehensive_demo SparkyEngine)

# Only link Vulkan libraries if Vulkan was found
if (Vulkan_FOUND)
    target_link_libraries(comprehensive_demo
        ${VULKAN_LIBRARIES}
    )
endif()

# Only link Bullet Physics libraries if Bullet was found
if (Bullet_FOUND)
    target_link_libraries(comprehensive_demo
        ${BULLET_LIBRARIES}
    )
endif()

# Only link GLFW libraries if GLFW was found
if (glfw3_FOUND)
    if (TARGET glfw)
        # GLFW found as target
        target_link_libraries(comprehensive_demo glfw)
    else()
        # GLFW found via manual configuration
        target_include_directories(comprehensive_demo PRIVATE
            ${GLFW_INCLUDE_DIRS}
        )
        target_link_libraries(comprehensive_demo
            ${GLFW_LIBRARIES}
        )
    endif()
    
    # Add definition for HAS_GLFW
    target_compile_definitions(comprehensive_demo PRIVATE HAS_GLFW)
endif()

# Create an improvements summary executable
add_executable(improvements_summary
    src/improvements_summary.cpp
)

target_include_directories(improvements_summary PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GLM_INCLUDE_DIRS}
    ${JSON_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../external
)

target_link_libraries(improvements_summary SparkyEngine)

# Only link Vulkan libraries if Vulkan was found
if (Vulkan_FOUND)
    target_link_libraries(improvements_summary
        ${VULKAN_LIBRARIES}
    )
endif()

# Only link Bullet Physics libraries if Bullet was found
if (Bullet_FOUND)
    target_link_libraries(improvements_summary
        ${BULLET_LIBRARIES}
    )
endif()

# Only link GLFW libraries if GLFW was found
if (glfw3_FOUND)
    if (TARGET glfw)
        # GLFW found as target
        target_link_libraries(improvements_summary glfw)
    else()
        # GLFW found via manual configuration
        target_include_directories(improvements_summary PRIVATE
            ${GLFW_INCLUDE_DIRS}
        )
        target_link_libraries(improvements_summary
            ${GLFW_LIBRARIES}
        )
    endif()
    
    # Add definition for HAS_GLFW
    target_compile_definitions(improvements_summary PRIVATE HAS_GLFW)
endif()

# Create a Phase 2 comprehensive demo executable
add_executable(phase2_comprehensive_demo
    src/phase2_comprehensive_demo.cpp
)

target_include_directories(phase2_comprehensive_demo PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GLM_INCLUDE_DIRS}
    ${JSON_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../external
)

target_link_libraries(phase2_comprehensive_demo SparkyEngine)

# Only link Vulkan libraries if Vulkan was found
if (Vulkan_FOUND)
    target_link_libraries(phase2_comprehensive_demo
        ${VULKAN_LIBRARIES}
    )
endif()

# Only link Bullet Physics libraries if Bullet was found
if (Bullet_FOUND)
    target_link_libraries(phase2_comprehensive_demo
        ${BULLET_LIBRARIES}
    )
endif()

# Only link GLFW libraries if GLFW was found
if (glfw3_FOUND)
    if (TARGET glfw)
        # GLFW found as target
        target_link_libraries(phase2_comprehensive_demo glfw)
    else()
        # GLFW found via manual configuration
        target_include_directories(phase2_comprehensive_demo PRIVATE
            ${GLFW_INCLUDE_DIRS}
        )
        target_link_libraries(phase2_comprehensive_demo
            ${GLFW_LIBRARIES}
        )
    endif()
    
    # Add definition for HAS_GLFW
    target_compile_definitions(phase2_comprehensive_demo PRIVATE HAS_GLFW)
endif()

# Create a Phase 2 improvements summary executable
add_executable(phase2_improvements_summary
    src/phase2_improvements_summary.cpp
)

target_include_directories(phase2_improvements_summary PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GLM_INCLUDE_DIRS}
    ${JSON_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../external
)

target_link_libraries(phase2_improvements_summary SparkyEngine)

# Only link Vulkan libraries if Vulkan was found
if (Vulkan_FOUND)
    target_link_libraries(phase2_improvements_summary
        ${VULKAN_LIBRARIES}
    )
endif()

# Only link Bullet Physics libraries if Bullet was found
if (Bullet_FOUND)
    target_link_libraries(phase2_improvements_summary
        ${BULLET_LIBRARIES}
    )
endif()

# Only link GLFW libraries if GLFW was found
if (glfw3_FOUND)
    if (TARGET glfw)
        # GLFW found as target
        target_link_libraries(phase2_improvements_summary glfw)
    else()
        # GLFW found via manual configuration
        target_include_directories(phase2_improvements_summary PRIVATE
            ${GLFW_INCLUDE_DIRS}
        )
        target_link_libraries(phase2_improvements_summary
            ${GLFW_LIBRARIES}
        )
    endif()
    
    # Add definition for HAS_GLFW
    target_compile_definitions(phase2_improvements_summary PRIVATE HAS_GLFW)
endif()
