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

    // Collision data structure for PhysicsComponents
    struct CollisionData {
        PhysicsComponent* componentA;
        PhysicsComponent* componentB;
        glm::vec3 contactPoint;
        glm::vec3 normal;
        float penetrationDepth;
        bool resolved;
    };

    // Collision data structure for RigidBodyComponents
    struct CollisionDataRB {
        RigidBodyComponent* bodyA;
        RigidBodyComponent* bodyB;
        glm::vec3 contactPoint;
        glm::vec3 normal;
        float penetrationDepth;
        bool resolved;
    };

    // Collision data structure for PhysicsComponent-RigidBodyComponent
    struct CollisionDataCR {
        PhysicsComponent* component;
        RigidBodyComponent* rigidBody;
        glm::vec3 contactPoint;
        glm::vec3 normal;
        float penetrationDepth;
        bool resolved;
    };

    class PhysicsWorld {
    public:
        // Constructor for dependency injection
        PhysicsWorld();
        PhysicsWorld(const glm::vec3& gravity);
        ~PhysicsWorld();

        static PhysicsWorld& getInstance();

        // Method to create a new PhysicsWorld instance for dependency injection
        static std::unique_ptr<PhysicsWorld> create(const glm::vec3& gravity = glm::vec3(0.0f, -9.81f, 0.0f));

        void addPhysicsComponent(PhysicsComponent* component);
        void removePhysicsComponent(PhysicsComponent* component);
        
        void addRigidBody(RigidBodyComponent* rigidBody);
        void removeRigidBody(RigidBodyComponent* rigidBody);

        void update(float deltaTime);
        void setGravity(const glm::vec3& gravity);

        // Collision detection
        RaycastHit raycast(const glm::vec3& origin, const glm::vec3& direction, float maxDistance);
        std::vector<CollisionData> detectCollisions();
        bool checkCollision(PhysicsComponent* componentA, PhysicsComponent* componentB);
        bool checkCollisionRB(RigidBodyComponent* bodyA, RigidBodyComponent* bodyB);
        bool checkCollisionCR(PhysicsComponent* component, RigidBodyComponent* rigidBody);
        
        // Collision resolution
        void resolveCollisions(const std::vector<CollisionData>& collisions);
        void resolveCollision(CollisionData& collision);
        void resolveCollisionRB(CollisionDataRB& collision);
        void resolveCollisionCR(CollisionDataCR& collision);

        // Getters
        const glm::vec3& getGravity() const { return gravity; }

    private:
        std::vector<PhysicsComponent*> components;
        std::vector<RigidBodyComponent*> rigidBodies;
        glm::vec3 gravity;
        
        // Broadphase collision optimization
        std::vector<std::pair<PhysicsComponent*, PhysicsComponent*>> broadphasePairs;
        std::vector<std::pair<RigidBodyComponent*, RigidBodyComponent*>> broadphasePairsRB;
        std::vector<std::pair<PhysicsComponent*, RigidBodyComponent*>> broadphasePairsCR;
        
        // Collision data
        std::vector<CollisionDataRB> collisionsRB;
        std::vector<CollisionDataCR> collisionsCR;
        
        void broadphase();
    };
}