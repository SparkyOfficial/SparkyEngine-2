#include "../include/CharacterController.h"
#include "../include/GameObject.h"
#include "../include/PhysicsWorld.h"
#include "../include/Logger.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Sparky {

    CharacterController::CharacterController() : 
        rigidBody(nullptr),
        moveSpeed(5.0f),
        jumpForce(10.0f),
        grounded(false),
        groundCheckDistance(0.1f),
        moveDirection(0.0f),
        jumpRequested(false) {
    }

    CharacterController::~CharacterController() {
    }

    void CharacterController::update(float deltaTime) {
        if (!rigidBody || !owner) {
            return;
        }
        
        // Check if character is grounded
        glm::vec3 position = owner->getPosition();
        glm::vec3 down = glm::vec3(0.0f, -1.0f, 0.0f);
        
        // Simple raycast to check ground
        RaycastHit hit = PhysicsWorld::getInstance().raycast(
            position, 
            down, 
            groundCheckDistance
        );
        
        grounded = hit.hit;
        
        // Apply movement
        if (glm::length(moveDirection) > 0.0f) {
            glm::vec3 velocity = rigidBody->getLinearVelocity();
            velocity.x = moveDirection.x * moveSpeed;
            velocity.z = moveDirection.z * moveSpeed;
            rigidBody->setLinearVelocity(velocity);
        }
        
        // Apply jump
        if (jumpRequested && grounded) {
            glm::vec3 velocity = rigidBody->getLinearVelocity();
            velocity.y = jumpForce;
            rigidBody->setLinearVelocity(velocity);
            grounded = false;
        }
        
        // Reset movement direction and jump request
        moveDirection = glm::vec3(0.0f);
        jumpRequested = false;
    }

    void CharacterController::render() {
        // Character controller doesn't need rendering
    }

    void CharacterController::move(const glm::vec3& direction) {
        moveDirection = direction;
    }

    void CharacterController::jump() {
        jumpRequested = true;
    }

    void CharacterController::setMoveSpeed(float speed) {
        moveSpeed = speed;
    }

    void CharacterController::setJumpForce(float force) {
        jumpForce = force;
    }
    
    void CharacterController::setRigidBody(RigidBodyComponent* rigidBody) {
        this->rigidBody = rigidBody;
    }
}