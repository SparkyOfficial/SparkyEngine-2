#include "../include/SaveGameManager.h"
#include "../include/Logger.h"
#include "../include/Config.h"

int main() {
    Sparky::Logger::getInstance().setLogLevel(Sparky::LogLevel::DEBUG);
    
    // Test save game functionality
    Sparky::SaveGameManager& saveManager = Sparky::SaveGameManager::getInstance();
    
    // Create some test data
    Sparky::GameSaveData testData = {};
    testData.playerHealth = 100.0f;
    testData.playerMaxHealth = 100.0f;
    testData.playerPosition[0] = 10.0f;
    testData.playerPosition[1] = 5.0f;
    testData.playerPosition[2] = 0.0f;
    testData.currentLevel = 2;
    testData.score = 1500;
    testData.playTime = 120.5f;
    testData.masterVolume = 0.8f;
    testData.musicVolume = 0.6f;
    testData.sfxVolume = 0.9f;
    testData.fullscreen = true;
    testData.resolutionWidth = 1920;
    testData.resolutionHeight = 1080;
    
    // Add some inventory items
    testData.inventoryItems.push_back("HealthPack");
    testData.inventoryItems.push_back("Ammo");
    testData.inventoryQuantities.push_back(3);
    testData.inventoryQuantities.push_back(50);
    
    // Save the data
    bool saved = saveManager.saveGame("test_save", testData);
    if (saved) {
        Sparky::Logger::getInstance().info("Test save created successfully!");
    } else {
        Sparky::Logger::getInstance().error("Failed to create test save!");
        return 1;
    }
    
    // Load the data
    Sparky::GameSaveData loadedData;
    bool loaded = saveManager.loadGame("test_save", loadedData);
    if (loaded) {
        Sparky::Logger::getInstance().info("Test save loaded successfully!");
        
        // Verify the data
        if (loadedData.playerHealth == testData.playerHealth &&
            loadedData.playerMaxHealth == testData.playerMaxHealth &&
            loadedData.currentLevel == testData.currentLevel &&
            loadedData.score == testData.score) {
            Sparky::Logger::getInstance().info("Save data verification passed!");
        } else {
            Sparky::Logger::getInstance().error("Save data verification failed!");
            return 1;
        }
    } else {
        Sparky::Logger::getInstance().error("Failed to load test save!");
        return 1;
    }
    
    // Test settings save/load
    Sparky::Config& config = Sparky::Config::getInstance();
    config.setFloat("audio.masterVolume", 0.7f);
    config.setBool("video.fullscreen", false);
    config.setString("player.name", "TestPlayer");
    
    saveManager.saveSettings();
    Sparky::Logger::getInstance().info("Settings saved!");
    
    // Clear config and reload
    // Note: In a real implementation, we would have a method to clear the config
    // For now, we'll just load it again to verify it was saved
    saveManager.loadSettings();
    Sparky::Logger::getInstance().info("Settings loaded!");
    
    float masterVolume = config.getFloat("audio.masterVolume", 1.0f);
    bool fullscreen = config.getBool("video.fullscreen", true);
    std::string playerName = config.getString("player.name", "");
    
    Sparky::Logger::getInstance().info("Loaded settings - Master Volume: " + std::to_string(masterVolume) + 
                                      ", Fullscreen: " + (fullscreen ? "true" : "false") + 
                                      ", Player Name: " + playerName);
    
    // List save files
    std::vector<std::string> saveFiles = saveManager.getSaveFiles();
    Sparky::Logger::getInstance().info("Found " + std::to_string(saveFiles.size()) + " save files:");
    for (const auto& saveFile : saveFiles) {
        Sparky::Logger::getInstance().info("  - " + saveFile);
    }
    
    Sparky::Logger::getInstance().info("Save/Load test completed successfully!");
    return 0;
}