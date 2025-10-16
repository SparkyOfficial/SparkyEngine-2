#pragma once

#include "Component.h"
#include "PhysicsComponent.h"
#include <glm/glm.hpp>
#include <memory>

namespace Sparky {
    /**
     * @brief Advanced Character Controller
     * 
     * Implements a sophisticated character controller with support for:
     * - Ground movement (walking, running, crouching)
     * - Jumping and double jumping
     * - Wall running and wall jumping
     * - Slope handling
     * - Step climbing
     * - Physics-based movement
     */
    class CharacterController : public Component {
    public:
        CharacterController();
        virtual ~CharacterController() = default;

        // Component interface
        virtual void initialize();
        virtual void update(float deltaTime) override;
        virtual void destroy();
        virtual void render() override;

        // Movement controls
        void move(const glm::vec3& direction);
        void jump();
        void crouch(bool crouching);
        void sprint(bool sprinting);
        
        // Wall interactions
        void wallRun(const glm::vec3& wallNormal);
        bool canWallJump() const;
        void wallJump(const glm::vec3& wallNormal);

        // Movement properties
        void setWalkSpeed(float speed);
        float getWalkSpeed() const;
        
        void setRunSpeed(float speed);
        float getRunSpeed() const;
        
        void setJumpForce(float force);
        float getJumpForce() const;
        
        void setGravity(float gravity);
        float getGravity() const;
        
        void setAirControl(float control);
        float getAirControl() const;

        // State queries
        bool isGrounded() const;
        bool isCrouching() const;
        bool isSprinting() const;
        bool isWallRunning() const;
        
        glm::vec3 getVelocity() const;
        void setVelocity(const glm::vec3& velocity);

        // Collision handling
        void onCollisionEnter(PhysicsComponent* other);
        void onCollisionStay(PhysicsComponent* other);
        void onCollisionExit(PhysicsComponent* other);

    private:
        // Movement state
        glm::vec3 m_velocity;
        glm::vec3 m_moveDirection;
        bool m_isGrounded;
        bool m_isCrouching;
        bool m_isSprinting;
        bool m_isWallRunning;
        bool m_canDoubleJump;
        bool m_hasWallJumped;
        
        // Movement properties
        float m_walkSpeed;
        float m_runSpeed;
        float m_jumpForce;
        float m_gravity;
        float m_airControl;
        float m_crouchHeight;
        float m_standHeight;
        
        // Wall running properties
        glm::vec3 m_wallNormal;
        float m_wallRunGravity;
        float m_wallRunSpeed;
        
        // Physics reference
        PhysicsComponent* m_physicsComponent;
        
        // Ground check parameters
        float m_groundCheckDistance;
        glm::vec3 m_groundCheckOffset;
        
        // Slope handling
        float m_maxSlopeAngle;
        bool m_onSlope;
        glm::vec3 m_slopeNormal;
        
        // Step handling
        float m_stepHeight;
        
        // Internal methods
        void applyGravity(float deltaTime);
        void applyMovement(float deltaTime);
        void checkGrounded();
        void handleSlopeMovement();
        void updateColliderHeight();
    };
}