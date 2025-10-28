#include "../include/PhysicsWorld.h"
#include "../include/PhysicsComponent.h"
#include "../include/RigidBodyComponent.h"
#include "../include/GameObject.h"
#include "../include/Logger.h"
#include "../include/CollisionSystem.h"
#include <algorithm>
#include <cmath>
#include <memory>

namespace Sparky {
    
    // Default constructor
    PhysicsWorld::PhysicsWorld() : gravity(0.0f, -9.81f, 0.0f) {
        SPARKY_LOG_DEBUG("PhysicsWorld created");
    }
    
    // Constructor for dependency injection
    PhysicsWorld::PhysicsWorld(const glm::vec3& gravity) : gravity(gravity) {
        SPARKY_LOG_DEBUG("PhysicsWorld created with custom gravity");
    }
    
    PhysicsWorld::~PhysicsWorld() {
        SPARKY_LOG_DEBUG("PhysicsWorld destroyed");
    }
    
    // Singleton instance accessor (backward compatibility)
    PhysicsWorld& PhysicsWorld::getInstance() {
        static PhysicsWorld instance;
        return instance;
    }
    
    // Factory method for dependency injection
    std::unique_ptr<PhysicsWorld> PhysicsWorld::create(const glm::vec3& gravity) {
        return std::make_unique<PhysicsWorld>(gravity);
    }
    
    void PhysicsWorld::addPhysicsComponent(PhysicsComponent* component) {
        if (std::find(components.begin(), components.end(), component) == components.end()) {
            components.push_back(component);
            SPARKY_LOG_DEBUG("PhysicsComponent added to PhysicsWorld");
        }
    }
    
    void PhysicsWorld::removePhysicsComponent(PhysicsComponent* component) {
        auto it = std::find(components.begin(), components.end(), component);
        if (it != components.end()) {
            components.erase(it);
            SPARKY_LOG_DEBUG("PhysicsComponent removed from PhysicsWorld");
        }
    }
    
    void PhysicsWorld::addRigidBody(RigidBodyComponent* rigidBody) {
        if (std::find(rigidBodies.begin(), rigidBodies.end(), rigidBody) == rigidBodies.end()) {
            rigidBodies.push_back(rigidBody);
            SPARKY_LOG_DEBUG("RigidBodyComponent added to PhysicsWorld");
        }
    }
    
    void PhysicsWorld::removeRigidBody(RigidBodyComponent* rigidBody) {
        auto it = std::find(rigidBodies.begin(), rigidBodies.end(), rigidBody);
        if (it != rigidBodies.end()) {
            rigidBodies.erase(it);
            SPARKY_LOG_DEBUG("RigidBodyComponent removed from PhysicsWorld");
        }
    }
    
    void PhysicsWorld::update(float deltaTime) {
        // Update all physics components
        for (auto& component : components) {
            component->update(deltaTime);
        }
        
        // Update all rigid bodies
        for (auto& rigidBody : rigidBodies) {
            // Apply gravity
            if (rigidBody->isAffectedByGravity()) {
                glm::vec3 force = gravity * rigidBody->getMass();
                rigidBody->applyForce(force);
            }
            
            // Update physics
            rigidBody->update(deltaTime);
        }
        
        // Perform broadphase collision detection
        broadphase();
        
        // Detect and resolve collisions
        auto collisions = detectCollisions();
        resolveCollisions(collisions);
    }
    
    void PhysicsWorld::setGravity(const glm::vec3& grav) {
        gravity = grav;
        SPARKY_LOG_DEBUG("Gravity set to (" + std::to_string(grav.x) + ", " + 
                        std::to_string(grav.y) + ", " + std::to_string(grav.z) + ")");
    }
    
    void PhysicsWorld::broadphase() {
        // Simple broadphase - check all pairs
        // In a real implementation, we would use spatial partitioning for efficiency
        broadphasePairs.clear();
        
        // Check all physics component pairs
        for (size_t i = 0; i < components.size(); ++i) {
            for (size_t j = i + 1; j < components.size(); ++j) {
                broadphasePairs.push_back({components[i], components[j]});
            }
        }
        
        // Check all rigid body pairs
        for (size_t i = 0; i < rigidBodies.size(); ++i) {
            for (size_t j = i + 1; j < rigidBodies.size(); ++j) {
                broadphasePairsRB.push_back({rigidBodies[i], rigidBodies[j]});
            }
        }
        
        // Check physics components against rigid bodies
        for (auto& component : components) {
            for (auto& rigidBody : rigidBodies) {
                broadphasePairsCR.push_back({component, rigidBody});
            }
        }
    }
    
    std::vector<CollisionData> PhysicsWorld::detectCollisions() {
        std::vector<CollisionData> collisions;
        
        // Check physics component pairs
        for (const auto& pair : broadphasePairs) {
            if (checkCollision(pair.first, pair.second)) {
                CollisionData collision;
                collision.componentA = pair.first;
                collision.componentB = pair.second;
                collision.resolved = false;
                // In a full implementation, we would calculate contact points and normals
                collision.contactPoint = glm::vec3(0.0f);
                collision.normal = glm::vec3(0.0f, 1.0f, 0.0f);
                collision.penetrationDepth = 0.0f;
                collisions.push_back(collision);
            }
        }
        
        // Check rigid body pairs
        for (const auto& pair : broadphasePairsRB) {
            if (checkCollisionRB(pair.first, pair.second)) {
                CollisionDataRB collision;
                collision.bodyA = pair.first;
                collision.bodyB = pair.second;
                collision.resolved = false;
                // In a full implementation, we would calculate contact points and normals
                collision.contactPoint = glm::vec3(0.0f);
                collision.normal = glm::vec3(0.0f, 1.0f, 0.0f);
                collision.penetrationDepth = 0.0f;
                collisionsRB.push_back(collision);
            }
        }
        
        // Check physics components against rigid bodies
        for (const auto& pair : broadphasePairsCR) {
            if (checkCollisionCR(pair.first, pair.second)) {
                CollisionDataCR collision;
                collision.component = pair.first;
                collision.rigidBody = pair.second;
                collision.resolved = false;
                // In a full implementation, we would calculate contact points and normals
                collision.contactPoint = glm::vec3(0.0f);
                collision.normal = glm::vec3(0.0f, 1.0f, 0.0f);
                collision.penetrationDepth = 0.0f;
                collisionsCR.push_back(collision);
            }
        }
        
        return collisions;
    }
    
    bool PhysicsWorld::checkCollision(PhysicsComponent* componentA, PhysicsComponent* componentB) {
        if (!componentA || !componentB) return false;
        
        // Get the game objects
        GameObject* objA = componentA->getOwner();
        GameObject* objB = componentB->getOwner();
        
        if (!objA || !objB) return false;
        
        // Use enhanced collision system
        return CollisionSystem::checkCollision(objA, objB);
    }
    
    bool PhysicsWorld::checkCollisionRB(RigidBodyComponent* bodyA, RigidBodyComponent* bodyB) {
        if (!bodyA || !bodyB) return false;
        
        // Get the game objects
        GameObject* objA = bodyA->getOwner();
        GameObject* objB = bodyB->getOwner();
        
        if (!objA || !objB) return false;
        
        // Use enhanced collision system
        return CollisionSystem::checkCollision(objA, objB);
    }
    
    bool PhysicsWorld::checkCollisionCR(PhysicsComponent* component, RigidBodyComponent* rigidBody) {
        if (!component || !rigidBody) return false;
        
        // Get the game objects
        GameObject* objA = component->getOwner();
        GameObject* objB = rigidBody->getOwner();
        
        if (!objA || !objB) return false;
        
        // Use enhanced collision system
        return CollisionSystem::checkCollision(objA, objB);
    }
    
    RaycastHit PhysicsWorld::raycast(const glm::vec3& origin, const glm::vec3& direction, float maxDistance) {
        RaycastHit result;
        result.hit = false;
        result.point = glm::vec3(0.0f);
        result.normal = glm::vec3(0.0f);
        result.distance = 0.0f;
        result.object = nullptr;
        
        // In a full implementation, we would check for intersections with all objects
        // This is a simplified placeholder implementation
        SPARKY_LOG_DEBUG("Raycast performed from (" + std::to_string(origin.x) + ", " + 
                        std::to_string(origin.y) + ", " + std::to_string(origin.z) + ")");
        
        return result;
    }
    
    void PhysicsWorld::resolveCollisions(const std::vector<CollisionData>& collisions) {
        // In a full implementation, we would resolve all collisions
        // This is a simplified placeholder implementation
        for (const auto& collision : collisions) {
            if (!collision.resolved) {
                resolveCollision(const_cast<CollisionData&>(collision));
            }
        }
        
        for (auto& collision : collisionsRB) {
            if (!collision.resolved) {
                resolveCollisionRB(collision);
            }
        }
        
        for (auto& collision : collisionsCR) {
            if (!collision.resolved) {
                resolveCollisionCR(collision);
            }
        }
    }
    
    void PhysicsWorld::resolveCollision(CollisionData& collision) {
        // In a full implementation, we would resolve the collision
        // This is a simplified placeholder implementation
        collision.resolved = true;
        SPARKY_LOG_DEBUG("Collision resolved between PhysicsComponents");
    }
    
    void PhysicsWorld::resolveCollisionRB(CollisionDataRB& collision) {
        // In a full implementation, we would resolve the collision
        // This is a simplified placeholder implementation
        collision.resolved = true;
        SPARKY_LOG_DEBUG("Collision resolved between RigidBodyComponents");
    }
    
    void PhysicsWorld::resolveCollisionCR(CollisionDataCR& collision) {
        // In a full implementation, we would resolve the collision
        // This is a simplified placeholder implementation
        collision.resolved = true;
        SPARKY_LOG_DEBUG("Collision resolved between PhysicsComponent and RigidBodyComponent");
    }
}