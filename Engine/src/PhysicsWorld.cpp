#include "../include/PhysicsWorld.h"
#include "../include/PhysicsComponent.h"
#include "../include/RigidBodyComponent.h"
#include "../include/GameObject.h"
#include "../include/Logger.h"
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
        
        // Simple sphere collision detection
        glm::vec3 posA = objA->getPosition();
        glm::vec3 posB = objB->getPosition();
        float distance = glm::distance(posA, posB);
        
        // For now, we'll assume a fixed radius of 1.0 for all objects
        // In a full implementation, we would get the actual collision shapes
        float radiusA = 1.0f;
        float radiusB = 1.0f;
        
        return distance < (radiusA + radiusB);
    }
    
    bool PhysicsWorld::checkCollisionRB(RigidBodyComponent* bodyA, RigidBodyComponent* bodyB) {
        if (!bodyA || !bodyB) return false;
        
        // Get the game objects
        GameObject* objA = bodyA->getOwner();
        GameObject* objB = bodyB->getOwner();
        
        if (!objA || !objB) return false;
        
        // Simple sphere collision detection
        glm::vec3 posA = objA->getPosition();
        glm::vec3 posB = objB->getPosition();
        float distance = glm::distance(posA, posB);
        
        // For now, we'll assume a fixed radius of 1.0 for all objects
        // In a full implementation, we would get the actual collision shapes
        float radiusA = 1.0f;
        float radiusB = 1.0f;
        
        return distance < (radiusA + radiusB);
    }
    
    bool PhysicsWorld::checkCollisionCR(PhysicsComponent* component, RigidBodyComponent* rigidBody) {
        if (!component || !rigidBody) return false;
        
        // Get the game objects
        GameObject* objA = component->getOwner();
        GameObject* objB = rigidBody->getOwner();
        
        if (!objA || !objB) return false;
        
        // Simple sphere collision detection
        glm::vec3 posA = objA->getPosition();
        glm::vec3 posB = objB->getPosition();
        float distance = glm::distance(posA, posB);
        
        // For now, we'll assume a fixed radius of 1.0 for all objects
        // In a full implementation, we would get the actual collision shapes
        float radiusA = 1.0f;
        float radiusB = 1.0f;
        
        return distance < (radiusA + radiusB);
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
        // Simple collision response - just mark as resolved
        // In a full implementation, we would apply forces and update positions
        collision.resolved = true;
        SPARKY_LOG_DEBUG("Collision resolved between PhysicsComponents");
    }
    
    void PhysicsWorld::resolveCollisionRB(CollisionDataRB& collision) {
        if (!collision.bodyA || !collision.bodyB) return;
        
        // Get the game objects
        GameObject* objA = collision.bodyA->getOwner();
        GameObject* objB = collision.bodyB->getOwner();
        
        if (!objA || !objB) return;
        
        // Simple elastic collision response
        glm::vec3 posA = objA->getPosition();
        glm::vec3 posB = objB->getPosition();
        glm::vec3 velA = collision.bodyA->getVelocity();
        glm::vec3 velB = collision.bodyB->getVelocity();
        float massA = collision.bodyA->getMass();
        float massB = collision.bodyB->getMass();
        
        // Calculate collision normal
        glm::vec3 normal = glm::normalize(posB - posA);
        
        // Calculate relative velocity
        glm::vec3 relVel = velB - velA;
        
        // Calculate velocity along normal
        float velAlongNormal = glm::dot(relVel, normal);
        
        // Do not resolve if velocities are separating
        if (velAlongNormal > 0) return;
        
        // Calculate restitution (bounciness)
        float restitution = 0.8f; // Default value
        
        // Calculate impulse scalar
        float j = -(1 + restitution) * velAlongNormal;
        j /= 1/massA + 1/massB;
        
        // Apply impulse
        glm::vec3 impulse = j * normal;
        collision.bodyA->setVelocity(velA - 1/massA * impulse);
        collision.bodyB->setVelocity(velB + 1/massB * impulse);
        
        // Mark as resolved
        collision.resolved = true;
        SPARKY_LOG_DEBUG("RigidBody collision resolved");
    }
    
    void PhysicsWorld::resolveCollisionCR(CollisionDataCR& collision) {
        if (!collision.component || !collision.rigidBody) return;
        
        // Get the game objects
        GameObject* objA = collision.component->getOwner();
        GameObject* objB = collision.rigidBody->getOwner();
        
        if (!objA || !objB) return;
        
        // Simple response - stop the rigid body
        collision.rigidBody->setVelocity(glm::vec3(0.0f));
        
        // Mark as resolved
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