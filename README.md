# SparkyEngine-2

A powerful 3D game engine built with C++ and Vulkan, featuring a platformer game with platforms, ramps, stairs, and a table with a usable gun.

## Building the Project

1. Make sure you have CMake 3.20+ and Visual Studio installed
2. Open a command prompt in the project directory
3. Create a build directory: `mkdir build`
4. Navigate to the build directory: `cd build`
5. Configure with CMake (audio disabled to avoid OpenAL dependencies): 
   `cmake .. -DENABLE_AUDIO=OFF`
6. Build the project: `cmake --build . --config Release`

## Running the Game

After building, the game executable can be found at:
`build\Game\Release\SparkyGame.exe`

The game runs in a windowed mode (1280x720) and features:
- 3D rendering with Vulkan
- Player character with movement controls
- Enemy AI
- Physics system
- Multiple platforms, ramps, and stairs
- Weapon system

## Running Tests

Several test executables are available in:
`build\Engine\Release\`

These include tests for:
- Physics system
- PBR materials
- File utilities
- Enemy AI
- Weapon system
- And many more engine components

## Engine Features

The SparkyEngine2 includes 17 core systems:
1. Component-based architecture
2. Vulkan graphics rendering
3. Physics system
4. Audio system (when enabled)
5. Input management
6. AI behavior trees
7. Animation system
8. Particle effects
9. GUI system
10. Quest management
11. Save/load functionality
12. Level progression
13. Inventory system
14. Weapon mechanics
15. Health/damage system
16. Camera system
17. Resource management

## Configuration

The engine can be configured through:
`Engine\assets\engine.cfg`

Settings include window size, fullscreen mode, audio volumes, and debug options.