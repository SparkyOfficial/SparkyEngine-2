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
    # src/PBRExample.cpp  # Commented out due to compilation issues
    # src/PBRTest.cpp  # Commented out because it has main() function
    # src/PhysicsTest.cpp  # Commented out because it has main() function
    # src/AudioDemo.cpp  # Commented out due to audio dependency issues
    # src/AudioTest.cpp  # Commented out due to audio dependency issues
    src/Level.cpp
    src/LevelManager.cpp
    src/Gun.cpp
    src/GunImpl.cpp
    # src/test_components.cpp  # Commented out because it has main() function
    # src/test_json_level.cpp  # Commented out because it has main() function
)

# Create a simple demo executable
add_executable(SparkyDemo
    src/simple_demo.cpp
)

# Create a Level Loader demo executable
add_executable(LevelLoaderDemo
    src/level_loader_demo.cpp
)

# Create a Half-Life and Castle Wolfenstein inspired demo executable
add_executable(HalfLifeCastleWolfensteinDemo
    src/half_life_castle_wolfenstein_demo.cpp
)

# Include directories
target_include_directories(SparkyGame PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/../Engine/include
    ${GLM_INCLUDE_DIRS}
    ${JSON_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../external
)

target_include_directories(SparkyDemo PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/../Engine/include
    ${GLM_INCLUDE_DIRS}
    ${JSON_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../external
)

target_include_directories(LevelLoaderDemo PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/../Engine/include
    ${GLM_INCLUDE_DIRS}
    ${JSON_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../external
)

target_include_directories(HalfLifeCastleWolfensteinDemo PRIVATE
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
    
    target_include_directories(SparkyDemo PRIVATE
        ${VULKAN_INCLUDE_DIRS}
    )
    
    target_include_directories(LevelLoaderDemo PRIVATE
        ${VULKAN_INCLUDE_DIRS}
    )
    
    target_include_directories(HalfLifeCastleWolfensteinDemo PRIVATE
        ${VULKAN_INCLUDE_DIRS}
    )
endif()

# Link libraries
target_link_libraries(SparkyGame SparkyEngine)

target_link_libraries(SparkyDemo SparkyEngine)

target_link_libraries(LevelLoaderDemo SparkyEngine)

target_link_libraries(HalfLifeCastleWolfensteinDemo SparkyEngine)

# Only link Vulkan libraries if Vulkan was found
if (Vulkan_FOUND)
    target_link_libraries(SparkyGame
        ${VULKAN_LIBRARIES}
    )
    
    target_link_libraries(SparkyDemo
        ${VULKAN_LIBRARIES}
    )
    
    target_link_libraries(LevelLoaderDemo
        ${VULKAN_LIBRARIES}
    )
    
    target_link_libraries(HalfLifeCastleWolfensteinDemo
        ${VULKAN_LIBRARIES}
    )
endif()

# Only link GLFW libraries if GLFW was found
if (glfw3_FOUND)
    if (TARGET glfw)
        # GLFW found as target
        target_link_libraries(SparkyGame glfw)
        target_link_libraries(SparkyDemo glfw)
        target_link_libraries(LevelLoaderDemo glfw)
        target_link_libraries(HalfLifeCastleWolfensteinDemo glfw)
    else()
        # GLFW found via manual configuration
        target_include_directories(SparkyGame PRIVATE
            ${GLFW_INCLUDE_DIRS}
        )
        target_link_libraries(SparkyGame
            ${GLFW_LIBRARIES}
        )
        
        target_include_directories(SparkyDemo PRIVATE
            ${GLFW_INCLUDE_DIRS}
        )
        target_link_libraries(SparkyDemo
            ${GLFW_LIBRARIES}
        )
        
        target_include_directories(LevelLoaderDemo PRIVATE
            ${GLFW_INCLUDE_DIRS}
        )
        target_link_libraries(LevelLoaderDemo
            ${GLFW_LIBRARIES}
        )
        
        target_include_directories(HalfLifeCastleWolfensteinDemo PRIVATE
            ${GLFW_INCLUDE_DIRS}
        )
        target_link_libraries(HalfLifeCastleWolfensteinDemo
            ${GLFW_LIBRARIES}
        )
    endif()
endif()

# Add compiler definitions
target_compile_definitions(SparkyGame PRIVATE 
    PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

target_compile_definitions(SparkyDemo PRIVATE 
    PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

target_compile_definitions(LevelLoaderDemo PRIVATE 
    PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

target_compile_definitions(HalfLifeCastleWolfensteinDemo PRIVATE 
    PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

# Add ENABLE_AUDIO definition if audio is enabled
if(ENABLE_AUDIO)
    target_compile_definitions(SparkyGame PRIVATE ENABLE_AUDIO)
    target_compile_definitions(SparkyDemo PRIVATE ENABLE_AUDIO)
    target_compile_definitions(LevelLoaderDemo PRIVATE ENABLE_AUDIO)
    target_compile_definitions(HalfLifeCastleWolfensteinDemo PRIVATE ENABLE_AUDIO)
endif()

# Add a definition to indicate if GLFW is available
if (glfw3_FOUND)
    target_compile_definitions(SparkyGame PRIVATE HAS_GLFW)
    target_compile_definitions(SparkyDemo PRIVATE HAS_GLFW)
    target_compile_definitions(LevelLoaderDemo PRIVATE HAS_GLFW)
    target_compile_definitions(HalfLifeCastleWolfensteinDemo PRIVATE HAS_GLFW)
endif()

# Add a definition to indicate if Vulkan is available
if (Vulkan_FOUND)
    target_compile_definitions(SparkyGame PRIVATE HAS_VULKAN)
    target_compile_definitions(SparkyDemo PRIVATE HAS_VULKAN)
    target_compile_definitions(LevelLoaderDemo PRIVATE HAS_VULKAN)
    target_compile_definitions(HalfLifeCastleWolfensteinDemo PRIVATE HAS_VULKAN)
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