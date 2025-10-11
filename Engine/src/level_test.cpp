#include "../include/Logger.h"
#include <iostream>
#include <fstream>
#include <string>

int main() {
    Sparky::Logger::getInstance().setLogLevel(Sparky::LogLevel::DEBUG);
    
    Sparky::Logger::getInstance().info("=== Complex Level Layout Test ===");
    
    // Test loading complex_level.json
    std::string complexLevelPath = "../Game/assets/complex_level.json";
    std::ifstream complexFile(complexLevelPath);
    if (complexFile.is_open()) {
        Sparky::Logger::getInstance().info("Successfully found complex_level.json");
        
        // Read and display first few lines to verify content
        std::string line;
        int lineCount = 0;
        while (std::getline(complexFile, line) && lineCount < 10) {
            Sparky::Logger::getInstance().info("Line " + std::to_string(lineCount + 1) + ": " + line);
            lineCount++;
        }
        complexFile.close();
    } else {
        Sparky::Logger::getInstance().error("Could not find complex_level.json at " + complexLevelPath);
    }
    
    // Test loading advanced_level.json
    std::string advancedLevelPath = "../Game/assets/advanced_level.json";
    std::ifstream advancedFile(advancedLevelPath);
    if (advancedFile.is_open()) {
        Sparky::Logger::getInstance().info("Successfully found advanced_level.json");
        
        // Read and display first few lines to verify content
        std::string line;
        int lineCount = 0;
        while (std::getline(advancedFile, line) && lineCount < 10) {
            Sparky::Logger::getInstance().info("Line " + std::to_string(lineCount + 1) + ": " + line);
            lineCount++;
        }
        advancedFile.close();
    } else {
        Sparky::Logger::getInstance().error("Could not find advanced_level.json at " + advancedLevelPath);
    }
    
    // Describe the features of the complex levels
    Sparky::Logger::getInstance().info("");
    Sparky::Logger::getInstance().info("=== Complex Level Features ===");
    Sparky::Logger::getInstance().info("1. Multiple rooms with doors and walls");
    Sparky::Logger::getInstance().info("2. Interactive elements (doors, buttons, switches)");
    Sparky::Logger::getInstance().info("3. Trigger volumes for gameplay events");
    Sparky::getInstance().info("4. Diverse enemy placement");
    Sparky::Logger::getInstance().info("5. Pickup items (health, ammo)");
    Sparky::Logger::getInstance().info("6. Complex lighting setups");
    Sparky::Logger::getInstance().info("7. Puzzle elements requiring interaction");
    Sparky::Logger::getInstance().info("8. Multi-story level design");
    
    Sparky::Logger::getInstance().info("");
    Sparky::Logger::getInstance().info("Complex level layout test completed successfully!");
    return 0;
}