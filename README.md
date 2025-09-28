# Sparky Engine 2

A powerful 3D game engine built with C++ and Vulkan, featuring a complete platformer game example.

## Features

- Vulkan-based 3D rendering engine
- Entity-component architecture
- Physics system with collision detection
- Audio engine with OpenAL
- Advanced lighting system (directional, point, and spot lights)
- GUI system with event handling
- Animation system
- Particle effects system
- Quest and inventory systems
- Behavior tree AI system
- Scripting component for flexible game logic

## Prerequisites

- Windows 10/11
- Visual Studio 2022 or later
- CMake 3.20 or later
- Vulkan SDK 1.3 or later
- GLFW3
- GLM (OpenGL Mathematics)
- glslang (for shader compilation)

## Installing Dependencies

Run the dependency installer script for guidance:
```
install_dependencies.bat
```

Or manually install:

1. **Vulkan SDK**
   - Download from: https://vulkan.lunarg.com/
   - Install the latest version for Windows
   - Make sure to select "Add to PATH" during installation

2. **GLFW3**
   - Download from: https://www.glfw.org/download.html
   - Or install via vcpkg:
     ```
     vcpkg install glfw3
     ```

3. **GLM (OpenGL Mathematics)**
   - Download from: https://github.com/g-truc/glm/releases
   - Or install via vcpkg:
     ```
     vcpkg install glm
     ```

4. **glslang**
   - Usually included with Vulkan SDK
   - Or install via vcpkg:
     ```
     vcpkg install glslang
     ```

## Building

### Windows

1. Make sure you have all prerequisites installed
2. Run the build script:
   ```
   build.bat
   ```

   Or manually build:
   ```
   mkdir build
   cd build
   cmake .. -G "Visual Studio 17 2022"
   cmake --build . --config Release
   ```

## Running

### Test Application
To run the Vulkan test application:
```
run_test.bat
```

### Game
To run the platformer game:
```
run_game.bat
```

Or manually:
```
cd build
Game\Release\SparkyGame.exe
```

## Project Structure

- `Engine/` - Core engine code
  - `src/` - Source files
  - `include/` - Header files
  - `shaders/` - GLSL shader files
  - `assets/` - Engine assets
- `Game/` - Game-specific code and assets
  - `src/` - Source files
  - `assets/` - Game assets

## Controls

### Game Controls
- WASD - Move player
- Space - Jump
- Mouse - Look around
- ESC - Exit game

## Documentation

See `SUMMARY.md` for a complete overview of the engine architecture and features.

## License

This project is licensed under the MIT License - see the LICENSE file for details.