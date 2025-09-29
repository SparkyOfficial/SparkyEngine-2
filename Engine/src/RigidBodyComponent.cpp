#include "../include/RigidBodyComponent.h"
#include "../include/GameObject.h"
#include "../include/Logger.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Sparky {

    RigidBodyComponent::RigidBodyComponent() : 
        PhysicsComponent(),
        bodyType(BodyType::DYNAMIC),
        linearVelocity(0.0f),
        angularVelocity(0.0f),
        totalForce(0.0f),
        totalTorque(0.0f),
        linearDamping(0.0f),
        angularDamping(0.0f),
        restitution(0.3f),
        friction(0.5f),
        inverseMass(1.0f),
        inverseInertiaTensor(1.0f) {
    }

    RigidBodyComponent::~RigidBodyComponent() {
    }

    void RigidBodyComponent::update(float deltaTime) {
        // Integrate forces and velocities
        integrateForces(deltaTime);
        integrateVelocity(deltaTime);
        
        // Apply damping
        linearVelocity *= glm::pow(1.0f - linearDamping, deltaTime);
        angularVelocity *= glm::pow(1.0f - angularDamping, deltaTime);
        
        // Update position and rotation of the owner object
        if (owner) {
            // Update position
            glm::vec3 position = owner->getPosition();
            position += linearVelocity * deltaTime;
            owner->setPosition(position);
            
            // Update rotation
            glm::vec3 rotation = owner->getRotation();
            rotation += angularVelocity * deltaTime;
            owner->setRotation(rotation);
        }
        
        // Clear forces for next frame
        clearForces();
    }

    void RigidBodyComponent::render() {
        // Rigid body components don't need rendering
    }

    void RigidBodyComponent::setBodyType(BodyType type) {
        bodyType = type;
        
        // Update mass properties based on body type
        if (type == BodyType::STATIC) {
            inverseMass = 0.0f;
        } else if (type == BodyType::KINEMATIC) {
            inverseMass = 0.0f;
        }
    }

    void RigidBodyComponent::setLinearVelocity(const glm::vec3& velocity) {
        linearVelocity = velocity;
    }

    void RigidBodyComponent::setAngularVelocity(const glm::vec3& velocity) {
        angularVelocity = velocity;
    }

    void RigidBodyComponent::setLinearDamping(float damping) {
        linearDamping = damping;
    }

    void RigidBodyComponent::setAngularDamping(float damping) {
        angularDamping = damping;
    }

    void RigidBodyComponent::setRestitution(float restitution) {
        this->restitution = restitution;
    }

    void RigidBodyComponent::setFriction(float friction) {
        this->friction = friction;
    }

    void RigidBodyComponent::addForce(const glm::vec3& force) {
        totalForce += force;
    }

    void RigidBodyComponent::addForceAtPosition(const glm::vec3& force, const glm::vec3& position) {
        totalForce += force;
        
        // Calculate torque from force applied at position
        if (owner) {
            glm::vec3 centerOfMass = owner->getPosition();
            glm::vec3 torque = glm::cross(position - centerOfMass, force);
            totalTorque += torque;
        }
    }

    void RigidBodyComponent::addTorque(const glm::vec3& torque) {
        totalTorque += torque;
    }

    void RigidBodyComponent::clearForces() {
        totalForce = glm::vec3(0.0f);
        totalTorque = glm::vec3(0.0f);
    }

    void RigidBodyComponent::integrateForces(float deltaTime) {
        if (bodyType != BodyType::DYNAMIC) return;
        
        // Apply gravity if inherited from PhysicsComponent
        glm::vec3 gravity = getGravity();
        if (glm::length(gravity) > 0.0f) {
            totalForce += gravity * (1.0f / inverseMass);
        }
        
        // Integrate acceleration to velocity
        linearVelocity += totalForce * inverseMass * deltaTime;
        angularVelocity += (inverseInertiaTensor * totalTorque) * deltaTime;
    }

    void RigidBodyComponent::integrateVelocity(float deltaTime) {
        if (bodyType != BodyType::DYNAMIC) return;
        
        // Integrate velocity to position is done in update()
        // This method is kept for potential future enhancements
    }
}