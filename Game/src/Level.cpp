#include "Level.h"
#include "Player.h"
#include "Enemy.h"
#include "../../Engine/include/GameObject.h"
#include "../../Engine/include/Logger.h"
#include "../../Engine/include/FileUtils.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <sstream>

using json = nlohmann::json;

namespace Sparky {

    Level::Level() : name("Untitled Level"), description("No description") {
        SPARKY_LOG_DEBUG("Level created");
    }

    Level::~Level() {
        SPARKY_LOG_DEBUG("Level destroyed");
    }

    bool Level::loadFromFile(const std::string& filepath) {
        SPARKY_LOG_INFO("Loading level from file: " + filepath);
        
        try {
            // Read the file content
            std::vector<char> fileContent = FileUtils::readFile(filepath);
            std::string content(fileContent.begin(), fileContent.end());
            
            // Parse the JSON content
            parseLevelFile(content);
            
            SPARKY_LOG_INFO("Level loaded successfully: " + name);
            return true;
        } catch (const std::exception& e) {
            SPARKY_LOG_ERROR("Failed to load level: " + std::string(e.what()));
            return false;
        }
    }

    bool Level::saveToFile(const std::string& filepath) const {
        SPARKY_LOG_INFO("Saving level to file: " + filepath);
        
        try {
            // Serialize the level to JSON
            std::string content = serializeLevel();
            
            // Write to file
            std::ofstream file(filepath);
            if (!file.is_open()) {
                SPARKY_LOG_ERROR("Failed to open file for writing: " + filepath);
                return false;
            }
            
            file << content;
            file.close();
            
            SPARKY_LOG_INFO("Level saved successfully: " + filepath);
            return true;
        } catch (const std::exception& e) {
            SPARKY_LOG_ERROR("Failed to save level: " + std::string(e.what()));
            return false;
        }
    }

    void Level::addLevelObject(const LevelObject& obj) {
        levelObjects.push_back(obj);
    }

    void Level::removeLevelObject(size_t index) {
        if (index < levelObjects.size()) {
            levelObjects.erase(levelObjects.begin() + index);
        }
    }

    void Level::spawnObjects() {
        SPARKY_LOG_INFO("Spawning " + std::to_string(levelObjects.size()) + " objects in level");
        
        // Clear previously spawned objects
        spawnedObjects.clear();
        
        // Spawn all level objects
        for (const auto& obj : levelObjects) {
            auto gameObject = createObject(obj);
            if (gameObject) {
                // Set the object's transform
                gameObject->setPosition(obj.position);
                gameObject->setRotation(obj.rotation);
                gameObject->setScale(obj.scale);
                gameObject->setName(obj.name);
                
                // Add to spawned objects
                spawnedObjects.push_back(std::move(gameObject));
            }
        }
        
        SPARKY_LOG_INFO("Spawned " + std::to_string(spawnedObjects.size()) + " objects");
    }

    std::unique_ptr<GameObject> Level::createObject(const LevelObject& obj) const {
        // Create objects based on their type
        if (obj.type == "player") {
            auto player = std::make_unique<Player>();
            return player;
        } else if (obj.type == "enemy") {
            auto enemy = std::make_unique<Enemy>();
            return enemy;
        } else if (obj.type == "crate" || obj.type == "box") {
            auto crate = std::make_unique<GameObject>(obj.name);
            // In a full implementation, we would set up the crate's mesh, etc.
            return crate;
        } else if (obj.type == "wall") {
            auto wall = std::make_unique<GameObject>(obj.name);
            // In a full implementation, we would set up the wall's mesh, etc.
            return wall;
        } else {
            SPARKY_LOG_WARNING("Unknown object type in level: " + obj.type);
            auto genericObject = std::make_unique<GameObject>(obj.name);
            return genericObject;
        }
    }

    void Level::parseLevelFile(const std::string& content) {
        try {
            // Parse JSON
            json levelData = json::parse(content);
            
            // Extract level metadata
            if (levelData.contains("name")) {
                name = levelData["name"];
            }
            
            if (levelData.contains("description")) {
                description = levelData["description"];
            }
            
            // Clear existing objects
            levelObjects.clear();
            
            // Extract objects
            if (levelData.contains("objects") && levelData["objects"].is_array()) {
                for (const auto& objData : levelData["objects"]) {
                    LevelObject obj;
                    
                    // Extract object properties
                    obj.type = objData.value("type", "generic");
                    obj.name = objData.value("name", "UnnamedObject");
                    
                    // Extract position
                    if (objData.contains("position") && objData["position"].is_array() && objData["position"].size() == 3) {
                        obj.position.x = objData["position"][0];
                        obj.position.y = objData["position"][1];
                        obj.position.z = objData["position"][2];
                    } else {
                        obj.position = glm::vec3(0.0f);
                    }
                    
                    // Extract rotation
                    if (objData.contains("rotation") && objData["rotation"].is_array() && objData["rotation"].size() == 3) {
                        obj.rotation.x = objData["rotation"][0];
                        obj.rotation.y = objData["rotation"][1];
                        obj.rotation.z = objData["rotation"][2];
                    } else {
                        obj.rotation = glm::vec3(0.0f);
                    }
                    
                    // Extract scale
                    if (objData.contains("scale") && objData["scale"].is_array() && objData["scale"].size() == 3) {
                        obj.scale.x = objData["scale"][0];
                        obj.scale.y = objData["scale"][1];
                        obj.scale.z = objData["scale"][2];
                    } else {
                        obj.scale = glm::vec3(1.0f);
                    }
                    
                    levelObjects.push_back(obj);
                }
            }
            
        } catch (const std::exception& e) {
            SPARKY_LOG_ERROR("Failed to parse level file: " + std::string(e.what()));
            throw;
        }
    }

    std::string Level::serializeLevel() const {
        json levelData;
        
        // Add level metadata
        levelData["name"] = name;
        levelData["description"] = description;
        
        // Add objects
        json objectsArray = json::array();
        for (const auto& obj : levelObjects) {
            json objData;
            objData["type"] = obj.type;
            objData["name"] = obj.name;
            objData["position"] = {obj.position.x, obj.position.y, obj.position.z};
            objData["rotation"] = {obj.rotation.x, obj.rotation.y, obj.rotation.z};
            objData["scale"] = {obj.scale.x, obj.scale.y, obj.scale.z};
            objectsArray.push_back(objData);
        }
        
        levelData["objects"] = objectsArray;
        
        return levelData.dump(4); // Pretty print with 4 spaces
    }
}