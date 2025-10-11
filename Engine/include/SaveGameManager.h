#pragma once

#include <string>
#include <vector>
#include <memory>

namespace Sparky {
    // Forward declarations
    class Player;
    class Level;
    class Inventory;
    
    struct GameSaveData {
        // Player data
        float playerHealth;
        float playerMaxHealth;
        float playerPosition[3]; // x, y, z
        float playerRotation[3]; // x, y, z
        
        // Game state
        int currentLevel;
        int score;
        float playTime;
        
        // Inventory data
        std::vector<std::string> inventoryItems;
        std::vector<int> inventoryQuantities;
        
        // Game settings
        float masterVolume;
        float musicVolume;
        float sfxVolume;
        bool fullscreen;
        int resolutionWidth;
        int resolutionHeight;
    };
    
    class SaveGameManager {
    public:
        static SaveGameManager& getInstance();
        
        // Save game methods
        bool saveGame(const std::string& saveName, Player* player, Level* level, Inventory* inventory);
        bool saveGame(const std::string& saveName, const GameSaveData& data);
        
        // Load game methods
        bool loadGame(const std::string& saveName, Player* player, Level* level, Inventory* inventory);
        bool loadGame(const std::string& saveName, GameSaveData& data);
        
        // Save/Load settings
        bool saveSettings();
        bool loadSettings();
        
        // Utility methods
        std::vector<std::string> getSaveFiles() const;
        bool deleteSave(const std::string& saveName);
        std::string getSaveDirectory() const;
        
        // Getters and setters
        const std::string& getLastSave() const { return lastSave; }
        void setAutoSave(bool autoSave) { this->autoSave = autoSave; }
        bool getAutoSave() const { return autoSave; }
        
    private:
        SaveGameManager();
        ~SaveGameManager();
        
        // Helper methods
        std::string getSaveFilePath(const std::string& saveName) const;
        bool serializeSaveData(const GameSaveData& data, std::vector<char>& buffer) const;
        bool deserializeSaveData(const std::vector<char>& buffer, GameSaveData& data) const;
        
        std::string saveDirectory;
        std::string lastSave;
        bool autoSave;
    };
}