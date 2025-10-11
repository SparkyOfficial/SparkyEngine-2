#include "../include/PhysicsWorld.h"
#include "../include/PhysicsComponent.h"
#include "../include/RigidBodyComponent.h"
#include "../include/GameObject.h"
#include "../include/Logger.h"
#include "../include/CollisionSystem.h"
#include <algorithm>
#include <cmath>

namespace Sparky {
    
    PhysicsWorld& PhysicsWorld::getInstance() {
        static PhysicsWorld instance;
        return instance;
    }
    
    PhysicsWorld::PhysicsWorld() : gravity(0.0f, -9.81f, 0.0f) {
        SPARKY_LOG_DEBUG("PhysicsWorld created");
    }
    
    PhysicsWorld::~PhysicsWorld() {
        SPARKY_LOG_DEBUG("PhysicsWorld destroyed");
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
    
    void PhysicsWorld::resolveCollisions(const std::vector<CollisionData>& collisions) {
        // Resolve physics component collisions
        for (auto& collision : collisions) {
            if (!collision.resolved) {
                resolveCollision(const_cast<CollisionData&>(collision));
            }
        }
        
        // Resolve rigid body collisions
        for (auto& collision : collisionsRB) {
            if (!collision.resolved) {
                resolveCollisionRB(collision);
            }
        }
        
        // Resolve component-rigid body collisions
        for (auto& collision : collisionsCR) {
            if (!collision.resolved) {
                resolveCollisionCR(collision);
            }
        }
    }
    
    void PhysicsWorld::resolveCollision(CollisionData& collision) {
        if (!collision.componentA || !collision.componentB) return;
        
        // Get the game objects
        GameObject* objA = collision.componentA->getOwner();
        GameObject* objB = collision.componentB->getOwner();
        
        if (!objA || !objB) return;
        
        // Create a collision structure for the enhanced collision system
        Sparky::Collision enhancedCollision;
        enhancedCollision.objectA = objA;
        enhancedCollision.objectB = objB;
        enhancedCollision.rigidBodyA = nullptr;
        enhancedCollision.rigidBodyB = nullptr;
        enhancedCollision.contactPoint = collision.contactPoint;
        enhancedCollision.normal = collision.normal;
        enhancedCollision.penetrationDepth = collision.penetrationDepth;
        enhancedCollision.restitution = 0.3f; // Default restitution
        enhancedCollision.friction = 0.5f;    // Default friction
        
        // Use enhanced collision resolution
        CollisionSystem::resolveCollision(enhancedCollision);
        
        collision.resolved = true;
        SPARKY_LOG_DEBUG("Collision resolved between PhysicsComponents");
    }
    
    void PhysicsWorld::resolveCollisionRB(CollisionDataRB& collision) {
        if (!collision.bodyA || !collision.bodyB) return;
        
        // Get the game objects
        GameObject* objA = collision.bodyA->getOwner();
        GameObject* objB = collision.bodyB->getOwner();
        
        if (!objA || !objB) return;
        
        // Create a collision structure for the enhanced collision system
        Sparky::Collision enhancedCollision;
        enhancedCollision.objectA = objA;
        enhancedCollision.objectB = objB;
        enhancedCollision.rigidBodyA = collision.bodyA;
        enhancedCollision.rigidBodyB = collision.bodyB;
        enhancedCollision.contactPoint = collision.contactPoint;
        enhancedCollision.normal = collision.normal;
        enhancedCollision.penetrationDepth = collision.penetrationDepth;
        
        // Calculate combined restitution and friction
        float restitutionA = collision.bodyA->getRestitution();
        float restitutionB = collision.bodyB->getRestitution();
        enhancedCollision.restitution = (restitutionA + restitutionB) * 0.5f;
        
        float frictionA = collision.bodyA->getFriction();
        float frictionB = collision.bodyB->getFriction();
        enhancedCollision.friction = (frictionA + frictionB) * 0.5f;
        
        // Use enhanced collision resolution
        CollisionSystem::resolveCollision(enhancedCollision);
        
        collision.resolved = true;
        SPARKY_LOG_DEBUG("RigidBody collision resolved");
    }
    
    void PhysicsWorld::resolveCollisionCR(CollisionDataCR& collision) {
        if (!collision.component || !collision.rigidBody) return;
        
        // Get the game objects
        GameObject* objA = collision.component->getOwner();
        GameObject* objB = collision.rigidBody->getOwner();
        
        if (!objA || !objB) return;
        
        // Create a collision structure for the enhanced collision system
        Sparky::Collision enhancedCollision;
        enhancedCollision.objectA = objA;
        enhancedCollision.objectB = objB;
        enhancedCollision.rigidBodyA = nullptr; // PhysicsComponent doesn't have rigid body
        enhancedCollision.rigidBodyB = collision.rigidBody;
        enhancedCollision.contactPoint = collision.contactPoint;
        enhancedCollision.normal = collision.normal;
        enhancedCollision.penetrationDepth = collision.penetrationDepth;
        
        // Calculate combined restitution and friction
        float restitutionB = collision.rigidBody->getRestitution();
        enhancedCollision.restitution = restitutionB * 0.5f; // Average with default
        
        float frictionB = collision.rigidBody->getFriction();
        enhancedCollision.friction = frictionB * 0.5f; // Average with default
        
        // Use enhanced collision resolution
        CollisionSystem::resolveCollision(enhancedCollision);
        
        collision.resolved = true;
        SPARKY_LOG_DEBUG("Component-RigidBody collision resolved");
    }
    
    RaycastHit PhysicsWorld::raycast(const glm::vec3& origin, const glm::vec3& direction, float maxDistance) {
        RaycastHit result;
        result.hit = false;
        result.distance = maxDistance;
        result.object = nullptr;
        
        // Normalize the direction
        glm::vec3 normalizedDirection = glm::normalize(direction);
        
        // Check against all physics components
        for (auto& component : components) {
            GameObject* obj = component->getOwner();
            if (!obj) continue;
            
            // Simple sphere raycast
            glm::vec3 sphereCenter = obj->getPosition();
            float sphereRadius = 1.0f; // Default radius
            
            // Calculate intersection
            glm::vec3 oc = origin - sphereCenter;
            float a = glm::dot(normalizedDirection, normalizedDirection);
            float b = 2.0f * glm::dot(oc, normalizedDirection);
            float c = glm::dot(oc, oc) - sphereRadius * sphereRadius;
            float discriminant = b * b - 4 * a * c;
            
            if (discriminant >= 0) {
                float t = (-b - sqrt(discriminant)) / (2.0f * a);
                if (t >= 0 && t <= maxDistance && t < result.distance) {
                    result.hit = true;
                    result.distance = t;
                    result.point = origin + t * normalizedDirection;
                    result.normal = glm::normalize(result.point - sphereCenter);
                    result.object = obj;
                }
            }
        }
        
        return result;
    }
}