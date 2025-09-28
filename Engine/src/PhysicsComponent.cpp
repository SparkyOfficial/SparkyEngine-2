#include "../include/PhysicsComponent.h"
#include "../include/GameObject.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Sparky {

    PhysicsComponent::PhysicsComponent() : velocity(0.0f), acceleration(0.0f), gravity(0.0f, -9.81f, 0.0f), 
                                          mass(1.0f), onGround(false) {
    }

    PhysicsComponent::~PhysicsComponent() {
    }

    void PhysicsComponent::update(float deltaTime) {
        if (!onGround) {
            // Apply gravity
            acceleration += gravity;
        }

        // Update velocity
        velocity += acceleration * deltaTime;

        // Update position
        if (owner) {
            glm::vec3 position = owner->getPosition();
            position += velocity * deltaTime;
            owner->setPosition(position);
        }

        // Reset acceleration
        acceleration = glm::vec3(0.0f);
    }

    void PhysicsComponent::render() {
        // Physics components don't need rendering
    }

    void PhysicsComponent::setVelocity(const glm::vec3& velocity) {
        this->velocity = velocity;
    }

    void PhysicsComponent::setAcceleration(const glm::vec3& acceleration) {
        this->acceleration = acceleration;
    }

    void PhysicsComponent::setMass(float mass) {
        this->mass = mass;
    }

    void PhysicsComponent::setGravity(const glm::vec3& gravity) {
        this->gravity = gravity;
    }

    void PhysicsComponent::setOnGround(bool onGround) {
        this->onGround = onGround;
        
        // Reset vertical velocity when on ground
        if (onGround) {
            velocity.y = 0.0f;
        }
    }
}