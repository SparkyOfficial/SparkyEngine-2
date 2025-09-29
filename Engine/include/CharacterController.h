#pragma once

#include "Component.h"
#include "RigidBodyComponent.h"
#include <glm/glm.hpp>

namespace Sparky {
    class CharacterController : public Component {
    public:
        CharacterController();
        virtual ~CharacterController();

        void update(float deltaTime) override;
        void render() override;

        // Movement
        void move(const glm::vec3& direction);
        void jump();
        void setMoveSpeed(float speed);
        void setJumpForce(float force);
        
        // Ground detection
        bool isGrounded() const { return grounded; }
        
        // Physics properties
        void setRigidBody(RigidBodyComponent* rigidBody);
        RigidBodyComponent* getRigidBody() const { return rigidBody; }

    private:
        RigidBodyComponent* rigidBody;
        float moveSpeed;
        float jumpForce;
        bool grounded;
        float groundCheckDistance;
        
        // Movement state
        glm::vec3 moveDirection;
        bool jumpRequested;
    };
}