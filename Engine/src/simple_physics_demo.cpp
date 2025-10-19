/*
 * simple_physics_demo.cpp
 * 
 * Author: Андрій Будильников
 * 
 * Simple physics demo to demonstrate the concept of physics integration
 */

#include "../include/SparkyEngine.h"
#include "../include/GameObject.h"
#include "../include/Bullet/BulletRigidBodyComponent.h"
#include "../include/Bullet/BulletPhysicsWorld.h"
#include "../include/Bullet/BulletCollisionShapeComponent.h"
#include "../include/Logger.h"
#include <iostream>
#include <memory>

// Include glm headers after the SparkyEngine includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace Sparky;

class SimplePhysicsDemo {
private:
    std::unique_ptr<GameObject> boxObject;
    std::unique_ptr<GameObject> groundObject;
    Bullet::BulletRigidBodyComponent* boxRigidBody;
    Bullet::BulletRigidBodyComponent* groundRigidBody;
    Bullet::BulletPhysicsWorld& physicsWorld;
    float simulationTime;

public:
    SimplePhysicsDemo() : physicsWorld(Bullet::BulletPhysicsWorld::getInstance()), simulationTime(0.0f) {
        // Initialize the physics world
        physicsWorld.initialize();
    }

    ~SimplePhysicsDemo() {
        // Cleanup
        physicsWorld.cleanup();
    }

    bool initialize() {
        SPARKY_LOG_INFO("Initializing Simple Physics Demo...");

        // Create a box object
        boxObject = std::make_unique<GameObject>("Box");
        boxObject->setPosition(glm::vec3(0.0f, 10.0f, 0.0f));

        // Create a collision shape for the box
        auto* boxCollisionShape = boxObject->addComponent<Bullet::BulletCollisionShapeComponent>();
        boxCollisionShape->createBoxShape(glm::vec3(1.0f, 1.0f, 1.0f));

        // Create a rigid body for the box
        boxRigidBody = boxObject->addComponent<Bullet::BulletRigidBodyComponent>();
        boxRigidBody->setMass(1.0f); // Dynamic body
        boxRigidBody->setRestitution(0.5f); // Bounciness
        boxRigidBody->setFriction(0.3f);

        // Add the box to the physics world
        physicsWorld.addRigidBody(boxRigidBody);

        // Create a ground object
        groundObject = std::make_unique<GameObject>("Ground");
        groundObject->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

        // Create a collision shape for the ground (static plane)
        auto* groundCollisionShape = groundObject->addComponent<Bullet::BulletCollisionShapeComponent>();
        groundCollisionShape->createStaticPlaneShape(glm::vec3(0.0f, 1.0f, 0.0f), 0.0f);

        // Create a rigid body for the ground
        groundRigidBody = groundObject->addComponent<Bullet::BulletRigidBodyComponent>();
        groundRigidBody->setBodyType(Bullet::BulletBodyType::STATIC); // Static body
        groundRigidBody->setRestitution(0.1f);
        groundRigidBody->setFriction(0.8f);

        // Add the ground to the physics world
        physicsWorld.addRigidBody(groundRigidBody);

        SPARKY_LOG_INFO("Simple Physics Demo initialized successfully!");
        return true;
    }

    void update(float deltaTime) {
        // Update the physics world
        physicsWorld.update(deltaTime);
        
        // Print the box position to see it falling
        if (boxObject) {
            glm::vec3 pos = boxObject->getPosition();
            SPARKY_LOG_INFO("Box position: (" + std::to_string(pos.x) + ", " + 
                           std::to_string(pos.y) + ", " + std::to_string(pos.z) + ")");
        }
        
        simulationTime += deltaTime;
    }

    void run() {
        if (!initialize()) {
            SPARKY_LOG_ERROR("Failed to initialize Simple Physics Demo!");
            return;
        }

        // Simulate for 5 seconds
        float totalTime = 0.0f;
        float deltaTime = 1.0f / 60.0f; // 60 FPS
        
        SPARKY_LOG_INFO("Starting physics simulation...");
        
        while (totalTime < 5.0f) {
            update(deltaTime);
            totalTime += deltaTime;
            
            // Sleep to simulate frame time (in a real application, this would be handled by the engine)
            // std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }
        
        SPARKY_LOG_INFO("Physics simulation completed!");
    }
};

int main() {
    try {
        SimplePhysicsDemo demo;
        demo.run();
        return 0;
    } catch (const std::exception& e) {
        SPARKY_LOG_ERROR("Exception occurred: " + std::string(e.what()));
        return -1;
    }
}