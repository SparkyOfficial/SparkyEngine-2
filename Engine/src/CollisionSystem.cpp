#include "../include/CollisionSystem.h"
#include "../include/GameObject.h"
#include "../include/RigidBodyComponent.h"
#include "../include/PhysicsComponent.h"
#include <algorithm>
#include <cmath>

namespace Sparky {

    bool CollisionSystem::checkCollision(GameObject* objectA, GameObject* objectB) {
        if (!objectA || !objectB) {
            return false;
        }

        // For now, we'll assume all objects are AABBs (Axis-Aligned Bounding Boxes)
        glm::vec3 posA = objectA->getPosition();
        glm::vec3 scaleA = objectA->getScale();
        glm::vec3 posB = objectB->getPosition();
        glm::vec3 scaleB = objectB->getScale();

        return checkAABBCollision(posA, scaleA, posB, scaleB);
    }

    bool CollisionSystem::checkAABBCollision(const glm::vec3& posA, const glm::vec3& sizeA, 
                                           const glm::vec3& posB, const glm::vec3& sizeB) {
        // Check for overlap on each axis
        bool xOverlap = (posA.x - sizeA.x/2.0f < posB.x + sizeB.x/2.0f) && 
                        (posA.x + sizeA.x/2.0f > posB.x - sizeB.x/2.0f);
        
        bool yOverlap = (posA.y - sizeA.y/2.0f < posB.y + sizeB.y/2.0f) && 
                        (posA.y + sizeA.y/2.0f > posB.y - sizeB.y/2.0f);
        
        bool zOverlap = (posA.z - sizeA.z/2.0f < posB.z + sizeB.z/2.0f) && 
                        (posA.z + sizeA.z/2.0f > posB.z - sizeB.z/2.0f);

        return xOverlap && yOverlap && zOverlap;
    }
    
    // Add sphere collision detection
    bool CollisionSystem::checkSphereCollision(const glm::vec3& posA, float radiusA,
                                             const glm::vec3& posB, float radiusB) {
        float distance = glm::distance(posA, posB);
        return distance < (radiusA + radiusB);
    }
    
    // Add sphere vs AABB collision detection
    bool CollisionSystem::checkSphereAABBCollision(const glm::vec3& spherePos, float sphereRadius,
                                                 const glm::vec3& boxPos, const glm::vec3& boxSize) {
        // Find the closest point on the AABB to the sphere
        glm::vec3 closestPoint;
        closestPoint.x = std::max(boxPos.x - boxSize.x/2.0f, std::min(spherePos.x, boxPos.x + boxSize.x/2.0f));
        closestPoint.y = std::max(boxPos.y - boxSize.y/2.0f, std::min(spherePos.y, boxPos.y + boxSize.y/2.0f));
        closestPoint.z = std::max(boxPos.z - boxSize.z/2.0f, std::min(spherePos.z, boxPos.z + boxSize.z/2.0f));
        
        // Check if the distance from the sphere center to the closest point is less than the radius
        float distance = glm::distance(spherePos, closestPoint);
        return distance < sphereRadius;
    }

    std::vector<Collision> CollisionSystem::detectCollisions(const std::vector<GameObject*>& objects) {
        std::vector<Collision> collisions;
        
        for (size_t i = 0; i < objects.size(); i++) {
            for (size_t j = i + 1; j < objects.size(); j++) {
                if (checkCollision(objects[i], objects[j])) {
                    Collision collision;
                    collision.objectA = objects[i];
                    collision.objectB = objects[j];
                    
                    // Calculate contact point, normal, and penetration depth for AABB collision
                    glm::vec3 posA = objects[i]->getPosition();
                    glm::vec3 scaleA = objects[i]->getScale();
                    glm::vec3 posB = objects[j]->getPosition();
                    glm::vec3 scaleB = objects[j]->getScale();
                    
                    // Calculate half-sizes
                    glm::vec3 halfA = scaleA * 0.5f;
                    glm::vec3 halfB = scaleB * 0.5f;
                    
                    // Vector between centers
                    glm::vec3 delta = posB - posA;
                    
                    // Calculate overlap on each axis
                    glm::vec3 overlap;
                    overlap.x = halfA.x + halfB.x - std::abs(delta.x);
                    overlap.y = halfA.y + halfB.y - std::abs(delta.y);
                    overlap.z = halfA.z + halfB.z - std::abs(delta.z);
                    
                    // Find axis with minimum overlap (separating axis)
                    if (overlap.x < overlap.y && overlap.x < overlap.z) {
                        // X axis has minimum overlap
                        collision.normal = glm::vec3((delta.x > 0) ? 1.0f : -1.0f, 0.0f, 0.0f);
                        collision.penetrationDepth = overlap.x;
                        // Contact point is at the center of the overlapping region on the face
                        collision.contactPoint = posA + glm::vec3(
                            (delta.x > 0) ? halfA.x : -halfA.x,
                            delta.y,
                            delta.z
                        );
                    } else if (overlap.y < overlap.z) {
                        // Y axis has minimum overlap
                        collision.normal = glm::vec3(0.0f, (delta.y > 0) ? 1.0f : -1.0f, 0.0f);
                        collision.penetrationDepth = overlap.y;
                        // Contact point is at the center of the overlapping region on the face
                        collision.contactPoint = posA + glm::vec3(
                            delta.x,
                            (delta.y > 0) ? halfA.y : -halfA.y,
                            delta.z
                        );
                    } else {
                        // Z axis has minimum overlap
                        collision.normal = glm::vec3(0.0f, 0.0f, (delta.z > 0) ? 1.0f : -1.0f);
                        collision.penetrationDepth = overlap.z;
                        // Contact point is at the center of the overlapping region on the face
                        collision.contactPoint = posA + glm::vec3(
                            delta.x,
                            delta.y,
                            (delta.z > 0) ? halfA.z : -halfA.z
                        );
                    }
                    
                    collisions.push_back(collision);
                }
            }
        }
        
        return collisions;
    }

    void CollisionSystem::resolveCollision(Collision& collision) {
        // More complex collision resolution with proper impulse-based physics
        if (collision.objectA && collision.objectB) {
            glm::vec3 posA = collision.objectA->getPosition();
            glm::vec3 scaleA = collision.objectA->getScale();
            glm::vec3 posB = collision.objectB->getPosition();
            glm::vec3 scaleB = collision.objectB->getScale();
            
            // Get physics components to calculate real velocities
            Sparky::PhysicsComponent* physicsA = collision.objectA->getComponent<Sparky::PhysicsComponent>();
            Sparky::PhysicsComponent* physicsB = collision.objectB->getComponent<Sparky::PhysicsComponent>();
            
            // Get velocities from physics components if they exist
            glm::vec3 velA(0.0f);
            glm::vec3 velB(0.0f);
            
            if (physicsA) {
                velA = physicsA->getVelocity();
            }
            
            if (physicsB) {
                velB = physicsB->getVelocity();
            }
            
            // Calculate relative velocity
            glm::vec3 relativeVelocity = velA - velB;
            
            // Calculate impulse
            float restitution = 0.5f; // Coefficient of restitution (bounciness)
            float impulse = -(1.0f + restitution) * glm::dot(relativeVelocity, collision.normal);
            
            // Apply impulse to separate objects
            // Apply forces to the objects' rigid bodies
            Sparky::RigidBodyComponent* rigidBodyA = collision.objectA->getComponent<Sparky::RigidBodyComponent>();
            Sparky::RigidBodyComponent* rigidBodyB = collision.objectB->getComponent<Sparky::RigidBodyComponent>();
            
            if (rigidBodyA && rigidBodyB) {
                // Apply the impulse to the objects' velocities
                glm::vec3 impulseVector = impulse * collision.normal;
                rigidBodyA->setLinearVelocity(rigidBodyA->getLinearVelocity() + impulseVector);
                rigidBodyB->setLinearVelocity(rigidBodyB->getLinearVelocity() - impulseVector);
            }
            
            // Positional correction to prevent sinking
            const float percent = 0.2f; // Penetration percentage to correct
            const float slop = 0.01f;   // Penetration allowance
            glm::vec3 correction = (glm::max(collision.penetrationDepth - slop, 0.0f) / 2.0f) * percent * collision.normal;
            
            posA += correction;
            posB -= correction;
            
            collision.objectA->setPosition(posA);
            collision.objectB->setPosition(posB);
        }
    }
}