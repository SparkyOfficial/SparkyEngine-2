/*
 * bullet_physics_test.cpp
 * 
 * Author: Андрій Будильников
 * 
 * Simple test to demonstrate Bullet Physics integration with Sparky Engine
 */

#include "../include/SparkyEngine.h"
#include "../include/GameObject.h"
#include "../include/Logger.h"
#include <iostream>
#include <memory>

#ifdef HAS_BULLET
#include "../include/Bullet/BulletRigidBodyComponent.h"
#include "../include/Bullet/BulletPhysicsWorld.h"
#include "../include/Bullet/BulletCollisionShapeComponent.h"
#endif

using namespace Sparky;

class BulletPhysicsTest {
private:
    std::unique_ptr<Engine> engine;
#ifdef HAS_BULLET
    std::unique_ptr<GameObject> boxObject;
    std::unique_ptr<GameObject> groundObject;
    Bullet::BulletRigidBodyComponent* boxRigidBody;
    Bullet::BulletRigidBodyComponent* groundRigidBody;
    Bullet::BulletPhysicsWorld& physicsWorld;
#endif

public:
#ifdef HAS_BULLET
    BulletPhysicsTest() : physicsWorld(Bullet::BulletPhysicsWorld::getInstance()) {
        // Initialize the physics world
        physicsWorld.initialize();
    }

    ~BulletPhysicsTest() {
        // Cleanup
        physicsWorld.cleanup();
    }
#else
    BulletPhysicsTest() {}
    ~BulletPhysicsTest() {}
#endif

#ifdef HAS_BULLET
    bool initialize() {
        SPARKY_LOG_INFO("Initializing Bullet Physics Test...");

        // Create a box object
        boxObject = std::make_unique<GameObject>("Box");
        boxObject->setPosition(glm::vec3(0.0f, 10.0f, 0.0f));

        // Create a collision shape for the box
        auto* boxCollisionShape = boxObject->addComponent<Bullet::BulletCollisionShapeComponent>();
        float halfExtents[3] = {1.0f, 1.0f, 1.0f};
        boxCollisionShape->createBoxShape(halfExtents);

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
        float normal[3] = {0.0f, 1.0f, 0.0f};
        groundCollisionShape->createStaticPlaneShape(normal, 0.0f);

        // Create a rigid body for the ground
        groundRigidBody = groundObject->addComponent<Bullet::BulletRigidBodyComponent>();
        groundRigidBody->setBodyType(Bullet::BulletBodyType::STATIC); // Static body
        groundRigidBody->setRestitution(0.1f);
        groundRigidBody->setFriction(0.8f);

        // Add the ground to the physics world
        physicsWorld.addRigidBody(groundRigidBody);

        SPARKY_LOG_INFO("Bullet Physics Test initialized successfully!");
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
    }

    void run() {
        if (!initialize()) {
            SPARKY_LOG_ERROR("Failed to initialize Bullet Physics Test!");
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
#else
    bool initialize() {
        SPARKY_LOG_INFO("Bullet Physics not available - test skipped.");
        return true;
    }

    void update(float deltaTime) {
        // No physics update when Bullet is not available
    }

    void run() {
        SPARKY_LOG_INFO("Bullet Physics not available - test skipped.");
    }
#endif
};

int main() {
    try {
        BulletPhysicsTest test;
        test.run();
        return 0;
    } catch (const std::exception& e) {
        SPARKY_LOG_ERROR("Exception occurred: " + std::string(e.what()));
        return -1;
    }
}