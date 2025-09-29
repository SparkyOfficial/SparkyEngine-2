#include "Level.h"
#include "../../Engine/include/Logger.h"
#include <iostream>

int main() {
    // Initialize logger
    Sparky::Logger::getInstance().setLogLevel(Sparky::LogLevel::DEBUG);
    
    try {
        // Create a level and load from JSON file
        Sparky::Level level("TestLevel");
        
        std::cout << "Loading level from JSON file..." << std::endl;
        level.loadLevelFromFile("assets/sample_level.json");
        
        std::cout << "Level loaded successfully!" << std::endl;
        std::cout << "Number of platforms: " << level.getPlatforms().size() << std::endl;
        std::cout << "Number of guns: " << level.getGuns().size() << std::endl;
        
        // Clean up
        level.unloadLevel();
        
        std::cout << "Test completed successfully!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
}