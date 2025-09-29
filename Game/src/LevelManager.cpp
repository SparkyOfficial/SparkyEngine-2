#include "LevelManager.h"
#include "Level.h"
#include "../../Engine/include/Logger.h"
#include "../../Engine/include/FileUtils.h"
#include <algorithm>

namespace Sparky {
    
    LevelManager& LevelManager::getInstance() {
        static LevelManager instance;
        return instance;
    }
    
    LevelManager::LevelManager() : currentLevel(nullptr), loading(false) {
        // Add default level paths
        levelPaths.push_back("../Game/assets/");
        levelPaths.push_back("./assets/");
        SPARKY_LOG_DEBUG("LevelManager created");
    }
    
    LevelManager::~LevelManager() {
        SPARKY_LOG_DEBUG("LevelManager destroyed");
    }
    
    bool LevelManager::loadLevel(const std::string& levelName, const std::string& filepath) {
        SPARKY_LOG_INFO("Loading level: " + levelName + " from " + filepath);
        
        loading = true;
        
        // Create a new level
        auto level = std::make_unique<Level>();
        
        // Try to load the level
        if (!level->loadFromFile(filepath)) {
            SPARKY_LOG_ERROR("Failed to load level: " + levelName);
            loading = false;
            return false;
        }
        
        // Store the level
        levels[levelName] = std::move(level);
        levelNames.push_back(levelName);
        
        SPARKY_LOG_INFO("Level loaded successfully: " + levelName);
        loading = false;
        return true;
    }
    
    bool LevelManager::unloadLevel(const std::string& levelName) {
        auto it = levels.find(levelName);
        if (it == levels.end()) {
            SPARKY_LOG_WARNING("Attempted to unload non-existent level: " + levelName);
            return false;
        }
        
        // If this is the current level, clear it
        if (currentLevel == it->second.get()) {
            currentLevel = nullptr;
        }
        
        // Remove from collections
        levels.erase(it);
        levelNames.erase(std::remove(levelNames.begin(), levelNames.end(), levelName), levelNames.end());
        
        SPARKY_LOG_INFO("Level unloaded: " + levelName);
        return true;
    }
    
    bool LevelManager::setCurrentLevel(const std::string& levelName) {
        auto it = levels.find(levelName);
        if (it == levels.end()) {
            SPARKY_LOG_ERROR("Attempted to set non-existent level as current: " + levelName);
            return false;
        }
        
        currentLevel = it->second.get();
        SPARKY_LOG_INFO("Current level set to: " + levelName);
        return true;
    }
    
    Level* LevelManager::getCurrentLevel() const {
        return currentLevel;
    }
    
    Level* LevelManager::getLevel(const std::string& levelName) const {
        auto it = levels.find(levelName);
        if (it == levels.end()) {
            return nullptr;
        }
        return it->second.get();
    }
    
    void LevelManager::transitionToLevel(const std::string& levelName) {
        SPARKY_LOG_INFO("Transitioning to level: " + levelName);
        
        // In a full implementation, we would:
        // 1. Fade out current level
        // 2. Unload current level (optional)
        // 3. Load new level
        // 4. Set new level as current
        // 5. Fade in new level
        // 6. Initialize level objects
        
        // For now, we'll just set the current level
        setCurrentLevel(levelName);
    }
    
    std::string LevelManager::findLevelFile(const std::string& levelName) const {
        // Try to find the level file in the registered paths
        for (const auto& path : levelPaths) {
            std::string fullPath = path + levelName + ".json";
            if (FileUtils::fileExists(fullPath)) {
                return fullPath;
            }
        }
        
        // If not found, return empty string
        return "";
    }
}