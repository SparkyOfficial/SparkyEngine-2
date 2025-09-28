#include "../include/CollisionSystem.h"
#include "../include/GameObject.h"
#include <algorithm>

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

    std::vector<Collision> CollisionSystem::detectCollisions(const std::vector<GameObject*>& objects) {
        std::vector<Collision> collisions;
        
        for (size_t i = 0; i < objects.size(); i++) {
            for (size_t j = i + 1; j < objects.size(); j++) {
                if (checkCollision(objects[i], objects[j])) {
                    Collision collision;
                    collision.objectA = objects[i];
                    collision.objectB = objects[j];
                    // In a real implementation, we would calculate contact point, normal, and penetration depth
                    collisions.push_back(collision);
                }
            }
        }
        
        return collisions;
    }

    void CollisionSystem::resolveCollision(Collision& collision) {
        // Simple collision resolution
        // In a real implementation, this would be much more complex
        // For now, we'll just set the first object on top of the second
        
        if (collision.objectA && collision.objectB) {
            glm::vec3 posA = collision.objectA->getPosition();
            glm::vec3 scaleA = collision.objectA->getScale();
            glm::vec3 posB = collision.objectB->getPosition();
            glm::vec3 scaleB = collision.objectB->getScale();
            
            // Move object A above object B
            posA.y = posB.y + (scaleB.y/2.0f) + (scaleA.y/2.0f);
            collision.objectA->setPosition(posA);
        }
    }
}