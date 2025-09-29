#include "Level.h"
#include "../../Engine/include/Logger.h"
#include <iostream>

int main() {
    Sparky::Logger::getInstance().setLogLevel(Sparky::LogLevel::DEBUG);
    
    std::cout << "Testing Level JSON parsing...\n";
    
    Sparky::Level level;
    
    // Try to load a level file
    if (level.loadFromFile("../Game/assets/sample_level.json")) {
        std::cout << "Level loaded successfully!\n";
        std::cout << "Level name: " << level.getName() << "\n";
        std::cout << "Level description: " << level.getDescription() << "\n";
        std::cout << "Number of objects: " << level.getLevelObjects().size() << "\n";
        
        // Print object information
        for (size_t i = 0; i < level.getLevelObjects().size(); ++i) {
            const auto& obj = level.getLevelObjects()[i];
            std::cout << "Object " << i << ": " << obj.type << " - " << obj.name << "\n";
            std::cout << "  Position: (" << obj.position.x << ", " << obj.position.y << ", " << obj.position.z << ")\n";
            std::cout << "  Rotation: (" << obj.rotation.x << ", " << obj.rotation.y << ", " << obj.rotation.z << ")\n";
            std::cout << "  Scale: (" << obj.scale.x << ", " << obj.scale.y << ", " << obj.scale.z << ")\n";
        }
        
        // Try to save the level to a new file
        if (level.saveToFile("../Game/assets/sample_level_copy.json")) {
            std::cout << "Level saved successfully!\n";
        } else {
            std::cout << "Failed to save level!\n";
        }
        
        // Try to spawn objects
        level.spawnObjects();
        std::cout << "Spawned " << level.getSpawnedObjects().size() << " objects\n";
    } else {
        std::cout << "Failed to load level!\n";
        return -1;
    }
    
    std::cout << "Test completed successfully!\n";
    return 0;
}