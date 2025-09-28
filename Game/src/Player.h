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
        void moveForward(float deltaTime);
        void moveBackward(float deltaTime);
        void moveLeft(float deltaTime);
        void moveRight(float deltaTime);
        void jump();

        // Player properties
        void setCamera(Camera* camera);
        Camera* getCamera() const { return camera; }

        bool isOnGround() const;
        void setOnGround(bool onGround);

    private:
        Camera* camera;
        float jumpForce;
        float moveSpeed;
    };
}