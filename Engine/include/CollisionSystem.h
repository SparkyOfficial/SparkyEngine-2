#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace Sparky {
    class GameObject;

    struct Collision {
        GameObject* objectA;
        GameObject* objectB;
        glm::vec3 contactPoint;
        glm::vec3 normal;
        float penetrationDepth;
    };

    class CollisionSystem {
    public:
        static bool checkCollision(GameObject* objectA, GameObject* objectB);
        static bool checkAABBCollision(const glm::vec3& posA, const glm::vec3& sizeA, 
                                      const glm::vec3& posB, const glm::vec3& sizeB);
        static bool checkSphereCollision(const glm::vec3& posA, float radiusA,
                                       const glm::vec3& posB, float radiusB);
        static bool checkSphereAABBCollision(const glm::vec3& spherePos, float sphereRadius,
                                           const glm::vec3& boxPos, const glm::vec3& boxSize);
        static std::vector<Collision> detectCollisions(const std::vector<GameObject*>& objects);
        
        // Resolve collisions
        static void resolveCollision(Collision& collision);
    };
}