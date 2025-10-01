cmake_minimum_required(VERSION 3.20)
project(SparkyGame)

add_executable(SparkyGame
    src/main.cpp
    src/ExampleGame.cpp 
    src/Player.cpp
    src/Platform.cpp
    src/Gun.cpp
    src/Level.cpp
    src/Enemy.cpp
    src/PhysicsDemo.cpp
)

target_compile_definitions(SparkyGame PRIVATE PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

# Only link Vulkan libraries if Vulkan was found
if (Vulkan_FOUND)
    target_link_libraries(SparkyGame SparkyEngine 
        ${VULKAN_LIBRARIES}
    )
else()
    target_link_libraries(SparkyGame SparkyEngine)
endif()

# Only link GLFW libraries if GLFW was found
if (glfw3_FOUND)
    target_link_libraries(SparkyGame 
        ${GLFW_LIBRARIES}
    )
endif()

target_include_directories(SparkyGame PRIVATE 
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
endif()

# Only include GLFW include directories if GLFW was found
if (glfw3_FOUND)
    target_include_directories(SparkyGame PRIVATE 
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