#include "../include/SaveGameManager.h"
#include "../include/Config.h"
#include "../include/FileUtils.h"
#include "../include/Logger.h"
#include "../include/Player.h"
#include "../include/HealthComponent.h"
#include "../include/Inventory.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <filesystem>

#ifdef _WIN32
#include <windows.h>
#include <shlobj.h>
#endif

namespace Sparky {

    SaveGameManager& SaveGameManager::getInstance() {
        static SaveGameManager instance;
        return instance;
    }
    
    SaveGameManager::SaveGameManager() : autoSave(false) {
        // Set default save directory
#ifdef _WIN32
        // Get user's Documents folder
        PWSTR documentsPath = nullptr;
        if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Documents, 0, nullptr, &documentsPath))) {
            std::wstring wPath(documentsPath);
            CoTaskMemFree(documentsPath);
            
            // Convert wide string to UTF-8
            int size_needed = WideCharToMultiByte(CP_UTF8, 0, wPath.c_str(), (int)wPath.size(), NULL, 0, NULL, NULL);
            if (size_needed > 0) {
                std::string strTo(size_needed, 0);
                WideCharToMultiByte(CP_UTF8, 0, wPath.c_str(), (int)wPath.size(), &strTo[0], size_needed, NULL, NULL);
                saveDirectory = strTo + "/SparkyEngine/Saves";
            } else {
                saveDirectory = "./Saves";
            }
        } else {
            saveDirectory = "./Saves";
        }
#else
        // For non-Windows, use a simple path
        saveDirectory = "./Saves";
#endif
        
        // Create save directory if it doesn't exist
        std::filesystem::create_directories(saveDirectory);
        
        SPARKY_LOG_INFO("Save directory set to: " + saveDirectory);
    }
    
    SaveGameManager::~SaveGameManager() {
    }
    
    bool SaveGameManager::saveGame(const std::string& saveName, Player* player, Level* level, Inventory* inventory) {
        if (!player) {
            SPARKY_LOG_ERROR("Cannot save game: Player is null");
            return false;
        }
        
        GameSaveData data = {};
        
        // Save player data
        HealthComponent* healthComp = player->getHealthComponent();
        if (healthComp) {
            data.playerHealth = healthComp->getHealth();
            data.playerMaxHealth = healthComp->getMaxHealth();
        }
        
        // Note: In a full implementation, we would save player position and rotation
        // For now, we'll just initialize to zero
        data.playerPosition[0] = 0.0f;
        data.playerPosition[1] = 0.0f;
        data.playerPosition[2] = 0.0f;
        data.playerRotation[0] = 0.0f;
        data.playerRotation[1] = 0.0f;
        data.playerRotation[2] = 0.0f;
        
        // Save inventory data
        if (inventory) {
            // In a full implementation, we would iterate through inventory items
            // For now, we'll leave the vectors empty
        }
        
        // Save game state
        data.currentLevel = 1; // Default level
        data.score = 0; // Default score
        data.playTime = 0.0f; // Default play time
        
        // Save settings
        Config& config = Config::getInstance();
        data.masterVolume = config.getFloat("audio.masterVolume", 1.0f);
        data.musicVolume = config.getFloat("audio.musicVolume", 1.0f);
        data.sfxVolume = config.getFloat("audio.sfxVolume", 1.0f);
        data.fullscreen = config.getBool("video.fullscreen", false);
        data.resolutionWidth = config.getInt("video.resolutionWidth", 1920);
        data.resolutionHeight = config.getInt("video.resolutionHeight", 1080);
        
        // Save the data
        bool result = saveGame(saveName, data);
        if (result) {
            lastSave = saveName;
            SPARKY_LOG_INFO("Game saved successfully: " + saveName);
        } else {
            SPARKY_LOG_ERROR("Failed to save game: " + saveName);
        }
        
        return result;
    }
    
    bool SaveGameManager::saveGame(const std::string& saveName, const GameSaveData& data) {
        // Serialize the data
        std::vector<char> buffer;
        if (!serializeSaveData(data, buffer)) {
            SPARKY_LOG_ERROR("Failed to serialize save data");
            return false;
        }
        
        // Write to file
        std::string savePath = getSaveFilePath(saveName);
        bool result = FileUtils::writeFile(savePath, buffer);
        
        if (result) {
            lastSave = saveName;
            SPARKY_LOG_INFO("Game saved successfully to: " + savePath);
        } else {
            SPARKY_LOG_ERROR("Failed to write save file: " + savePath);
        }
        
        return result;
    }
    
    bool SaveGameManager::loadGame(const std::string& saveName, Player* player, Level* level, Inventory* inventory) {
        if (!player) {
            SPARKY_LOG_ERROR("Cannot load game: Player is null");
            return false;
        }
        
        GameSaveData data;
        if (!loadGame(saveName, data)) {
            return false;
        }
        
        // Load player data
        HealthComponent* healthComp = player->getHealthComponent();
        if (healthComp) {
            healthComp->setMaxHealth(data.playerMaxHealth);
            healthComp->setHealth(data.playerHealth);
        }
        
        // Load inventory data
        if (inventory) {
            // In a full implementation, we would restore inventory items
        }
        
        // Load settings
        Config& config = Config::getInstance();
        config.setFloat("audio.masterVolume", data.masterVolume);
        config.setFloat("audio.musicVolume", data.musicVolume);
        config.setFloat("audio.sfxVolume", data.sfxVolume);
        config.setBool("video.fullscreen", data.fullscreen);
        config.setInt("video.resolutionWidth", data.resolutionWidth);
        config.setInt("video.resolutionHeight", data.resolutionHeight);
        
        SPARKY_LOG_INFO("Game loaded successfully: " + saveName);
        return true;
    }
    
    bool SaveGameManager::loadGame(const std::string& saveName, GameSaveData& data) {
        // Read from file
        std::string savePath = getSaveFilePath(saveName);
        if (!FileUtils::fileExists(savePath)) {
            SPARKY_LOG_ERROR("Save file does not exist: " + savePath);
            return false;
        }
        
        std::vector<char> buffer = FileUtils::readFile(savePath);
        if (buffer.empty()) {
            SPARKY_LOG_ERROR("Failed to read save file: " + savePath);
            return false;
        }
        
        // Deserialize the data
        if (!deserializeSaveData(buffer, data)) {
            SPARKY_LOG_ERROR("Failed to deserialize save data from: " + savePath);
            return false;
        }
        
        lastSave = saveName;
        SPARKY_LOG_INFO("Game loaded successfully from: " + savePath);
        return true;
    }
    
    bool SaveGameManager::saveSettings() {
        Config& config = Config::getInstance();
        std::string settingsPath = saveDirectory + "/settings.cfg";
        config.saveToFile(settingsPath);
        SPARKY_LOG_INFO("Settings saved to: " + settingsPath);
        return true;
    }
    
    bool SaveGameManager::loadSettings() {
        Config& config = Config::getInstance();
        std::string settingsPath = saveDirectory + "/settings.cfg";
        if (FileUtils::fileExists(settingsPath)) {
            config.loadFromFile(settingsPath);
            SPARKY_LOG_INFO("Settings loaded from: " + settingsPath);
            return true;
        }
        SPARKY_LOG_WARNING("Settings file not found: " + settingsPath);
        return false;
    }
    
    std::vector<std::string> SaveGameManager::getSaveFiles() const {
        std::vector<std::string> saveFiles;
        
        try {
            for (const auto& entry : std::filesystem::directory_iterator(saveDirectory)) {
                if (entry.is_regular_file() && entry.path().extension() == ".sav") {
                    std::string filename = entry.path().filename().string();
                    // Remove the .sav extension
                    if (filename.size() > 4) {
                        filename = filename.substr(0, filename.size() - 4);
                    }
                    saveFiles.push_back(filename);
                }
            }
        } catch (const std::exception& e) {
            SPARKY_LOG_ERROR("Failed to list save files: " + std::string(e.what()));
        }
        
        return saveFiles;
    }
    
    bool SaveGameManager::deleteSave(const std::string& saveName) {
        std::string savePath = getSaveFilePath(saveName);
        try {
            bool result = std::filesystem::remove(savePath);
            if (result) {
                SPARKY_LOG_INFO("Save file deleted: " + savePath);
            } else {
                SPARKY_LOG_WARNING("Save file not found for deletion: " + savePath);
            }
            return result;
        } catch (const std::exception& e) {
            SPARKY_LOG_ERROR("Failed to delete save file: " + savePath + " - " + std::string(e.what()));
            return false;
        }
    }
    
    std::string SaveGameManager::getSaveDirectory() const {
        return saveDirectory;
    }
    
    std::string SaveGameManager::getSaveFilePath(const std::string& saveName) const {
        return saveDirectory + "/" + saveName + ".sav";
    }
    
    bool SaveGameManager::serializeSaveData(const GameSaveData& data, std::vector<char>& buffer) const {
        try {
            // Create a string stream to serialize the data
            std::ostringstream oss;
            oss << std::fixed << std::setprecision(6);
            
            // Serialize player data
            oss << "playerHealth=" << data.playerHealth << "\n";
            oss << "playerMaxHealth=" << data.playerMaxHealth << "\n";
            oss << "playerPosition=" << data.playerPosition[0] << "," << data.playerPosition[1] << "," << data.playerPosition[2] << "\n";
            oss << "playerRotation=" << data.playerRotation[0] << "," << data.playerRotation[1] << "," << data.playerRotation[2] << "\n";
            
            // Serialize game state
            oss << "currentLevel=" << data.currentLevel << "\n";
            oss << "score=" << data.score << "\n";
            oss << "playTime=" << data.playTime << "\n";
            
            // Serialize inventory data
            oss << "inventoryItems=";
            for (size_t i = 0; i < data.inventoryItems.size(); ++i) {
                if (i > 0) oss << ";";
                oss << data.inventoryItems[i];
            }
            oss << "\n";
            
            oss << "inventoryQuantities=";
            for (size_t i = 0; i < data.inventoryQuantities.size(); ++i) {
                if (i > 0) oss << ";";
                oss << data.inventoryQuantities[i];
            }
            oss << "\n";
            
            // Serialize settings
            oss << "masterVolume=" << data.masterVolume << "\n";
            oss << "musicVolume=" << data.musicVolume << "\n";
            oss << "sfxVolume=" << data.sfxVolume << "\n";
            oss << "fullscreen=" << (data.fullscreen ? "true" : "false") << "\n";
            oss << "resolutionWidth=" << data.resolutionWidth << "\n";
            oss << "resolutionHeight=" << data.resolutionHeight << "\n";
            
            // Convert to vector of chars
            std::string str = oss.str();
            buffer.assign(str.begin(), str.end());
            
            return true;
        } catch (const std::exception& e) {
            SPARKY_LOG_ERROR("Failed to serialize save data: " + std::string(e.what()));
            return false;
        }
    }
    
    bool SaveGameManager::deserializeSaveData(const std::vector<char>& buffer, GameSaveData& data) const {
        try {
            // Convert vector of chars to string
            std::string str(buffer.begin(), buffer.end());
            
            // Parse the string line by line
            std::istringstream iss(str);
            std::string line;
            
            while (std::getline(iss, line)) {
                // Remove any carriage returns
                if (!line.empty() && line.back() == '\r') {
                    line.pop_back();
                }
                
                // Find the equals sign
                size_t equalsPos = line.find('=');
                if (equalsPos == std::string::npos) continue;
                
                std::string key = line.substr(0, equalsPos);
                std::string value = line.substr(equalsPos + 1);
                
                // Parse based on key
                if (key == "playerHealth") {
                    data.playerHealth = std::stof(value);
                } else if (key == "playerMaxHealth") {
                    data.playerMaxHealth = std::stof(value);
                } else if (key == "playerPosition") {
                    std::istringstream posStream(value);
                    std::string coord;
                    int index = 0;
                    while (std::getline(posStream, coord, ',') && index < 3) {
                        data.playerPosition[index++] = std::stof(coord);
                    }
                } else if (key == "playerRotation") {
                    std::istringstream rotStream(value);
                    std::string coord;
                    int index = 0;
                    while (std::getline(rotStream, coord, ',') && index < 3) {
                        data.playerRotation[index++] = std::stof(coord);
                    }
                } else if (key == "currentLevel") {
                    data.currentLevel = std::stoi(value);
                } else if (key == "score") {
                    data.score = std::stoi(value);
                } else if (key == "playTime") {
                    data.playTime = std::stof(value);
                } else if (key == "inventoryItems") {
                    std::istringstream itemsStream(value);
                    std::string item;
                    while (std::getline(itemsStream, item, ';')) {
                        if (!item.empty()) {
                            data.inventoryItems.push_back(item);
                        }
                    }
                } else if (key == "inventoryQuantities") {
                    std::istringstream quantitiesStream(value);
                    std::string quantity;
                    while (std::getline(quantitiesStream, quantity, ';')) {
                        if (!quantity.empty()) {
                            data.inventoryQuantities.push_back(std::stoi(quantity));
                        }
                    }
                } else if (key == "masterVolume") {
                    data.masterVolume = std::stof(value);
                } else if (key == "musicVolume") {
                    data.musicVolume = std::stof(value);
                } else if (key == "sfxVolume") {
                    data.sfxVolume = std::stof(value);
                } else if (key == "fullscreen") {
                    data.fullscreen = (value == "true");
                } else if (key == "resolutionWidth") {
                    data.resolutionWidth = std::stoi(value);
                } else if (key == "resolutionHeight") {
                    data.resolutionHeight = std::stoi(value);
                }
            }
            
            return true;
        } catch (const std::exception& e) {
            SPARKY_LOG_ERROR("Failed to deserialize save data: " + std::string(e.what()));
            return false;
        }
    }
}