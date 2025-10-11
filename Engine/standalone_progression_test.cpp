#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>
#include <sstream>

// Simplified standalone test for level progression system concepts

enum class LevelDifficulty {
    EASY,
    NORMAL,
    HARD,
    NIGHTMARE
};

struct LevelInfo {
    std::string name;
    std::string description;
    int levelNumber;
    LevelDifficulty difficulty;
    std::string sceneFile;
    std::vector<std::string> requiredLevels;
};

struct PlayerProgress {
    int experience;
    int totalKills;
    int totalDeaths;
    float playTime;
    std::vector<std::string> completedLevels;
};

class LevelProgressionSystem {
private:
    std::vector<LevelInfo> levels;
    std::string currentLevel;
    PlayerProgress playerProgress;
    std::map<std::string, std::vector<std::string>> levelToQuestMap;

public:
    LevelProgressionSystem() : currentLevel(""), playerProgress{0, 0, 0, 0.0f} {}
    
    void addLevel(const LevelInfo& level) {
        auto it = std::find_if(levels.begin(), levels.end(), 
                              [&level](const LevelInfo& l) { return l.name == level.name; });
        
        if (it == levels.end()) {
            levels.push_back(level);
            std::cout << "Added level: " << level.name << "\n";
        }
    }
    
    bool canAccessLevel(const std::string& levelName) const {
        auto it = std::find_if(levels.begin(), levels.end(), 
                              [&levelName](const LevelInfo& l) { return l.name == levelName; });
        
        if (it == levels.end()) {
            return false;
        }
        
        if (it->requiredLevels.empty()) {
            return true;
        }
        
        for (const auto& requiredLevel : it->requiredLevels) {
            if (std::find(playerProgress.completedLevels.begin(), 
                         playerProgress.completedLevels.end(), 
                         requiredLevel) == playerProgress.completedLevels.end()) {
                return false;
            }
        }
        
        return true;
    }
    
    void startLevel(const std::string& levelName) {
        if (!canAccessLevel(levelName)) {
            std::cout << "Cannot access level: " << levelName << "\n";
            return;
        }
        
        currentLevel = levelName;
        std::cout << "Starting level: " << levelName << "\n";
    }
    
    void completeLevel(const std::string& levelName) {
        if (std::find(playerProgress.completedLevels.begin(), 
                     playerProgress.completedLevels.end(), 
                     levelName) != playerProgress.completedLevels.end()) {
            std::cout << "Level already completed: " << levelName << "\n";
            return;
        }
        
        playerProgress.completedLevels.push_back(levelName);
        std::cout << "Level completed: " << levelName << "\n";
        
        // Award experience
        auto it = std::find_if(levels.begin(), levels.end(), 
                              [&levelName](const LevelInfo& l) { return l.name == levelName; });
        if (it != levels.end()) {
            int baseExp = 100;
            switch (it->difficulty) {
                case LevelDifficulty::EASY: baseExp = 50; break;
                case LevelDifficulty::NORMAL: baseExp = 100; break;
                case LevelDifficulty::HARD: baseExp = 200; break;
                case LevelDifficulty::NIGHTMARE: baseExp = 500; break;
            }
            playerProgress.experience += baseExp;
            std::cout << "Awarded " << baseExp << " experience. Total: " << playerProgress.experience << "\n";
        }
    }
    
    void addKill() {
        playerProgress.totalKills++;
        std::cout << "Kill added. Total: " << playerProgress.totalKills << "\n";
    }
    
    void addDeath() {
        playerProgress.totalDeaths++;
        std::cout << "Death added. Total: " << playerProgress.totalDeaths << "\n";
    }
    
    std::vector<std::string> getAvailableLevels() const {
        std::vector<std::string> available;
        
        for (const auto& level : levels) {
            if (canAccessLevel(level.name)) {
                available.push_back(level.name);
            }
        }
        
        return available;
    }
    
    const PlayerProgress& getPlayerProgress() const {
        return playerProgress;
    }
    
    bool saveProgress(const std::string& filepath) {
        std::ofstream file(filepath);
        if (!file.is_open()) {
            return false;
        }

        file << playerProgress.experience << "\n";
        file << playerProgress.totalKills << "\n";
        file << playerProgress.totalDeaths << "\n";
        file << playerProgress.playTime << "\n";
        
        file << playerProgress.completedLevels.size() << "\n";
        for (const auto& level : playerProgress.completedLevels) {
            file << level << "\n";
        }
        
        file.close();
        return true;
    }
    
    bool loadProgress(const std::string& filepath) {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            return false;
        }

        std::string line;
        
        if (!(file >> playerProgress.experience) ||
            !(file >> playerProgress.totalKills) ||
            !(file >> playerProgress.totalDeaths) ||
            !(file >> playerProgress.playTime)) {
            return false;
        }
        
        file.ignore();
        
        size_t completedLevelsCount;
        if (!(file >> completedLevelsCount)) {
            return false;
        }
        
        file.ignore();
        
        playerProgress.completedLevels.clear();
        for (size_t i = 0; i < completedLevelsCount; i++) {
            std::string levelName;
            if (!std::getline(file, levelName)) {
                return false;
            }
            playerProgress.completedLevels.push_back(levelName);
        }
        
        file.close();
        return true;
    }
};

int main() {
    std::cout << "Standalone Level Progression System Test\n";
    std::cout << "======================================\n\n";
    
    LevelProgressionSystem levelSystem;
    
    // Create levels
    LevelInfo tutorial;
    tutorial.name = "Tutorial";
    tutorial.description = "Learn the basics";
    tutorial.levelNumber = 1;
    tutorial.difficulty = LevelDifficulty::EASY;
    tutorial.sceneFile = "levels/tutorial.json";
    
    levelSystem.addLevel(tutorial);
    
    LevelInfo facility;
    facility.name = "Black Mesa Facility";
    facility.description = "Investigate the facility";
    facility.levelNumber = 2;
    facility.difficulty = LevelDifficulty::NORMAL;
    facility.sceneFile = "levels/facility.json";
    facility.requiredLevels = {"Tutorial"};
    
    levelSystem.addLevel(facility);
    
    LevelInfo escape;
    escape.name = "Surface Escape";
    escape.description = "Escape to the surface";
    escape.levelNumber = 3;
    escape.difficulty = LevelDifficulty::HARD;
    escape.sceneFile = "levels/escape.json";
    escape.requiredLevels = {"Black Mesa Facility"};
    
    levelSystem.addLevel(escape);
    
    std::cout << "\n--- Testing Level Access ---\n";
    std::cout << "Can access Tutorial: " << (levelSystem.canAccessLevel("Tutorial") ? "Yes" : "No") << "\n";
    std::cout << "Can access Facility: " << (levelSystem.canAccessLevel("Black Mesa Facility") ? "Yes" : "No") << "\n";
    std::cout << "Can access Escape: " << (levelSystem.canAccessLevel("Surface Escape") ? "Yes" : "No") << "\n";
    
    std::cout << "\n--- Available Levels ---\n";
    auto available = levelSystem.getAvailableLevels();
    for (const auto& level : available) {
        std::cout << "- " << level << "\n";
    }
    
    std::cout << "\n--- Completing Tutorial ---\n";
    levelSystem.startLevel("Tutorial");
    levelSystem.completeLevel("Tutorial");
    levelSystem.addKill();
    levelSystem.addKill();
    
    std::cout << "\n--- Testing Level Access After Tutorial ---\n";
    std::cout << "Can access Tutorial: " << (levelSystem.canAccessLevel("Tutorial") ? "Yes" : "No") << "\n";
    std::cout << "Can access Facility: " << (levelSystem.canAccessLevel("Black Mesa Facility") ? "Yes" : "No") << "\n";
    std::cout << "Can access Escape: " << (levelSystem.canAccessLevel("Surface Escape") ? "Yes" : "No") << "\n";
    
    std::cout << "\n--- Available Levels After Tutorial ---\n";
    available = levelSystem.getAvailableLevels();
    for (const auto& level : available) {
        std::cout << "- " << level << "\n";
    }
    
    std::cout << "\n--- Completing Facility ---\n";
    levelSystem.startLevel("Black Mesa Facility");
    levelSystem.completeLevel("Black Mesa Facility");
    levelSystem.addKill();
    levelSystem.addKill();
    levelSystem.addDeath();
    
    std::cout << "\n--- Completing Escape ---\n";
    levelSystem.startLevel("Surface Escape");
    levelSystem.completeLevel("Surface Escape");
    levelSystem.addKill();
    
    const auto& progress = levelSystem.getPlayerProgress();
    std::cout << "\n--- Final Player Stats ---\n";
    std::cout << "Experience: " << progress.experience << "\n";
    std::cout << "Kills: " << progress.totalKills << "\n";
    std::cout << "Deaths: " << progress.totalDeaths << "\n";
    std::cout << "Play Time: " << progress.playTime << " seconds\n";
    
    std::cout << "\nCompleted Levels:\n";
    for (const auto& level : progress.completedLevels) {
        std::cout << "- " << level << "\n";
    }
    
    std::cout << "\n--- Testing Save/Load ---\n";
    bool saved = levelSystem.saveProgress("test_progress.txt");
    std::cout << "Progress saved: " << (saved ? "Yes" : "No") << "\n";
    
    LevelProgressionSystem levelSystem2;
    bool loaded = levelSystem2.loadProgress("test_progress.txt");
    std::cout << "Progress loaded: " << (loaded ? "Yes" : "No") << "\n";
    
    if (loaded) {
        const auto& progress2 = levelSystem2.getPlayerProgress();
        std::cout << "Loaded Experience: " << progress2.experience << "\n";
        std::cout << "Loaded Kills: " << progress2.totalKills << "\n";
        std::cout << "Loaded Deaths: " << progress2.totalDeaths << "\n";
    }
    
    std::cout << "\nStandalone level progression test completed successfully!\n";
    return 0;
}