#include "Level.h"
#include "Platform.h"
#include "Gun.h"
#include <glm/glm.hpp>
#include <fstream>
#include <sstream>
// #include <json/json.h> // Assuming JSON library is available

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
        // In a real implementation, this would parse the JSON file
        // and create entities based on the file contents
        // For now, we'll just create the test level
        createTestLevel();
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
}