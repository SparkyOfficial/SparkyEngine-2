#pragma once

#include "../../Engine/include/GameObject.h"
#include "../../Engine/include/Camera.h"
#include "../../Engine/include/PhysicsComponent.h"

namespace Sparky {
    class Player : public GameObject {
    public:
        Player();
        ~Player();

        void update(float deltaTime) override;
        void render() override;

        // Player movement
        void processInput(float deltaTime);
        void moveForward(float deltaTime);
        void moveBackward(float deltaTime);
        void moveLeft(float deltaTime);
        void moveRight(float deltaTime);
        void moveUp(float deltaTime);
        void moveDown(float deltaTime);
        void jump();

        // Player properties
        void setCamera(Camera* camera);
        Camera* getCamera() const { return camera; }

        bool isOnGround() const;
        void setOnGround(bool onGround);
        
        // Getters and setters for movement properties
        float getMoveSpeed() const { return moveSpeed; }
        void setMoveSpeed(float speed) { moveSpeed = speed; }
        
        float getJumpForce() const { return jumpForce; }
        void setJumpForce(float force) { jumpForce = force; }
        
        bool isMouseLocked() const { return mouseLocked; }
        void setMouseLocked(bool locked) { mouseLocked = locked; }

    private:
        Camera* camera;
        float jumpForce;
        float moveSpeed;
        bool mouseLocked;
        float lastX, lastY;
        bool firstMouse;
    };
}