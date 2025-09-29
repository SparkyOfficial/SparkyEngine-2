#include "Level.h"
#include "Player.h"
#include "Enemy.h"
#include "../../Engine/include/GameObject.h"
#include "../../Engine/include/RenderComponent.h"
#include "../../Engine/include/PhysicsComponent.h"
#include "../../Engine/include/AIComponent.h"
#include "../../Engine/include/HealthComponent.h"
#include "../../Engine/include/Mesh.h"
#include "../../Engine/include/Material.h"
#include "../../Engine/include/Logger.h"
#include "../../Engine/include/FileUtils.h"

// Only include JSON if available
#ifdef __has_include
    #if __has_include(<nlohmann/json.hpp>)
        #include <nlohmann/json.hpp>
        #define HAS_JSON 1
    #else
        #define HAS_JSON 0
    #endif
#else
    #define HAS_JSON 0
#endif

#include <fstream>
#include <sstream>

#if HAS_JSON
using json = nlohmann::json;
#endif

namespace Sparky {

    Level::Level() : name("Untitled Level"), description("No description") {
        SPARKY_LOG_DEBUG("Level created");
    }

    Level::~Level() {
        SPARKY_LOG_DEBUG("Level destroyed");
    }

    bool Level::loadFromFile(const std::string& filepath) {
        SPARKY_LOG_INFO("Loading level from file: " + filepath);
        
#if HAS_JSON
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
#else
        SPARKY_LOG_ERROR("JSON support not available. Cannot load level file.");
        return false;
#endif
    }

    bool Level::saveToFile(const std::string& filepath) const {
        SPARKY_LOG_INFO("Saving level to file: " + filepath);
        
#if HAS_JSON
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
#else
        SPARKY_LOG_ERROR("JSON support not available. Cannot save level file.");
        return false;
#endif
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
            
            // Add AI component to enemy
            auto aiComponent = enemy->addComponent<AIComponent>();
            
            // Add health component to enemy
            auto healthComponent = enemy->addComponent<HealthComponent>(100.0f);
            
            return enemy;
        } else if (obj.type == "crate" || obj.type == "box") {
            auto crate = std::make_unique<GameObject>(obj.name);
            
            // Add render component
            auto renderComponent = crate->addComponent<RenderComponent>();
            auto mesh = Mesh::createCube(1.0f);
            renderComponent->setMesh(std::move(mesh));
            
            // Add material
            auto material = std::make_unique<Material>("CrateMaterial");
            material->setDiffuse(glm::vec3(0.8f, 0.6f, 0.4f)); // Brown color
            renderComponent->setMaterial(std::move(material));
            
            // Add physics component
            auto physicsComponent = crate->addComponent<PhysicsComponent>();
            physicsComponent->setMass(10.0f); // Heavy crate
            
            return crate;
        } else if (obj.type == "wall") {
            auto wall = std::make_unique<GameObject>(obj.name);
            
            // Add render component
            auto renderComponent = wall->addComponent<RenderComponent>();
            auto mesh = Mesh::createCube(1.0f);
            renderComponent->setMesh(std::move(mesh));
            
            // Add material
            auto material = std::make_unique<Material>("WallMaterial");
            material->setDiffuse(glm::vec3(0.5f, 0.5f, 0.5f)); // Gray color
            renderComponent->setMaterial(std::move(material));
            
            // Add physics component
            auto physicsComponent = wall->addComponent<PhysicsComponent>();
            physicsComponent->setMass(0.0f); // Static object
            
            return wall;
        } else if (obj.type == "floor") {
            auto floor = std::make_unique<GameObject>(obj.name);
            
            // Add render component
            auto renderComponent = floor->addComponent<RenderComponent>();
            auto mesh = Mesh::createPlane(10.0f, 10.0f);
            renderComponent->setMesh(std::move(mesh));
            
            // Add material
            auto material = std::make_unique<Material>("FloorMaterial");
            material->setDiffuse(glm::vec3(0.3f, 0.3f, 0.3f)); // Dark gray color
            renderComponent->setMaterial(std::move(material));
            
            // Add physics component
            auto physicsComponent = floor->addComponent<PhysicsComponent>();
            physicsComponent->setMass(0.0f); // Static object
            
            return floor;
        } else {
            SPARKY_LOG_WARNING("Unknown object type in level: " + obj.type);
            auto genericObject = std::make_unique<GameObject>(obj.name);
            
            // Add a simple render component for visualization
            auto renderComponent = genericObject->addComponent<RenderComponent>();
            auto mesh = Mesh::createCube(1.0f);
            renderComponent->setMesh(std::move(mesh));
            
            // Add material
            auto material = std::make_unique<Material>("GenericMaterial");
            material->setDiffuse(glm::vec3(1.0f, 0.0f, 1.0f)); // Magenta color for unknown objects
            renderComponent->setMaterial(std::move(material));
            
            return genericObject;
        }
    }

#if HAS_JSON
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
            
            // Extract lighting information if present
            if (levelData.contains("lights") && levelData["lights"].is_array()) {
                // In a full implementation, we would parse and create lights
                SPARKY_LOG_INFO("Level contains " + std::to_string(levelData["lights"].size()) + " lights");
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
        
        // In a full implementation, we would also serialize lights, materials, etc.
        
        return levelData.dump(4); // Pretty print with 4 spaces
    }
#else
    void Level::parseLevelFile(const std::string& content) {
        SPARKY_LOG_ERROR("JSON support not available. Cannot parse level file.");
    }

    std::string Level::serializeLevel() const {
        SPARKY_LOG_ERROR("JSON support not available. Cannot serialize level.");
        return "";
    }
#endif
    
    // Additional methods for level management
    GameObject* Level::findObjectByName(const std::string& name) {
        for (auto& obj : spawnedObjects) {
            if (obj->getName() == name) {
                return obj.get();
            }
        }
        return nullptr;
    }
    
    std::vector<GameObject*> Level::findObjectsByType(const std::string& type) {
        std::vector<GameObject*> result;
        for (auto& obj : spawnedObjects) {
            // In a full implementation, we would check the object type
            // For now, we'll just return all objects
            result.push_back(obj.get());
        }
        return result;
    }
    
    void Level::addSpawnedObject(std::unique_ptr<GameObject> object) {
        spawnedObjects.push_back(std::move(object));
    }
    
    void Level::removeSpawnedObject(GameObject* object) {
        auto it = std::find_if(spawnedObjects.begin(), spawnedObjects.end(),
                              [object](const std::unique_ptr<GameObject>& obj) {
                                  return obj.get() == object;
                              });
        if (it != spawnedObjects.end()) {
            spawnedObjects.erase(it);
        }
    }
}