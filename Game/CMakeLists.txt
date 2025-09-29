cmake_minimum_required(VERSION 3.20)
project(SparkyGame)

# Create the game executable
add_executable(SparkyGame
    src/main.cpp
    src/Player.cpp
    src/Platform.cpp
    src/Gun.cpp
    src/Level.cpp
    src/Enemy.cpp
    src/ExampleGame.cpp
    src/PhysicsDemo.cpp
)

# Create a test executable for JSON parsing
add_executable(test_json_level
    src/test_json_level.cpp
    src/Level.cpp
    src/Platform.cpp
    src/Gun.cpp
    src/Enemy.cpp
)

# Create a test executable for the new components
add_executable(test_components
    src/test_components.cpp
    ../Engine/src/HealthComponent.cpp
    ../Engine/src/AIComponent.cpp
)

# Pass the project source directory to the code
target_compile_definitions(SparkyGame PRIVATE PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}")

# Link with the engine library
target_link_libraries(SparkyGame SparkyEngine 
    ${VULKAN_LIBRARIES}
    ${GLFW_LIBRARIES}
    ${glslang_LIBRARIES}
)

# Link the test executable with the engine library
target_link_libraries(test_json_level SparkyEngine 
    ${VULKAN_LIBRARIES}
    ${GLFW_LIBRARIES}
    ${glslang_LIBRARIES}
)

# Link the components test executable
target_link_libraries(test_components SparkyEngine 
    ${VULKAN_LIBRARIES}
    ${GLFW_LIBRARIES}
    ${glslang_LIBRARIES}
)

# Include directories
target_include_directories(SparkyGame PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/../Engine/include
)

# Include directories for test executable
target_include_directories(test_json_level PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/../Engine/include
)

# Include directories for components test executable
target_include_directories(test_components PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/../Engine/include
)