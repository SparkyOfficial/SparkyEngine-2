#include "../include/PhysicsWorld.h"
#include "../include/PhysicsComponent.h"
#include "../include/GameObject.h"
#include "../include/Logger.h"
#include "../include/Profiler.h"
#include <algorithm>

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
        
        // Update all physics components
        for (auto component : components) {
            if (component && component->getOwner()) {
                // Apply gravity
                glm::vec3 acceleration = component->getAcceleration();
                acceleration += gravity * component->getMass();
                component->setAcceleration(acceleration);

                // Update velocity
                glm::vec3 velocity = component->getVelocity();
                velocity += acceleration * deltaTime;
                component->setVelocity(velocity);

                // Update position
                glm::vec3 position = component->getOwner()->getPosition();
                position += velocity * deltaTime;
                component->getOwner()->setPosition(position);

                // Reset acceleration for next frame
                component->setAcceleration(glm::vec3(0.0f));
            }
        }

        // Check for collisions
        for (size_t i = 0; i < components.size(); i++) {
            for (size_t j = i + 1; j < components.size(); j++) {
                if (checkCollision(components[i], components[j])) {
                    // Handle collision response
                    // In a more advanced implementation, we would calculate proper collision response
                    SPARKY_LOG_DEBUG("Collision detected between physics components");
                }
            }
        }
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

    void PhysicsWorld::addConstraint() {
        // Future implementation for physics constraints
        // Constraints could include joints, springs, etc.
    }
}