#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace Sparky {
    class GameObject;
    class RigidBodyComponent;

    struct Collision {
        GameObject* objectA;
        GameObject* objectB;
        RigidBodyComponent* rigidBodyA;
        RigidBodyComponent* rigidBodyB;
        glm::vec3 contactPoint;
        glm::vec3 normal;
        float penetrationDepth;
        float restitution; // Combined restitution
        float friction;    // Combined friction
    };

    // Collision shapes
    enum class CollisionShape {
        AABB,
        SPHERE,
        CAPSULE,
        MESH
    };

    struct CollisionShapeData {
        CollisionShape shape;
        glm::vec3 position;
        glm::vec3 size;     // For AABB
        float radius;       // For Sphere/Capsule
        float height;       // For Capsule
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
        static bool checkCapsuleCapsuleCollision(const glm::vec3& posA, float radiusA, float heightA,
                                               const glm::vec3& posB, float radiusB, float heightB);
        static bool checkCapsuleAABBCollision(const glm::vec3& capsulePos, float capsuleRadius, float capsuleHeight,
                                            const glm::vec3& boxPos, const glm::vec3& boxSize);
        
        static std::vector<Collision> detectCollisions(const std::vector<GameObject*>& objects);
        
        // Resolve collisions
        static void resolveCollision(Collision& collision);
        
        // Collision shape utilities
        static CollisionShapeData getCollisionShapeData(GameObject* object);
        
        // Contact manifold generation
        static void generateContactManifold(Collision& collision);
    };
}