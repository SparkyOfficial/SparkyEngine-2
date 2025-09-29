#include "../include/PhysicsWorld.h"
#include "../include/PhysicsComponent.h"
#include "../include/RigidBodyComponent.h"
#include "../include/GameObject.h"
#include "../include/Logger.h"
#include "../include/Profiler.h"
#include <algorithm>
#include <cmath>

namespace Sparky {

    PhysicsWorld::PhysicsWorld() : gravity(0.0f, -9.81f, 0.0f) {
    }

    PhysicsWorld::~PhysicsWorld() {
    }

    PhysicsWorld& PhysicsWorld::getInstance() {
        static PhysicsWorld instance;
        return instance;
    }

    void PhysicsWorld::addPhysicsComponent(PhysicsComponent* component) {
        if (component && std::find(components.begin(), components.end(), component) == components.end()) {
            components.push_back(component);
            SPARKY_LOG_DEBUG("Added physics component to world");
        }
    }

    void PhysicsWorld::removePhysicsComponent(PhysicsComponent* component) {
        if (component) {
            components.erase(std::remove(components.begin(), components.end(), component), components.end());
            SPARKY_LOG_DEBUG("Removed physics component from world");
        }
    }

    void PhysicsWorld::update(float deltaTime) {
        SPARKY_PROFILE("PhysicsUpdate");
        
        // Broadphase collision detection
        broadphase();
        
        // Update all physics components
        for (auto component : components) {
            if (component && component->getOwner()) {
                component->update(deltaTime);
            }
        }

        // Detect collisions
        std::vector<CollisionData> collisions = detectCollisions();
        
        // Resolve collisions
        resolveCollisions(collisions);
    }

    void PhysicsWorld::setGravity(const glm::vec3& gravity) {
        this->gravity = gravity;
        SPARKY_LOG_DEBUG("Gravity set to (" + std::to_string(gravity.x) + ", " + 
                        std::to_string(gravity.y) + ", " + std::to_string(gravity.z) + ")");
    }

    RaycastHit PhysicsWorld::raycast(const glm::vec3& origin, const glm::vec3& direction, float maxDistance) {
        RaycastHit result;
        result.hit = false;
        result.distance = maxDistance;
        result.object = nullptr;

        // Normalize direction
        glm::vec3 normalizedDirection = glm::normalize(direction);

        // Check intersection with all physics components
        for (auto component : components) {
            if (component && component->getOwner()) {
                glm::vec3 objectPos = component->getOwner()->getPosition();
                glm::vec3 objectScale = component->getOwner()->getScale();

                // Simple sphere-ray intersection for now
                glm::vec3 toObject = objectPos - origin;
                float t = glm::dot(toObject, normalizedDirection);
                
                if (t >= 0 && t <= result.distance) {
                    glm::vec3 closestPoint = origin + normalizedDirection * t;
                    float distance = glm::length(closestPoint - objectPos);
                    
                    // Approximate radius as average of scale components
                    float radius = (objectScale.x + objectScale.y + objectScale.z) / 3.0f * 0.5f;
                    
                    if (distance <= radius) {
                        result.hit = true;
                        result.point = closestPoint;
                        result.normal = glm::normalize(closestPoint - objectPos);
                        result.distance = t;
                        result.object = component->getOwner();
                    }
                }
            }
        }

        return result;
    }

    std::vector<CollisionData> PhysicsWorld::detectCollisions() {
        std::vector<CollisionData> collisions;
        
        // Check potential collisions from broadphase
        for (const auto& pair : broadphasePairs) {
            CollisionData collision;
            if (checkCollision(pair.first, pair.second)) {
                collision.componentA = pair.first;
                collision.componentB = pair.second;
                collision.resolved = false;
                // For simplicity, we'll use a basic contact point calculation
                collision.contactPoint = (pair.first->getOwner()->getPosition() + 
                                        pair.second->getOwner()->getPosition()) * 0.5f;
                collision.normal = glm::normalize(pair.second->getOwner()->getPosition() - 
                                                pair.first->getOwner()->getPosition());
                collision.penetrationDepth = 0.1f; // Simplified value
                collisions.push_back(collision);
            }
        }
        
        return collisions;
    }

    bool PhysicsWorld::checkCollision(PhysicsComponent* componentA, PhysicsComponent* componentB) {
        if (!componentA || !componentB || !componentA->getOwner() || !componentB->getOwner()) {
            return false;
        }

        GameObject* objectA = componentA->getOwner();
        GameObject* objectB = componentB->getOwner();

        glm::vec3 posA = objectA->getPosition();
        glm::vec3 scaleA = objectA->getScale();
        glm::vec3 posB = objectB->getPosition();
        glm::vec3 scaleB = objectB->getScale();

        // Simple AABB collision check
        bool xOverlap = (posA.x - scaleA.x/2.0f < posB.x + scaleB.x/2.0f) && 
                        (posA.x + scaleA.x/2.0f > posB.x - scaleB.x/2.0f);
        
        bool yOverlap = (posA.y - scaleA.y/2.0f < posB.y + scaleB.y/2.0f) && 
                        (posA.y + scaleA.y/2.0f > posB.y - scaleB.y/2.0f);
        
        bool zOverlap = (posA.z - scaleA.z/2.0f < posB.z + scaleB.z/2.0f) && 
                        (posA.z + scaleA.z/2.0f > posB.z - scaleB.z/2.0f);

        return xOverlap && yOverlap && zOverlap;
    }

    void PhysicsWorld::resolveCollisions(const std::vector<CollisionData>& collisions) {
        for (auto& collision : collisions) {
            if (!collision.resolved) {
                resolveCollision(const_cast<CollisionData&>(collision));
            }
        }
    }

    void PhysicsWorld::resolveCollision(CollisionData& collision) {
        // Get the rigid body components if they exist
        RigidBodyComponent* rbA = dynamic_cast<RigidBodyComponent*>(collision.componentA);
        RigidBodyComponent* rbB = dynamic_cast<RigidBodyComponent*>(collision.componentB);
        
        // If both are rigid bodies, apply collision response
        if (rbA && rbB) {
            // Calculate relative velocity
            glm::vec3 relativeVelocity = rbA->getLinearVelocity() - rbB->getLinearVelocity();
            
            // Calculate velocity along the normal
            float velocityAlongNormal = glm::dot(relativeVelocity, collision.normal);
            
            // Do not resolve if objects are separating
            if (velocityAlongNormal > 0) {
                return;
            }
            
            // Calculate restitution (bounciness)
            float restitution = (rbA->getRestitution() + rbB->getRestitution()) * 0.5f;
            
            // Calculate impulse scalar
            float impulseScalar = -(1 + restitution) * velocityAlongNormal;
            impulseScalar /= 2.0f; // Simplified mass calculation
            
            // Apply impulse
            glm::vec3 impulse = impulseScalar * collision.normal;
            rbA->setLinearVelocity(rbA->getLinearVelocity() + impulse);
            rbB->setLinearVelocity(rbB->getLinearVelocity() - impulse);
            
            // Positional correction to prevent sinking
            const float percent = 0.2f; // Penetration percentage to correct
            const float slop = 0.01f;   // Penetration allowance
            glm::vec3 correction = (glm::max(collision.penetrationDepth - slop, 0.0f) / 2.0f) * percent * collision.normal;
            
            if (rbA->getBodyType() == BodyType::DYNAMIC) {
                glm::vec3 posA = rbA->getOwner()->getPosition();
                rbA->getOwner()->setPosition(posA + correction);
            }
            
            if (rbB->getBodyType() == BodyType::DYNAMIC) {
                glm::vec3 posB = rbB->getOwner()->getPosition();
                rbB->getOwner()->setPosition(posB - correction);
            }
        }
        
        collision.resolved = true;
    }

    void PhysicsWorld::addConstraint() {
        // Future implementation for physics constraints
        // Constraints could include joints, springs, etc.
    }
    
    void PhysicsWorld::broadphase() {
        // Simple broadphase: check all pairs (in a real engine, this would use spatial partitioning)
        broadphasePairs.clear();
        
        for (size_t i = 0; i < components.size(); i++) {
            for (size_t j = i + 1; j < components.size(); j++) {
                // Simple distance check for broadphase
                glm::vec3 posA = components[i]->getOwner()->getPosition();
                glm::vec3 posB = components[j]->getOwner()->getPosition();
                float distance = glm::length(posA - posB);
                
                // If objects are close enough, add to potential collision pairs
                if (distance < 10.0f) { // Arbitrary distance threshold
                    broadphasePairs.push_back({components[i], components[j]});
                }
            }
        }
    }
}