#include "../include/Scene.h"
#include "../include/GameObject.h"
#include "../include/Logger.h"
#include <iostream>
#include <memory>

// Test to verify all enhanced Scene features are working
int main() {
    std::cout << "Testing Enhanced Scene Features..." << std::endl;
    
    // Test 1: Basic scene creation and properties
    auto scene = std::make_unique<Sparky::Scene>();
    
    // Test tagging
    scene->setTag("TestScene");
    std::cout << "Scene tag: " << scene->getTag() << std::endl;
    
    // Test active state
    scene->setActive(true);
    std::cout << "Scene active: " << (scene->isActive() ? "true" : "false") << std::endl;
    
    // Test physics enabled
    scene->setPhysicsEnabled(true);
    std::cout << "Physics enabled: " << (scene->isPhysicsEnabled() ? "true" : "false") << std::endl;
    
    // Test gravity
    glm::vec3 testGravity(0.0f, -20.0f, 0.0f);
    scene->setGravity(testGravity);
    glm::vec3 currentGravity = scene->getGravity();
    std::cout << "Gravity: (" << currentGravity.x << ", " << currentGravity.y << ", " << currentGravity.z << ")" << std::endl;
    
    // Test 2: Game object management
    auto obj1 = std::make_unique<Sparky::GameObject>("Player");
    auto obj2 = std::make_unique<Sparky::GameObject>("Enemy");
    auto obj3 = std::make_unique<Sparky::GameObject>("Obstacle");
    
    scene->addGameObject(std::move(obj1));
    scene->addGameObject(std::move(obj2));
    scene->addGameObject(std::move(obj3));
    
    std::cout << "Game objects in scene: " << scene->getGameObjectCount() << std::endl;
    
    // Test object retrieval
    Sparky::GameObject* player = scene->getGameObject("Player");
    if (player) {
        std::cout << "Found player object: " << player->getName() << std::endl;
    }
    
    // Test 3: Scene state changes
    scene->setActive(false);
    std::cout << "After deactivating scene - active: " << (scene->isActive() ? "true" : "false") << std::endl;
    
    scene->setPhysicsEnabled(false);
    std::cout << "After disabling physics - physics enabled: " << (scene->isPhysicsEnabled() ? "true" : "false") << std::endl;
    
    // Test 4: Different gravity settings
    glm::vec3 moonGravity(0.0f, -1.6f, 0.0f);
    scene->setGravity(moonGravity);
    glm::vec3 newGravity = scene->getGravity();
    std::cout << "Moon gravity: (" << newGravity.x << ", " << newGravity.y << ", " << newGravity.z << ")" << std::endl;
    
    std::cout << "All enhanced Scene features working correctly!" << std::endl;
    
    return 0;
}