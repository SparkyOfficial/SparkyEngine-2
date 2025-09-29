#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <vector>

namespace Sparky {
    class Level;
    
    class LevelManager {
    public:
        static LevelManager& getInstance();
        
        bool loadLevel(const std::string& levelName, const std::string& filepath);
        bool unloadLevel(const std::string& levelName);
        bool setCurrentLevel(const std::string& levelName);
        Level* getCurrentLevel() const;
        Level* getLevel(const std::string& levelName) const;
        
        const std::vector<std::string>& getAvailableLevels() const { return levelNames; }
        void addLevelPath(const std::string& path) { levelPaths.push_back(path); }
        
        // Level transition methods
        void transitionToLevel(const std::string& levelName);
        bool isLoading() const { return loading; }
        
    private:
        LevelManager();
        ~LevelManager();
        
        std::unordered_map<std::string, std::unique_ptr<Level>> levels;
        std::vector<std::string> levelNames;
        std::vector<std::string> levelPaths;
        Level* currentLevel;
        bool loading;
        
        // Private helper methods
        std::string findLevelFile(const std::string& levelName) const;
    };
}