# Sparky Engine 2 Documentation

## Project Structure

```
SparkyEngine2/
├── Engine/                 # Engine core library
│   ├── include/           # Header files
│   ├── src/               # Source files
│   ├── shaders/           # Shader files
│   └── assets/            # Engine assets
├── Game/                  # Game executable
│   ├── src/               # Game source files
│   └── assets/            # Game assets
├── build/                 # Build directory (created during build)
├── CMakeLists.txt         # Main CMake configuration
├── build.bat              # Windows build script
├── run.bat                # Windows run script
└── README.md              # Project README
```

## Engine Components

### Core Systems
- **VulkanRenderer**: Vulkan-based rendering system
- **WindowManager**: Window and context management
- **InputManager**: Input handling system
- **Camera**: First-person camera system
- **Logger**: Logging system with multiple levels
- **Config**: Configuration system
- **Profiler**: Performance profiling utilities
- **EventSystem**: Event-driven architecture
- **StateMachine**: State management system

### Graphics
- **Material**: Material system with properties and textures
- **ShaderProgram**: Shader management
- **Light**: Lighting system (Directional, Point, Spot lights)
- **Mesh**: 3D mesh representation
- **MeshRenderer**: Mesh rendering utilities

### Game Objects
- **GameObject**: Base game object class
- **Component**: Base component class
- **RenderComponent**: Rendering component
- **PhysicsComponent**: Physics component
- **ScriptComponent**: Scriptable component
- **CharacterController**: Character movement controller

### Game Systems
- **Inventory**: Item inventory system
- **Quest**: Quest and objective system
- **ParticleSystem**: Particle effects system
- **Animation**: Animation system
- **BehaviorTree**: AI behavior tree system

### UI
- **GUIElement**: Base GUI element class
- **Button**: Interactive button element
- **GUIManager**: GUI management system

### Audio
- **AudioEngine**: Audio playback system
- **AudioBuffer**: Audio data buffer
- **AudioSource**: Audio source playback

## Building the Project

### Prerequisites
- C++17 compatible compiler
- CMake 3.20 or higher
- Vulkan SDK
- GLFW library
- GLM mathematics library

### Windows
```cmd
build.bat
```

### Linux/macOS
```bash
mkdir build
cd build
cmake ..
make
```

## Running the Game

### Windows
```cmd
run.bat
```

### Linux/macOS
```bash
cd build/Game
./SparkyGame
```

## Code Examples

### Creating a GameObject
```cpp
auto player = std::make_unique<GameObject>();
player->setName("Player");
player->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
```

### Adding a Component
```cpp
auto renderComponent = std::make_unique<RenderComponent>();
player->addComponent(std::move(renderComponent));
```

### Creating a Quest
```cpp
auto quest = std::make_unique<Quest>("MainQuest", "Defeat the enemy");
auto objective = std::make_unique<QuestObjective>("Defeat the enemy", 1);
quest->addObjective(std::move(objective));
```

### Setting up a Behavior Tree
```cpp
auto tree = std::make_unique<BehaviorTree>();
auto sequence = std::make_unique<Sequence>("Patrol");
auto move = std::make_unique<MoveToTarget>(owner, target);
sequence->addChild(std::move(move));
tree->setRoot(std::move(sequence));
```

## Key Features

1. **Modern C++17**: Uses modern C++ features for clean, efficient code
2. **Vulkan Rendering**: High-performance graphics using Vulkan API
3. **Component-Based Architecture**: Flexible entity-component system
4. **Physics System**: Basic physics and collision detection
5. **Audio System**: 3D audio playback capabilities
6. **GUI System**: Basic UI framework
7. **Animation System**: Keyframe-based animation
8. **AI System**: Behavior tree-based AI
9. **Quest System**: Flexible quest and objective tracking
10. **Inventory System**: Item management
11. **Particle System**: Visual effects
12. **Lighting System**: Multiple light types with attenuation
13. **Material System**: Physically-based materials
14. **Configuration System**: Flexible configuration management
15. **Logging System**: Multi-level logging
16. **Profiling System**: Performance profiling utilities

## Extending the Engine

### Adding a New Component
1. Create a new header file in `Engine/include/`
2. Create a new source file in `Engine/src/`
3. Inherit from `Component` class
4. Implement `update()` and `render()` methods
5. Add to CMakeLists.txt

### Adding a New System
1. Create system header and source files
2. Implement singleton pattern if needed
3. Add to CMakeLists.txt
4. Integrate with main engine loop

### Adding New Assets
1. Place assets in appropriate `assets/` directory
2. Update asset loading code
3. Add support in ResourceManager if needed