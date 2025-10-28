#include "../include/Scene.h"
#include "../include/GameObject.h"
#include "../include/Logger.h"
#include <iostream>
#include <memory>

using namespace Sparky;

// Simple demo to test Scene enhancements
int main() {
    std::cout << "=== Scene Enhancement Demo ===" << std::endl;
    
    // Create a scene
    auto scene = std::make_unique<Scene>();
    
    // Test scene properties
    scene->setTag("TestScene");
    scene->setActive(true);
    scene->setPhysicsEnabled(true);
    
    std::cout << "Scene tag: " << scene->getTag() << std::endl;
    std::cout << "Scene active: " << (scene->isActive() ? "true" : "false") << std::endl;
    std::cout << "Physics enabled: " << (scene->isPhysicsEnabled() ? "true" : "false") << std::endl;
    
    // Test gravity
    glm::vec3 gravity(0.0f, -9.81f, 0.0f);
    scene->setGravity(gravity);
    glm::vec3 currentGravity = scene->getGravity();
    std::cout << "Gravity: (" << currentGravity.x << ", " << currentGravity.y << ", " << currentGravity.z << ")" << std::endl;
    
    // Add some game objects
    auto obj1 = std::make_unique<GameObject>("Player");
    auto obj2 = std::make_unique<GameObject>("Enemy");
    auto obj3 = std::make_unique<GameObject>("Obstacle");
    
    scene->addGameObject(std::move(obj1));
    scene->addGameObject(std::move(obj2));
    scene->addGameObject(std::move(obj3));
    
    std::cout << "Game objects in scene: " << scene->getGameObjectCount() << std::endl;
    
    // Test object retrieval
    GameObject* player = scene->getGameObject("Player");
    if (player) {
        std::cout << "Found player object: " << player->getName() << std::endl;
    }
    
    // Test scene update
    scene->update(0.016f); // Simulate one frame at 60 FPS
    
    std::cout << "=== Demo Complete ===" << std::endl;
    
    return 0;
}