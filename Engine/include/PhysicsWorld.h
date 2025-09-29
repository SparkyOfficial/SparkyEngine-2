#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>

namespace Sparky {
    class PhysicsComponent;
    class RigidBodyComponent;
    class GameObject;

    struct RaycastHit {
        bool hit;
        glm::vec3 point;
        glm::vec3 normal;
        float distance;
        GameObject* object;
    };

    // Collision data structure
    struct CollisionData {
        PhysicsComponent* componentA;
        PhysicsComponent* componentB;
        glm::vec3 contactPoint;
        glm::vec3 normal;
        float penetrationDepth;
        bool resolved;
    };

    class PhysicsWorld {
    public:
        static PhysicsWorld& getInstance();

        void addPhysicsComponent(PhysicsComponent* component);
        void removePhysicsComponent(PhysicsComponent* component);

        void update(float deltaTime);
        void setGravity(const glm::vec3& gravity);

        // Collision detection
        RaycastHit raycast(const glm::vec3& origin, const glm::vec3& direction, float maxDistance);
        std::vector<CollisionData> detectCollisions();
        bool checkCollision(PhysicsComponent* componentA, PhysicsComponent* componentB);
        
        // Collision resolution
        void resolveCollisions(const std::vector<CollisionData>& collisions);
        void resolveCollision(CollisionData& collision);

        // Constraints
        void addConstraint(); // Future implementation for physics constraints

    private:
        PhysicsWorld();
        ~PhysicsWorld();

        std::vector<PhysicsComponent*> components;
        glm::vec3 gravity;
        
        // Broadphase collision optimization
        std::vector<std::pair<PhysicsComponent*, PhysicsComponent*>> broadphasePairs;
        void broadphase();
    };
}