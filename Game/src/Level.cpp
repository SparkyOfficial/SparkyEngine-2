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
        // Proper implementation for parsing JSON file and creating entities
        SPARKY_LOG_INFO("Loading level from file: " + filename);
        
        try {
            // Open the JSON file
            std::ifstream file(filename);
            if (!file.is_open()) {
                SPARKY_LOG_ERROR("Failed to open level file: " + filename);
                // Fall back to creating test level
                createTestLevel();
                return;
            }
            
            // Read the entire file content
            std::stringstream buffer;
            buffer << file.rdbuf();
            std::string content = buffer.str();
            file.close();
            
            // Parse the JSON content (simplified parser)
            parseLevelFromJSON(content);
            
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
    
    // Helper function to parse simple JSON
    void Level::parseLevelFromJSON(const std::string& jsonContent) {
        // This is a simplified JSON parser for demonstration purposes
        // In a real implementation, you would use a proper JSON library like nlohmann/json
        
        SPARKY_LOG_DEBUG("Parsing level from JSON content");
        
        // Clear existing level
        unloadLevel();
        
        // Simple parsing approach - look for object definitions
        // This is a very basic implementation that looks for specific patterns
        
        // Look for platforms
        size_t pos = 0;
        while ((pos = jsonContent.find("\"platforms\"", pos)) != std::string::npos) {
            // Find the start of the platforms array
            size_t arrayStart = jsonContent.find('[', pos);
            if (arrayStart != std::string::npos) {
                // Find the end of the platforms array
                size_t arrayEnd = findMatchingBracket(jsonContent, arrayStart);
                if (arrayEnd != std::string::npos) {
                    // Parse each platform in the array
                    parsePlatformsFromArray(jsonContent.substr(arrayStart + 1, arrayEnd - arrayStart - 1));
                }
            }
            pos = arrayStart;
        }
        
        // Look for guns
        pos = 0;
        while ((pos = jsonContent.find("\"guns\"", pos)) != std::string::npos) {
            // Find the start of the guns array
            size_t arrayStart = jsonContent.find('[', pos);
            if (arrayStart != std::string::npos) {
                // Find the end of the guns array
                size_t arrayEnd = findMatchingBracket(jsonContent, arrayStart);
                if (arrayEnd != std::string::npos) {
                    // Parse each gun in the array
                    parseGunsFromArray(jsonContent.substr(arrayStart + 1, arrayEnd - arrayStart - 1));
                }
            }
            pos = arrayStart;
        }
        
        SPARKY_LOG_DEBUG("Parsed " + std::to_string(platforms.size()) + " platforms and " + 
                        std::to_string(guns.size()) + " guns from JSON");
    }
    
    // Helper function to find matching closing bracket
    size_t Level::findMatchingBracket(const std::string& content, size_t startPos) {
        int bracketCount = 1;
        for (size_t i = startPos + 1; i < content.length(); i++) {
            if (content[i] == '[' || content[i] == '{') {
                bracketCount++;
            } else if (content[i] == ']' || content[i] == '}') {
                bracketCount--;
                if (bracketCount == 0) {
                    return i;
                }
            }
        }
        return std::string::npos;
    }
    
    // Helper function to parse platforms from JSON array
    void Level::parsePlatformsFromArray(const std::string& arrayContent) {
        // Split the array content by objects
        size_t pos = 0;
        while (pos < arrayContent.length()) {
            // Find the start of an object
            size_t objStart = arrayContent.find('{', pos);
            if (objStart == std::string::npos) break;
            
            // Find the end of the object
            size_t objEnd = findMatchingBracket(arrayContent, objStart);
            if (objEnd == std::string::npos) break;
            
            // Parse the object
            parsePlatformFromObject(arrayContent.substr(objStart + 1, objEnd - objStart - 1));
            
            pos = objEnd + 1;
        }
    }
    
    // Helper function to parse a single platform from JSON object
    void Level::parsePlatformFromObject(const std::string& objContent) {
        // Extract name
        std::string name = "Platform";
        size_t namePos = objContent.find("\"name\"");
        if (namePos != std::string::npos) {
            size_t colonPos = objContent.find(':', namePos);
            if (colonPos != std::string::npos) {
                size_t quoteStart = objContent.find('"', colonPos);
                if (quoteStart != std::string::npos) {
                    size_t quoteEnd = objContent.find('"', quoteStart + 1);
                    if (quoteEnd != std::string::npos) {
                        name = objContent.substr(quoteStart + 1, quoteEnd - quoteStart - 1);
                    }
                }
            }
        }
        
        // Create platform
        Platform* platform = new Platform(name);
        
        // Extract position
        glm::vec3 position(0.0f);
        size_t posPos = objContent.find("\"position\"");
        if (posPos != std::string::npos) {
            size_t arrayStart = objContent.find('[', posPos);
            if (arrayStart != std::string::npos) {
                size_t arrayEnd = objContent.find(']', arrayStart);
                if (arrayEnd != std::string::npos) {
                    std::string posArray = objContent.substr(arrayStart + 1, arrayEnd - arrayStart - 1);
                    position = parseVector3FromArray(posArray);
                }
            }
        }
        platform->setPosition(position);
        
        // Extract size
        glm::vec3 size(1.0f);
        size_t sizePos = objContent.find("\"size\"");
        if (sizePos != std::string::npos) {
            size_t arrayStart = objContent.find('[', sizePos);
            if (arrayStart != std::string::npos) {
                size_t arrayEnd = objContent.find(']', arrayStart);
                if (arrayEnd != std::string::npos) {
                    std::string sizeArray = objContent.substr(arrayStart + 1, arrayEnd - arrayStart - 1);
                    size = parseVector3FromArray(sizeArray);
                }
            }
        }
        platform->setSize(size);
        
        // Add to level
        addPlatform(platform);
        
        SPARKY_LOG_DEBUG("Created platform: " + name + " at (" + 
                        std::to_string(position.x) + ", " + std::to_string(position.y) + ", " + std::to_string(position.z) + ")");
    }
    
    // Helper function to parse guns from JSON array
    void Level::parseGunsFromArray(const std::string& arrayContent) {
        // Split the array content by objects
        size_t pos = 0;
        while (pos < arrayContent.length()) {
            // Find the start of an object
            size_t objStart = arrayContent.find('{', pos);
            if (objStart == std::string::npos) break;
            
            // Find the end of the object
            size_t objEnd = findMatchingBracket(arrayContent, objStart);
            if (objEnd == std::string::npos) break;
            
            // Parse the object
            parseGunFromObject(arrayContent.substr(objStart + 1, objEnd - objStart - 1));
            
            pos = objEnd + 1;
        }
    }
    
    // Helper function to parse a single gun from JSON object
    void Level::parseGunFromObject(const std::string& objContent) {
        // Extract name
        std::string name = "Gun";
        size_t namePos = objContent.find("\"name\"");
        if (namePos != std::string::npos) {
            size_t colonPos = objContent.find(':', namePos);
            if (colonPos != std::string::npos) {
                size_t quoteStart = objContent.find('"', colonPos);
                if (quoteStart != std::string::npos) {
                    size_t quoteEnd = objContent.find('"', quoteStart + 1);
                    if (quoteEnd != std::string::npos) {
                        name = objContent.substr(quoteStart + 1, quoteEnd - quoteStart - 1);
                    }
                }
            }
        }
        
        // Create gun
        Gun* gun = new Gun(name);
        
        // Extract position
        glm::vec3 position(0.0f);
        size_t posPos = objContent.find("\"position\"");
        if (posPos != std::string::npos) {
            size_t arrayStart = objContent.find('[', posPos);
            if (arrayStart != std::string::npos) {
                size_t arrayEnd = objContent.find(']', arrayStart);
                if (arrayEnd != std::string::npos) {
                    std::string posArray = objContent.substr(arrayStart + 1, arrayEnd - arrayStart - 1);
                    position = parseVector3FromArray(posArray);
                }
            }
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
        
        while (std::getline(ss, item, ',')) {
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