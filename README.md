# Sparky Engine 3D Platformer Game

A powerful 3D game engine built with C++ and Vulkan, featuring a platformer game with platforms, ramps, stairs, and a table with a usable gun.

## Features

- Vulkan-based 3D rendering engine
- Entity-component architecture
- Physics system with collision detection
- Audio system
- Advanced lighting and materials
- GUI system
- Quest and inventory systems
- Particle effects system
- Animation system
- Behavior tree AI system

## Prerequisites

- Visual Studio 2022
- Vulkan SDK 1.4.321.1
- GLFW 3.4
- GLM (OpenGL Mathematics)

## Building the Project

### Method 1: Using the build script

1. Double-click `build.bat` to build the project
2. The executable will be generated in `build\bin\Release\` directory

### Method 2: Manual build

1. Open a command prompt in the project directory
2. Run the following commands:
   ```
   mkdir build
   cd build
   cmake .. -G "Visual Studio 17 2022" -A x64
   cmake --build . --config Release
   ```

## Running the Game

After building, run the executable:
```
build\bin\Release\SparkyGame.exe
```

## Controls

- WASD - Move player
- Space - Jump
- Mouse - Look around
- ESC - Exit game

## Project Structure

- `Engine/` - Core engine components
- `Game/` - Game-specific implementation
- `Engine/include/` - Header files
- `Engine/src/` - Source files
- `Game/src/` - Game source files
- `Engine/shaders/` - GLSL shader files
- `Game/assets/` - Game assets

## Game Elements

The game features:
- A player character that can move and jump
- Various platforms to navigate
- Ramps and stairs for complex movement
- A table with a usable gun
- Physics-based interactions
- Lighting effects
- Audio feedback

## Documentation

See `SUMMARY.md` for a complete overview of the engine architecture and features.

## License

This project is licensed under the MIT License - see the LICENSE file for details.