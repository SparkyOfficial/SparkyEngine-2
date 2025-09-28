cmake_minimum_required(VERSION 3.20)
project(SparkyGame)

# Create the game executable
add_executable(SparkyGame
    src/main.cpp
    src/Player.cpp
    src/Platform.cpp
    src/Gun.cpp
    src/Level.cpp
    src/ExampleGame.cpp
)

# Link with the engine library
target_link_libraries(SparkyGame SparkyEngine 
    ${VULKAN_LIBRARIES}
    ${GLFW_LIBRARIES}
    ${glslang_LIBRARIES}
)

# Include directories
target_include_directories(SparkyGame PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/../Engine/include
)