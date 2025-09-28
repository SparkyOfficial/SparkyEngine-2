#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>

namespace Sparky {
    class PhysicsComponent;
    class GameObject;

    struct RaycastHit {
        bool hit;
        glm::vec3 point;
        glm::vec3 normal;
        float distance;
        GameObject* object;
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
        bool checkCollision(PhysicsComponent* componentA, PhysicsComponent* componentB);

        // Constraints
        void addConstraint(); // Placeholder for future implementation

    private:
        PhysicsWorld();
        ~PhysicsWorld();

        std::vector<PhysicsComponent*> components;
        glm::vec3 gravity;
    };
}