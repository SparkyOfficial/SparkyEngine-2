#pragma once

#include "QuestManager.h"
#include "Quest.h"
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <map>

namespace Sparky {
    
    // Level completion conditions
    enum class LevelCompletionCondition {
        ALL_OBJECTIVES,
        MAIN_QUEST_COMPLETED,
        TIME_LIMIT,
        SURVIVAL
    };
    
    // Level difficulty settings
    enum class LevelDifficulty {
        EASY,
        NORMAL,
        HARD,
        NIGHTMARE
    };
    
    // Level information
    struct LevelInfo {
        std::string name;
        std::string description;
        int levelNumber;
        LevelDifficulty difficulty;
        std::string sceneFile; // Path to level scene file
        std::vector<std::string> requiredLevels; // Levels that must be completed first
        LevelCompletionCondition completionCondition;
        std::vector<std::string> rewards; // Rewards for completing the level
    };
    
    // Player progression data
    struct PlayerProgress {
        int currentLevel;
        int experience;
        int totalKills;
        int totalDeaths;
        float playTime; // Total play time in seconds
        std::vector<std::string> completedLevels;
        std::vector<std::string> unlockedAbilities;
    };
    
    class LevelProgressionSystem {
    public:
        static LevelProgressionSystem& getInstance();
        
        // Level management
        void addLevel(const LevelInfo& level);
        void removeLevel(const std::string& levelName);
        const LevelInfo* getLevel(const std::string& levelName) const;
        std::vector<std::string> getAvailableLevels() const;
        std::vector<std::string> getCompletedLevels() const;
        
        // Level progression
        bool canAccessLevel(const std::string& levelName) const;
        void startLevel(const std::string& levelName);
        void completeLevel(const std::string& levelName);
        void failLevel(const std::string& levelName);
        bool isLevelCompleted(const std::string& levelName) const;
        
        // Current level management
        const std::string& getCurrentLevel() const { return currentLevel; }
        void setCurrentLevel(const std::string& levelName) { currentLevel = levelName; }
        
        // Player progression
        void addExperience(int exp);
        void addKill();
        void addDeath();
        void addPlayTime(float time);
        const PlayerProgress& getPlayerProgress() const { return playerProgress; }
        void setPlayerProgress(const PlayerProgress& progress) { playerProgress = progress; }
        
        // Difficulty settings
        void setDifficulty(LevelDifficulty difficulty) { currentDifficulty = difficulty; }
        LevelDifficulty getDifficulty() const { return currentDifficulty; }
        
        // Quest integration
        void linkQuestToLevel(const std::string& questName, const std::string& levelName);
        std::vector<std::string> getQuestsForLevel(const std::string& levelName) const;
        
        // Save/Load
        bool saveProgress(const std::string& filepath);
        bool loadProgress(const std::string& filepath);
        
        // Events
        void setOnLevelStartCallback(std::function<void(const std::string&)> callback) { onLevelStartCallback = callback; }
        void setOnLevelCompleteCallback(std::function<void(const std::string&)> callback) { onLevelCompleteCallback = callback; }
        void setOnLevelFailCallback(std::function<void(const std::string&)> callback) { onLevelFailCallback = callback; }
        void setOnProgressUpdateCallback(std::function<void()> callback) { onProgressUpdateCallback = callback; }
        
        // Update
        void update(float deltaTime);

    private:
        LevelProgressionSystem();
        ~LevelProgressionSystem();
        
        // Level data
        std::vector<LevelInfo> levels;
        std::string currentLevel;
        LevelDifficulty currentDifficulty;
        
        // Player progression
        PlayerProgress playerProgress;
        
        // Quest to level mapping
        std::map<std::string, std::vector<std::string>> questToLevelMap; // quest -> levels
        std::map<std::string, std::vector<std::string>> levelToQuestMap; // level -> quests
        
        // Callbacks
        std::function<void(const std::string&)> onLevelStartCallback;
        std::function<void(const std::string&)> onLevelCompleteCallback;
        std::function<void(const std::string&)> onLevelFailCallback;
        std::function<void()> onProgressUpdateCallback;
    };
}