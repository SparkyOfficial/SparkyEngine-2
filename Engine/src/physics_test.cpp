#include "../include/RigidBodyComponent.h"
#include "../include/PhysicsWorld.h"
#include "../include/GameObject.h"
#include "../include/Logger.h"
#include <iostream>
#include <memory>

int main() {
    SPARKY_LOG_INFO("Starting Physics Test");
    
    // Create game objects
    auto objectA = std::make_unique<Sparky::GameObject>("ObjectA");
    objectA->setPosition(glm::vec3(0.0f, 5.0f, 0.0f));
    
    auto objectB = std::make_unique<Sparky::GameObject>("ObjectB");
    objectB->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    
    // Create rigid body components
    auto rigidBodyA = objectA->addComponent<Sparky::RigidBodyComponent>();
    rigidBodyA->setBodyType(Sparky::BodyType::DYNAMIC);
    
    auto rigidBodyB = objectB->addComponent<Sparky::RigidBodyComponent>();
    rigidBodyB->setBodyType(Sparky::BodyType::STATIC);
    
    // Add to physics world
    Sparky::PhysicsWorld::getInstance().addPhysicsComponent(rigidBodyA);
    Sparky::PhysicsWorld::getInstance().addPhysicsComponent(rigidBodyB);
    
    // Set gravity
    Sparky::PhysicsWorld::getInstance().setGravity(glm::vec3(0.0f, -9.81f, 0.0f));
    
    // Simulate for 5 seconds
    float deltaTime = 1.0f / 60.0f; // 60 FPS
    for (int i = 0; i < 300; i++) { // 300 frames = 5 seconds
        Sparky::PhysicsWorld::getInstance().update(deltaTime);
        
        // Print positions every 60 frames (1 second)
        if (i % 60 == 0) {
            glm::vec3 posA = objectA->getPosition();
            glm::vec3 posB = objectB->getPosition();
            SPARKY_LOG_INFO("Frame " + std::to_string(i) + 
                          " - ObjectA: (" + std::to_string(posA.x) + ", " + 
                          std::to_string(posA.y) + ", " + std::to_string(posA.z) + ")" +
                          " ObjectB: (" + std::to_string(posB.x) + ", " + 
                          std::to_string(posB.y) + ", " + std::to_string(posB.z) + ")");
        }
    }
    
    SPARKY_LOG_INFO("Physics Test Completed");
    return 0;
}