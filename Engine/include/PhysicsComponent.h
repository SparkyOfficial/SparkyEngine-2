#pragma once

#include "Component.h"
#include <glm/glm.hpp>

namespace Sparky {
    class PhysicsComponent : public Component {
    public:
        PhysicsComponent();
        virtual ~PhysicsComponent();

        void update(float deltaTime) override;
        void render() override;

        // Physics properties
        void setVelocity(const glm::vec3& velocity);
        void setAcceleration(const glm::vec3& acceleration);
        void setMass(float mass);
        void setGravity(const glm::vec3& gravity);

        glm::vec3 getVelocity() const { return velocity; }
        glm::vec3 getAcceleration() const { return acceleration; }
        float getMass() const { return mass; }
        glm::vec3 getGravity() const { return gravity; }

        // Physics state
        void setOnGround(bool onGround);
        bool isOnGround() const { return onGround; }

    private:
        glm::vec3 velocity;
        glm::vec3 acceleration;
        glm::vec3 gravity;
        float mass;
        bool onGround;
    };
}