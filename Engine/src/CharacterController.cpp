#include "../include/CharacterController.h"
#include "../include/GameObject.h"
#include "../include/Logger.h"
#include <glm/glm.hpp>

namespace Sparky {

    CharacterController::CharacterController(GameObject* character) : 
        character(character),
        walkSpeed(5.0f), runSpeed(10.0f), jumpForce(15.0f), gravity(9.81f),
        verticalVelocity(0.0f), grounded(true), running(false), crouching(false),
        height(1.8f), crouchHeight(1.0f) {
    }

    CharacterController::~CharacterController() {
    }

    void CharacterController::move(const glm::vec3& direction, float deltaTime) {
        if (!character) return;

        // Normalize direction
        glm::vec3 normalizedDirection = glm::normalize(direction);
        
        // Apply speed based on running state
        float speed = running ? runSpeed : walkSpeed;
        
        // Calculate movement vector
        glm::vec3 movement = normalizedDirection * speed * deltaTime;
        
        // Apply movement to character
        glm::vec3 position = character->getPosition();
        position += movement;
        character->setPosition(position);
        
        SPARKY_LOG_DEBUG("Character moved: (" + std::to_string(movement.x) + ", " + 
                        std::to_string(movement.y) + ", " + std::to_string(movement.z) + ")");
    }

    void CharacterController::jump() {
        if (!character || !grounded) return;

        verticalVelocity = jumpForce;
        grounded = false;
        
        SPARKY_LOG_DEBUG("Character jumped with force: " + std::to_string(jumpForce));
    }

    void CharacterController::crouch() {
        if (!character || crouching) return;

        crouching = true;
        
        // Reduce height
        glm::vec3 scale = character->getScale();
        scale.y = crouchHeight;
        character->setScale(scale);
        
        SPARKY_LOG_DEBUG("Character crouched");
    }

    void CharacterController::stand() {
        if (!character || !crouching) return;

        crouching = false;
        
        // Restore height
        glm::vec3 scale = character->getScale();
        scale.y = height;
        character->setScale(scale);
        
        SPARKY_LOG_DEBUG("Character stood up");
    }

    void CharacterController::setWalkSpeed(float speed) {
        walkSpeed = speed;
        SPARKY_LOG_DEBUG("Walk speed set to: " + std::to_string(speed));
    }

    void CharacterController::setRunSpeed(float speed) {
        runSpeed = speed;
        SPARKY_LOG_DEBUG("Run speed set to: " + std::to_string(speed));
    }

    void CharacterController::setJumpForce(float force) {
        jumpForce = force;
        SPARKY_LOG_DEBUG("Jump force set to: " + std::to_string(force));
    }

    void CharacterController::setGravity(float gravity) {
        this->gravity = gravity;
        SPARKY_LOG_DEBUG("Gravity set to: " + std::to_string(gravity));
    }

    void CharacterController::update(float deltaTime) {
        if (!character) return;

        // Apply gravity
        if (!grounded) {
            verticalVelocity -= gravity * deltaTime;
            
            glm::vec3 position = character->getPosition();
            position.y += verticalVelocity * deltaTime;
            character->setPosition(position);
            
            // Simple ground check (in a real implementation, this would use collision detection)
            if (position.y <= 0.0f) {
                position.y = 0.0f;
                character->setPosition(position);
                verticalVelocity = 0.0f;
                grounded = true;
            }
        }

        SPARKY_LOG_DEBUG("Character controller updated");
    }
}