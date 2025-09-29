#include "Level.h"
#include "Platform.h"
#include "Gun.h"
#include <glm/glm.hpp>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <iostream>

// Using the nlohmann/json library for proper JSON parsing
#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace Sparky {

    Level::Level(const std::string& name) : GameObject() {
        setName(name);
    }

    Level::~Level() {
        unloadLevel();
    }

    void Level::update(float deltaTime) {
        GameObject::update(deltaTime);
        
        // Update all platforms
        for (Platform* platform : platforms) {
            platform->update(deltaTime);
        }
        
        // Update all guns
        for (Gun* gun : guns) {
            gun->update(deltaTime);
        }
    }

    void Level::render() {
        GameObject::render();
        
        // Render all platforms
        for (Platform* platform : platforms) {
            platform->render();
        }
        
        // Render all guns
        for (Gun* gun : guns) {
            gun->render();
        }
    }

    void Level::loadLevel() {
        createTestLevel();
    }

    void Level::loadLevelFromFile(const std::string& filename) {
        // Parse the JSON file using a proper JSON library
        SPARKY_LOG_INFO("Loading level from file: " + filename);
        
        try {
            // Open and parse the JSON file
            std::ifstream file(filename);
            if (!file.is_open()) {
                SPARKY_LOG_ERROR("Failed to open level file: " + filename);
                // Fall back to creating test level
                createTestLevel();
                return;
            }
            
            // Parse JSON using nlohmann/json library
            json jsonData;
            file >> jsonData;
            file.close();
            
            // Clear existing level
            unloadLevel();
            
            // Parse platforms
            if (jsonData.contains("platforms") && jsonData["platforms"].is_array()) {
                for (const auto& platformData : jsonData["platforms"]) {
                    parsePlatformFromJSON(platformData);
                }
            }
            
            // Parse guns
            if (jsonData.contains("guns") && jsonData["guns"].is_array()) {
                for (const auto& gunData : jsonData["guns"]) {
                    parseGunFromJSON(gunData);
                }
            }
            
            SPARKY_LOG_INFO("Level loaded successfully from: " + filename);
        } catch (const std::exception& e) {
            SPARKY_LOG_ERROR("Failed to load level from file: " + filename + " - " + std::string(e.what()));
            // Fall back to creating test level
            createTestLevel();
        }
    }

    void Level::unloadLevel() {
        // Clean up platforms
        for (Platform* platform : platforms) {
            delete platform;
        }
        platforms.clear();
        
        // Clean up guns
        for (Gun* gun : guns) {
            delete gun;
        }
        guns.clear();
    }

    void Level::addPlatform(Platform* platform) {
        platforms.push_back(platform);
    }

    void Level::addGun(Gun* gun) {
        guns.push_back(gun);
    }

    void Level::createTestLevel() {
        // Create a floor platform
        Platform* floor = new Platform("Floor");
        floor->setPosition(glm::vec3(0.0f, -1.0f, 0.0f));
        floor->setSize(glm::vec3(20.0f, 1.0f, 20.0f));
        addPlatform(floor);
        
        // Create some platforms
        Platform* platform1 = new Platform("Platform1");
        platform1->setPosition(glm::vec3(5.0f, 2.0f, 0.0f));
        platform1->setSize(glm::vec3(3.0f, 1.0f, 3.0f));
        addPlatform(platform1);
        
        Platform* platform2 = new Platform("Platform2");
        platform2->setPosition(glm::vec3(-5.0f, 4.0f, 0.0f));
        platform2->setSize(glm::vec3(3.0f, 1.0f, 3.0f));
        addPlatform(platform2);
        
        // Create a ramp
        Platform* ramp = new Platform("Ramp");
        ramp->setPosition(glm::vec3(0.0f, 1.0f, 5.0f));
        ramp->setSize(glm::vec3(8.0f, 1.0f, 3.0f));
        addPlatform(ramp);
        
        // Create stairs
        for (int i = 0; i < 5; i++) {
            Platform* step = new Platform("Step" + std::to_string(i));
            step->setPosition(glm::vec3(8.0f, 0.5f + i * 1.0f, -5.0f + i * 1.0f));
            step->setSize(glm::vec3(2.0f, 1.0f, 1.0f));
            addPlatform(step);
        }
        
        // Create a table with a gun
        Platform* table = new Platform("Table");
        table->setPosition(glm::vec3(-8.0f, 1.5f, 0.0f));
        table->setSize(glm::vec3(2.0f, 0.5f, 1.0f));
        addPlatform(table);
        
        Gun* pistol = new Gun("Pistol");
        pistol->setPosition(glm::vec3(-8.0f, 2.0f, 0.0f));
        addGun(pistol);
    }
    
    // Helper function to parse a single platform from JSON data
    void Level::parsePlatformFromJSON(const json& platformData) {
        // Extract name
        std::string name = "Platform";
        if (platformData.contains("name") && platformData["name"].is_string()) {
            name = platformData["name"];
        }
        
        // Create platform
        Platform* platform = new Platform(name);
        
        // Extract position
        glm::vec3 position(0.0f);
        if (platformData.contains("position") && platformData["position"].is_array() && platformData["position"].size() >= 3) {
            position.x = platformData["position"][0];
            position.y = platformData["position"][1];
            position.z = platformData["position"][2];
        }
        platform->setPosition(position);
        
        // Extract size
        glm::vec3 size(1.0f);
        if (platformData.contains("size") && platformData["size"].is_array() && platformData["size"].size() >= 3) {
            size.x = platformData["size"][0];
            size.y = platformData["size"][1];
            size.z = platformData["size"][2];
        }
        platform->setSize(size);
        
        // Add to level
        addPlatform(platform);
        
        SPARKY_LOG_DEBUG("Created platform: " + name + " at (" + 
                        std::to_string(position.x) + ", " + std::to_string(position.y) + ", " + std::to_string(position.z) + ")");
    }
    
    // Helper function to parse a single gun from JSON data
    void Level::parseGunFromJSON(const json& gunData) {
        // Extract name
        std::string name = "Gun";
        if (gunData.contains("name") && gunData["name"].is_string()) {
            name = gunData["name"];
        }
        
        // Create gun
        Gun* gun = new Gun(name);
        
        // Extract position
        glm::vec3 position(0.0f);
        if (gunData.contains("position") && gunData["position"].is_array() && gunData["position"].size() >= 3) {
            position.x = gunData["position"][0];
            position.y = gunData["position"][1];
            position.z = gunData["position"][2];
        }
        gun->setPosition(position);
        
        // Add to level
        addGun(gun);
        
        SPARKY_LOG_DEBUG("Created gun: " + name + " at (" + 
                        std::to_string(position.x) + ", " + std::to_string(position.y) + ", " + std::to_string(position.z) + ")");
    }
    
    // Helper function to parse a vector3 from JSON array "[x,y,z]"
    glm::vec3 Level::parseVector3FromArray(const std::string& arrayContent) {
        glm::vec3 result(0.0f);
        std::stringstream ss(arrayContent);
        std::string item;
        int index = 0;
        
        while (std::getline(ss, item, ',') && index < 3) {
            // Remove whitespace
            item.erase(0, item.find_first_not_of(" \t"));
            item.erase(item.find_last_not_of(" \t") + 1);
            
            try {
                float value = std::stof(item);
                switch (index) {
                    case 0: result.x = value; break;
                    case 1: result.y = value; break;
                    case 2: result.z = value; break;
                }
                index++;
            } catch (const std::exception&) {
                // Ignore parsing errors
            }
        }
        
        return result;
    }
}