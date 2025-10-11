cmake_minimum_required(VERSION 3.20)
project(SparkyGame)

# Create the game executable
add_executable(SparkyGame
    src/main.cpp
    src/ExampleGame.cpp
    src/Player.cpp
    src/Enemy.cpp
    src/Platform.cpp
    src/PhysicsDemo.cpp
    src/EnhancedPhysicsDemo.cpp
    src/PBRExample.cpp
    src/PBRTest.cpp
    src/PhysicsTest.cpp
    src/AudioDemo.cpp
    src/AudioTest.cpp
    src/Level.cpp
    src/LevelManager.cpp
    src/Gun.cpp
    src/GunImpl.cpp
    src/test_components.cpp
    src/test_json_level.cpp
)

# Include directories
target_include_directories(SparkyGame PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/../Engine/include
    ${GLM_INCLUDE_DIRS}
    ${JSON_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../external
)

# Only include Vulkan include directories if Vulkan was found
if (Vulkan_FOUND)
    target_include_directories(SparkyGame PRIVATE
        ${VULKAN_INCLUDE_DIRS}
    )
endif()

# Link libraries
target_link_libraries(SparkyGame SparkyEngine)

# Only link Vulkan libraries if Vulkan was found
if (Vulkan_FOUND)
    target_link_libraries(SparkyGame
        ${VULKAN_LIBRARIES}
    )
endif()

# Only link GLFW libraries if GLFW was found
if (glfw3_FOUND)
    if (TARGET glfw)
        # GLFW found as target
        target_link_libraries(SparkyGame glfw)
    else()
        # GLFW found via manual configuration
        target_include_directories(SparkyGame PRIVATE
            ${GLFW_INCLUDE_DIRS}
        )
        target_link_libraries(SparkyGame
            ${GLFW_LIBRARIES}
        )
    endif()
endif()

# Add compiler definitions
target_compile_definitions(SparkyGame PRIVATE 
    PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

# Add ENABLE_AUDIO definition if audio is enabled
if(ENABLE_AUDIO)
    target_compile_definitions(SparkyGame PRIVATE ENABLE_AUDIO)
endif()

# Add a definition to indicate if GLFW is available
if (glfw3_FOUND)
    target_compile_definitions(SparkyGame PRIVATE HAS_GLFW)
endif()

# Add a definition to indicate if Vulkan is available
if (Vulkan_FOUND)
    target_compile_definitions(SparkyGame PRIVATE HAS_VULKAN)
endif()

# КОПИРОВАНИЕ ФАЙЛОВ ПРИ СБОРКЕ (более надежный способ)
add_custom_command(TARGET SparkyGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/Engine/shaders"
            "$<TARGET_FILE_DIR:SparkyGame>/Engine/shaders"
    COMMAND ${CMAKE_COMMAND} -E copy
            "${CMAKE_SOURCE_DIR}/Engine/assets/engine.cfg"
            "$<TARGET_FILE_DIR:SparkyGame>/"
    COMMENT "Copying assets and shaders..."
)
