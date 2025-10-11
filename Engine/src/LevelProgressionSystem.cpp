#include "../include/LevelProgressionSystem.h"
#include "../include/Logger.h"
#include "../include/FileUtils.h"
#include <fstream>
#include <sstream>
#include <algorithm>

namespace Sparky {

    LevelProgressionSystem::LevelProgressionSystem() : 
        currentLevel(""), currentDifficulty(LevelDifficulty::NORMAL) {
        // Initialize player progress
        playerProgress.currentLevel = 0;
        playerProgress.experience = 0;
        playerProgress.totalKills = 0;
        playerProgress.totalDeaths = 0;
        playerProgress.playTime = 0.0f;
        
        SPARKY_LOG_DEBUG("LevelProgressionSystem created");
    }

    LevelProgressionSystem::~LevelProgressionSystem() {
        SPARKY_LOG_DEBUG("LevelProgressionSystem destroyed");
    }

    LevelProgressionSystem& LevelProgressionSystem::getInstance() {
        static LevelProgressionSystem instance;
        return instance;
    }

    void LevelProgressionSystem::addLevel(const LevelInfo& level) {
        // Check if level already exists
        auto it = std::find_if(levels.begin(), levels.end(), 
                              [&level](const LevelInfo& l) { return l.name == level.name; });
        
        if (it == levels.end()) {
            levels.push_back(level);
            SPARKY_LOG_DEBUG("Added level: " + level.name);
        } else {
            SPARKY_LOG_WARNING("Level already exists: " + level.name);
        }
    }

    void LevelProgressionSystem::removeLevel(const std::string& levelName) {
        auto it = std::find_if(levels.begin(), levels.end(), 
                              [&levelName](const LevelInfo& l) { return l.name == levelName; });
        
        if (it != levels.end()) {
            levels.erase(it);
            SPARKY_LOG_DEBUG("Removed level: " + levelName);
        }
    }

    const LevelInfo* LevelProgressionSystem::getLevel(const std::string& levelName) const {
        auto it = std::find_if(levels.begin(), levels.end(), 
                              [&levelName](const LevelInfo& l) { return l.name == levelName; });
        
        if (it != levels.end()) {
            return &(*it);
        }
        return nullptr;
    }

    std::vector<std::string> LevelProgressionSystem::getAvailableLevels() const {
        std::vector<std::string> available;
        
        for (const auto& level : levels) {
            if (canAccessLevel(level.name)) {
                available.push_back(level.name);
            }
        }
        
        return available;
    }

    std::vector<std::string> LevelProgressionSystem::getCompletedLevels() const {
        return playerProgress.completedLevels;
    }

    bool LevelProgressionSystem::canAccessLevel(const std::string& levelName) const {
        const LevelInfo* level = getLevel(levelName);
        if (!level) {
            return false;
        }
        
        // If no required levels, it's accessible
        if (level->requiredLevels.empty()) {
            return true;
        }
        
        // Check if all required levels are completed
        for (const auto& requiredLevel : level->requiredLevels) {
            if (std::find(playerProgress.completedLevels.begin(), 
                         playerProgress.completedLevels.end(), 
                         requiredLevel) == playerProgress.completedLevels.end()) {
                return false;
            }
        }
        
        return true;
    }

    void LevelProgressionSystem::startLevel(const std::string& levelName) {
        if (!canAccessLevel(levelName)) {
            SPARKY_LOG_WARNING("Cannot access level: " + levelName);
            return;
        }
        
        currentLevel = levelName;
        SPARKY_LOG_DEBUG("Starting level: " + levelName);
        
        if (onLevelStartCallback) {
            onLevelStartCallback(levelName);
        }
    }

    void LevelProgressionSystem::completeLevel(const std::string& levelName) {
        // Check if level is already completed
        if (std::find(playerProgress.completedLevels.begin(), 
                     playerProgress.completedLevels.end(), 
                     levelName) != playerProgress.completedLevels.end()) {
            SPARKY_LOG_WARNING("Level already completed: " + levelName);
            return;
        }
        
        // Add to completed levels
        playerProgress.completedLevels.push_back(levelName);
        SPARKY_LOG_DEBUG("Level completed: " + levelName);
        
        // Award experience
        const LevelInfo* level = getLevel(levelName);
        if (level) {
            // Base experience based on difficulty
            int baseExp = 100;
            switch (level->difficulty) {
                case LevelDifficulty::EASY: baseExp = 50; break;
                case LevelDifficulty::NORMAL: baseExp = 100; break;
                case LevelDifficulty::HARD: baseExp = 200; break;
                case LevelDifficulty::NIGHTMARE: baseExp = 500; break;
            }
            addExperience(baseExp);
        }
        
        if (onLevelCompleteCallback) {
            onLevelCompleteCallback(levelName);
        }
        
        if (onProgressUpdateCallback) {
            onProgressUpdateCallback();
        }
    }

    void LevelProgressionSystem::failLevel(const std::string& levelName) {
        SPARKY_LOG_DEBUG("Level failed: " + levelName);
        
        if (onLevelFailCallback) {
            onLevelFailCallback(levelName);
        }
    }

    bool LevelProgressionSystem::isLevelCompleted(const std::string& levelName) const {
        return std::find(playerProgress.completedLevels.begin(), 
                        playerProgress.completedLevels.end(), 
                        levelName) != playerProgress.completedLevels.end();
    }

    void LevelProgressionSystem::addExperience(int exp) {
        if (exp <= 0) return;
        
        playerProgress.experience += exp;
        SPARKY_LOG_DEBUG("Added " + std::to_string(exp) + " experience. Total: " + std::to_string(playerProgress.experience));
        
        if (onProgressUpdateCallback) {
            onProgressUpdateCallback();
        }
    }

    void LevelProgressionSystem::addKill() {
        playerProgress.totalKills++;
        SPARKY_LOG_DEBUG("Kill added. Total: " + std::to_string(playerProgress.totalKills));
    }

    void LevelProgressionSystem::addDeath() {
        playerProgress.totalDeaths++;
        SPARKY_LOG_DEBUG("Death added. Total: " + std::to_string(playerProgress.totalDeaths));
    }

    void LevelProgressionSystem::addPlayTime(float time) {
        if (time <= 0) return;
        
        playerProgress.playTime += time;
        SPARKY_LOG_DEBUG("Play time added: " + std::to_string(time) + " seconds");
    }

    void LevelProgressionSystem::linkQuestToLevel(const std::string& questName, const std::string& levelName) {
        // Add to quest -> levels mapping
        questToLevelMap[questName].push_back(levelName);
        
        // Add to level -> quests mapping
        levelToQuestMap[levelName].push_back(questName);
        
        SPARKY_LOG_DEBUG("Linked quest '" + questName + "' to level '" + levelName + "'");
    }

    std::vector<std::string> LevelProgressionSystem::getQuestsForLevel(const std::string& levelName) const {
        auto it = levelToQuestMap.find(levelName);
        if (it != levelToQuestMap.end()) {
            return it->second;
        }
        return {};
    }

    bool LevelProgressionSystem::saveProgress(const std::string& filepath) {
        SPARKY_LOG_DEBUG("Saving progress to file: " + filepath);
        
        try {
            std::ofstream file(filepath);
            if (!file.is_open()) {
                SPARKY_LOG_ERROR("Failed to open progress file for writing: " + filepath);
                return false;
            }

            // Save player progress
            file << playerProgress.currentLevel << "\n";
            file << playerProgress.experience << "\n";
            file << playerProgress.totalKills << "\n";
            file << playerProgress.totalDeaths << "\n";
            file << playerProgress.playTime << "\n";
            
            // Save completed levels
            file << playerProgress.completedLevels.size() << "\n";
            for (const auto& level : playerProgress.completedLevels) {
                file << level << "\n";
            }
            
            // Save unlocked abilities
            file << playerProgress.unlockedAbilities.size() << "\n";
            for (const auto& ability : playerProgress.unlockedAbilities) {
                file << ability << "\n";
            }
            
            // Save current level
            file << currentLevel << "\n";
            
            // Save difficulty
            file << static_cast<int>(currentDifficulty) << "\n";
            
            file.close();
            SPARKY_LOG_DEBUG("Progress saved successfully");
            return true;
        } catch (const std::exception& e) {
            SPARKY_LOG_ERROR("Failed to save progress: " + std::string(e.what()));
            return false;
        }
    }

    bool LevelProgressionSystem::loadProgress(const std::string& filepath) {
        SPARKY_LOG_DEBUG("Loading progress from file: " + filepath);
        
        try {
            if (!FileUtils::fileExists(filepath)) {
                SPARKY_LOG_WARNING("Progress file does not exist: " + filepath);
                return false;
            }

            std::vector<char> data = FileUtils::readFile(filepath);
            std::string content(data.begin(), data.end());
            
            std::istringstream iss(content);
            std::string line;
            
            // Load player progress
            if (!(iss >> playerProgress.currentLevel) ||
                !(iss >> playerProgress.experience) ||
                !(iss >> playerProgress.totalKills) ||
                !(iss >> playerProgress.totalDeaths) ||
                !(iss >> playerProgress.playTime)) {
                SPARKY_LOG_ERROR("Invalid progress data in file");
                return false;
            }
            
            // Consume the newline character
            iss.ignore();
            
            // Load completed levels
            size_t completedLevelsCount;
            if (!(iss >> completedLevelsCount)) {
                SPARKY_LOG_ERROR("Invalid completed levels count in file");
                return false;
            }
            
            // Consume the newline character
            iss.ignore();
            
            playerProgress.completedLevels.clear();
            for (size_t i = 0; i < completedLevelsCount; i++) {
                std::string levelName;
                if (!std::getline(iss, levelName)) {
                    SPARKY_LOG_ERROR("Invalid completed level name in file");
                    return false;
                }
                playerProgress.completedLevels.push_back(levelName);
            }
            
            // Load unlocked abilities
            size_t unlockedAbilitiesCount;
            if (!(iss >> unlockedAbilitiesCount)) {
                SPARKY_LOG_ERROR("Invalid unlocked abilities count in file");
                return false;
            }
            
            // Consume the newline character
            iss.ignore();
            
            playerProgress.unlockedAbilities.clear();
            for (size_t i = 0; i < unlockedAbilitiesCount; i++) {
                std::string abilityName;
                if (!std::getline(iss, abilityName)) {
                    SPARKY_LOG_ERROR("Invalid unlocked ability name in file");
                    return false;
                }
                playerProgress.unlockedAbilities.push_back(abilityName);
            }
            
            // Load current level
            if (!std::getline(iss, currentLevel)) {
                SPARKY_LOG_ERROR("Invalid current level in file");
                return false;
            }
            
            // Load difficulty
            int difficulty;
            if (!(iss >> difficulty)) {
                SPARKY_LOG_ERROR("Invalid difficulty in file");
                return false;
            }
            currentDifficulty = static_cast<LevelDifficulty>(difficulty);
            
            SPARKY_LOG_DEBUG("Progress loaded successfully");
            return true;
        } catch (const std::exception& e) {
            SPARKY_LOG_ERROR("Failed to load progress: " + std::string(e.what()));
            return false;
        }
    }

    void LevelProgressionSystem::update(float deltaTime) {
        // Add play time
        addPlayTime(deltaTime);
        
        // Update any level-specific logic if needed
        SPARKY_LOG_DEBUG("Updating level progression system");
    }
}