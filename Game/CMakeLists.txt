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
    src/PhysicsDemo.cpp
)

# Create a test executable for JSON parsing
add_executable(test_json_parsing
    src/test_json_parsing.cpp
    src/Level.cpp
    src/Platform.cpp
    src/Gun.cpp
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
target_link_libraries(test_json_parsing SparkyEngine 
    ${VULKAN_LIBRARIES}
    ${GLFW_LIBRARIES}
    ${glslang_LIBRARIES}
)

# Include directories
target_include_directories(SparkyGame PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/../Engine/include
)

# Include directories for test executable
target_include_directories(test_json_parsing PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/../Engine/include
)