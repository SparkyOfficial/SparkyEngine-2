#pragma once

#include <glm/glm.hpp>

namespace Sparky {
    class GameObject;

    class CharacterController {
    public:
        CharacterController(GameObject* character);
        ~CharacterController();

        // Movement
        void move(const glm::vec3& direction, float deltaTime);
        void jump();
        void crouch();
        void stand();

        // Properties
        void setWalkSpeed(float speed);
        void setRunSpeed(float speed);
        void setJumpForce(float force);
        void setGravity(float gravity);

        float getWalkSpeed() const { return walkSpeed; }
        float getRunSpeed() const { return runSpeed; }
        float getJumpForce() const { return jumpForce; }
        float getGravity() const { return gravity; }

        // State
        bool isGrounded() const { return grounded; }
        bool isRunning() const { return running; }
        bool isCrouching() const { return crouching; }

        // Update
        void update(float deltaTime);

    private:
        GameObject* character;

        // Movement properties
        float walkSpeed;
        float runSpeed;
        float jumpForce;
        float gravity;
        float verticalVelocity;

        // State
        bool grounded;
        bool running;
        bool crouching;

        // Dimensions
        float height;
        float crouchHeight;
    };
}