cmake_minimum_required(VERSION 3.20)
project(SparkyGame)

add_executable(SparkyGame
    src/main.cpp
    src/ExampleGame.cpp 
    src/Player.cpp
    src/Platform.cpp
    src/Gun.cpp
    src/GunImpl.cpp
    src/Level.cpp
    src/Enemy.cpp
    src/PhysicsDemo.cpp
)

add_executable(PBRTest
    src/PBRTest.cpp
    src/PBRExample.cpp
)

add_executable(PhysicsTest
    src/PhysicsTest.cpp
    src/EnhancedPhysicsDemo.cpp
)

add_executable(AudioTest
    src/AudioTest.cpp
    src/AudioDemo.cpp
)

target_compile_definitions(SparkyGame PRIVATE PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

# Only link Vulkan libraries if Vulkan was found
if (Vulkan_FOUND)
    target_link_libraries(SparkyGame SparkyEngine 
        ${VULKAN_LIBRARIES}
    )
    target_link_libraries(PBRTest SparkyEngine 
        ${VULKAN_LIBRARIES}
    )
    target_link_libraries(PhysicsTest SparkyEngine 
        ${VULKAN_LIBRARIES}
    )
    target_link_libraries(AudioTest SparkyEngine 
        ${VULKAN_LIBRARIES}
    )
else()
    target_link_libraries(SparkyGame SparkyEngine)
    target_link_libraries(PBRTest SparkyEngine)
    target_link_libraries(PhysicsTest SparkyEngine)
    target_link_libraries(AudioTest SparkyEngine)
endif()

# Only link GLFW libraries if GLFW was found
if (glfw3_FOUND)
    target_link_libraries(SparkyGame 
        ${GLFW_LIBRARIES}
    )
    target_link_libraries(PBRTest 
        ${GLFW_LIBRARIES}
    )
    target_link_libraries(PhysicsTest 
        ${GLFW_LIBRARIES}
    )
    target_link_libraries(AudioTest 
        ${GLFW_LIBRARIES}
    )
    # Add definition to indicate GLFW is available
    target_compile_definitions(SparkyGame PRIVATE HAS_GLFW)
    target_compile_definitions(PBRTest PRIVATE HAS_GLFW)
    target_compile_definitions(PhysicsTest PRIVATE HAS_GLFW)
    target_compile_definitions(AudioTest PRIVATE HAS_GLFW)
endif()

target_include_directories(SparkyGame PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/../Engine/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../external
    ${GLM_INCLUDE_DIRS}
    ${JSON_INCLUDE_DIRS}
)

target_include_directories(PBRTest PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/../Engine/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../external
    ${GLM_INCLUDE_DIRS}
    ${JSON_INCLUDE_DIRS}
)

target_include_directories(PhysicsTest PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/../Engine/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../external
    ${GLM_INCLUDE_DIRS}
    ${JSON_INCLUDE_DIRS}
)

target_include_directories(AudioTest PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/../Engine/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../external
    ${GLM_INCLUDE_DIRS}
    ${JSON_INCLUDE_DIRS}
)

# Only include Vulkan include directories if Vulkan was found
if (Vulkan_FOUND)
    target_include_directories(SparkyGame PRIVATE 
        ${VULKAN_INCLUDE_DIRS}
    )
    target_include_directories(PBRTest PRIVATE 
        ${VULKAN_INCLUDE_DIRS}
    )
    target_include_directories(PhysicsTest PRIVATE 
        ${VULKAN_INCLUDE_DIRS}
    )
    target_include_directories(AudioTest PRIVATE 
        ${VULKAN_INCLUDE_DIRS}
    )
endif()

# Only include GLFW include directories if GLFW was found
if (glfw3_FOUND)
    target_include_directories(SparkyGame PRIVATE 
        ${GLFW_INCLUDE_DIRS}
    )
    target_include_directories(PBRTest PRIVATE 
        ${GLFW_INCLUDE_DIRS}
    )
    target_include_directories(PhysicsTest PRIVATE 
        ${GLFW_INCLUDE_DIRS}
    )
    target_include_directories(AudioTest PRIVATE 
        ${GLFW_INCLUDE_DIRS}
    )
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

add_custom_command(TARGET PBRTest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/Engine/shaders"
            "$<TARGET_FILE_DIR:PBRTest>/Engine/shaders"
    COMMAND ${CMAKE_COMMAND} -E copy
            "${CMAKE_SOURCE_DIR}/Engine/assets/engine.cfg"
            "$<TARGET_FILE_DIR:PBRTest>/"
    COMMENT "Copying assets and shaders for PBRTest..."
)

add_custom_command(TARGET PhysicsTest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/Engine/shaders"
            "$<TARGET_FILE_DIR:PhysicsTest>/Engine/shaders"
    COMMAND ${CMAKE_COMMAND} -E copy
            "${CMAKE_SOURCE_DIR}/Engine/assets/engine.cfg"
            "$<TARGET_FILE_DIR:PhysicsTest>/"
    COMMENT "Copying assets and shaders for PhysicsTest..."
)

add_custom_command(TARGET AudioTest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/Engine/shaders"
            "$<TARGET_FILE_DIR:AudioTest>/Engine/shaders"
    COMMAND ${CMAKE_COMMAND} -E copy
            "${CMAKE_SOURCE_DIR}/Engine/assets/engine.cfg"
            "$<TARGET_FILE_DIR:AudioTest>/"
    COMMENT "Copying assets and shaders for AudioTest..."
)