#pragma once

#include "PhysicsComponent.h"
#include <glm/glm.hpp>

namespace Sparky {
    enum class BodyType {
        STATIC,
        KINEMATIC,
        DYNAMIC
    };

    class RigidBodyComponent : public PhysicsComponent {
    public:
        RigidBodyComponent();
        virtual ~RigidBodyComponent();

        void update(float deltaTime) override;
        void render() override;

        // Rigid body properties
        void setBodyType(BodyType type);
        BodyType getBodyType() const { return bodyType; }

        void setLinearVelocity(const glm::vec3& velocity);
        glm::vec3 getLinearVelocity() const { return linearVelocity; }

        void setAngularVelocity(const glm::vec3& velocity);
        glm::vec3 getAngularVelocity() const { return angularVelocity; }

        void setLinearDamping(float damping);
        float getLinearDamping() const { return linearDamping; }

        void setAngularDamping(float damping);
        float getAngularDamping() const { return angularDamping; }

        void setRestitution(float restitution);
        float getRestitution() const { return restitution; }

        void setFriction(float friction);
        float getFriction() const { return friction; }

        // Gravity
        void setAffectedByGravity(bool affected);
        bool isAffectedByGravity() const { return affectedByGravity; }

        // Forces and torques
        void addForce(const glm::vec3& force);
        void addForceAtPosition(const glm::vec3& force, const glm::vec3& position);
        void addTorque(const glm::vec3& torque);
        void clearForces();
        void applyForce(const glm::vec3& force); // Add this method

        // Integration methods
        void integrateForces(float deltaTime);
        void integrateVelocity(float deltaTime);

    private:
        BodyType bodyType;
        
        glm::vec3 linearVelocity;
        glm::vec3 angularVelocity;
        
        glm::vec3 totalForce;
        glm::vec3 totalTorque;
        
        float linearDamping;
        float angularDamping;
        
        float restitution; // Bounciness
        float friction;    // Surface friction
        
        bool affectedByGravity; // Add this member
        
        float inverseMass;
        glm::mat3 inverseInertiaTensor;
    };
}