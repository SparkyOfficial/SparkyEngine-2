#include "../include/CharacterController.h"
#include "../include/GameObject.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Sparky {
    CharacterController::CharacterController()
        : m_velocity(0.0f, 0.0f, 0.0f)
        , m_moveDirection(0.0f, 0.0f, 0.0f)
        , m_isGrounded(false)
        , m_isCrouching(false)
        , m_isSprinting(false)
        , m_isWallRunning(false)
        , m_canDoubleJump(true)
        , m_hasWallJumped(false)
        , m_walkSpeed(4.0f)
        , m_runSpeed(8.0f)
        , m_jumpForce(10.0f)
        , m_gravity(-9.81f)
        , m_airControl(0.5f)
        , m_crouchHeight(1.0f)
        , m_standHeight(1.8f)
        , m_wallNormal(0.0f, 0.0f, 0.0f)
        , m_wallRunGravity(-3.0f)
        , m_wallRunSpeed(5.0f)
        , m_physicsComponent(nullptr)
        , m_groundCheckDistance(0.1f)
        , m_groundCheckOffset(0.0f, -0.9f, 0.0f)
        , m_maxSlopeAngle(45.0f)
        , m_onSlope(false)
        , m_slopeNormal(0.0f, 1.0f, 0.0f)
        , m_stepHeight(0.3f) {
    }

    void CharacterController::initialize() {
        // Get the physics component from the owner GameObject
        if (owner) {
            m_physicsComponent = owner->getComponent<PhysicsComponent>();
        }
    }

    void CharacterController::update(float deltaTime) {
        // Apply gravity
        applyGravity(deltaTime);
        
        // Apply movement
        applyMovement(deltaTime);
        
        // Check if grounded
        checkGrounded();
        
        // Handle slope movement
        handleSlopeMovement();
        
        // Update collider height based on crouching state
        updateColliderHeight();
        
        // Reset wall running state if not touching wall
        if (m_isWallRunning && glm::length(m_wallNormal) < 0.1f) {
            m_isWallRunning = false;
        }
        
        // Reset wall jump state
        m_hasWallJumped = false;
    }

    void CharacterController::destroy() {
        // Cleanup any resources if needed
    }

    void CharacterController::render() {
        // Character controller doesn't render anything directly
    }

    void CharacterController::move(const glm::vec3& direction) {
        m_moveDirection = direction;
    }

    void CharacterController::jump() {
        if (m_isGrounded) {
            m_velocity.y = m_jumpForce;
            m_isGrounded = false;
            m_canDoubleJump = true;
        } else if (m_canDoubleJump && !m_isWallRunning) {
            m_velocity.y = m_jumpForce * 0.8f;
            m_canDoubleJump = false;
        } else if (m_isWallRunning && !m_hasWallJumped) {
            // Wall jump - push away from wall and upwards
            glm::vec3 wallJumpDirection = m_wallNormal * 2.0f + glm::vec3(0.0f, 1.0f, 0.0f);
            m_velocity = wallJumpDirection * m_jumpForce;
            m_isWallRunning = false;
            m_hasWallJumped = true;
        }
    }

    void CharacterController::crouch(bool crouching) {
        m_isCrouching = crouching;
    }

    void CharacterController::sprint(bool sprinting) {
        m_isSprinting = sprinting;
    }

    void CharacterController::wallRun(const glm::vec3& wallNormal) {
        if (!m_isGrounded && !m_hasWallJumped) {
            m_isWallRunning = true;
            m_wallNormal = wallNormal;
            // Reduce gravity while wall running
            m_velocity.y = m_wallRunGravity;
        }
    }

    bool CharacterController::canWallJump() const {
        return m_isWallRunning && !m_hasWallJumped;
    }

    void CharacterController::wallJump(const glm::vec3& wallNormal) {
        if (m_isWallRunning && !m_hasWallJumped) {
            // Calculate wall jump direction - away from wall and upwards
            glm::vec3 jumpDirection = wallNormal * 2.0f + glm::vec3(0.0f, 1.0f, 0.0f);
            m_velocity = jumpDirection * m_jumpForce;
            m_isWallRunning = false;
            m_hasWallJumped = true;
        }
    }

    void CharacterController::setWalkSpeed(float speed) {
        m_walkSpeed = speed;
    }

    float CharacterController::getWalkSpeed() const {
        return m_walkSpeed;
    }

    void CharacterController::setRunSpeed(float speed) {
        m_runSpeed = speed;
    }

    float CharacterController::getRunSpeed() const {
        return m_runSpeed;
    }

    void CharacterController::setJumpForce(float force) {
        m_jumpForce = force;
    }

    float CharacterController::getJumpForce() const {
        return m_jumpForce;
    }

    void CharacterController::setGravity(float gravity) {
        m_gravity = gravity;
    }

    float CharacterController::getGravity() const {
        return m_gravity;
    }

    void CharacterController::setAirControl(float control) {
        m_airControl = control;
    }

    float CharacterController::getAirControl() const {
        return m_airControl;
    }

    bool CharacterController::isGrounded() const {
        return m_isGrounded;
    }

    bool CharacterController::isCrouching() const {
        return m_isCrouching;
    }

    bool CharacterController::isSprinting() const {
        return m_isSprinting;
    }

    bool CharacterController::isWallRunning() const {
        return m_isWallRunning;
    }

    glm::vec3 CharacterController::getVelocity() const {
        return m_velocity;
    }

    void CharacterController::setVelocity(const glm::vec3& velocity) {
        m_velocity = velocity;
    }

    void CharacterController::onCollisionEnter(PhysicsComponent* other) {
        // Handle collision events
    }

    void CharacterController::onCollisionStay(PhysicsComponent* other) {
        // Handle ongoing collision
    }

    void CharacterController::onCollisionExit(PhysicsComponent* other) {
        // Handle collision end
    }

    void CharacterController::applyGravity(float deltaTime) {
        float currentGravity = m_isWallRunning ? m_wallRunGravity : m_gravity;
        m_velocity.y += currentGravity * deltaTime;
    }

    void CharacterController::applyMovement(float deltaTime) {
        // Calculate movement speed based on state
        float currentSpeed = m_isSprinting ? m_runSpeed : m_walkSpeed;
        if (m_isCrouching) {
            currentSpeed *= 0.5f; // Reduce speed while crouching
        }

        // Apply movement direction
        glm::vec3 horizontalMovement = glm::vec3(m_moveDirection.x, 0.0f, m_moveDirection.z);
        horizontalMovement *= currentSpeed;

        // Apply air control if not grounded
        if (!m_isGrounded) {
            horizontalMovement *= m_airControl;
        }

        // Apply movement to velocity
        m_velocity.x = horizontalMovement.x;
        m_velocity.z = horizontalMovement.z;

        // Apply velocity to physics component if available
        if (m_physicsComponent) {
            m_physicsComponent->setVelocity(m_velocity);
        }
    }

    void CharacterController::checkGrounded() {
        // In a real implementation, this would check for collisions below the character
        // For now, we'll use a simple heuristic
        if (m_velocity.y <= 0.0f && !m_isWallRunning) {
            // Character is moving downward, likely approaching ground
            // In a real implementation, this would involve raycasting or collision detection
            m_isGrounded = (owner->getPosition().y <= 0.0f); // Simplified check
        }
    }

    void CharacterController::handleSlopeMovement() {
        if (m_onSlope && m_isGrounded) {
            // Adjust movement direction based on slope normal
            glm::vec3 slopeDirection = glm::cross(glm::cross(m_slopeNormal, glm::vec3(0.0f, 1.0f, 0.0f)), m_slopeNormal);
            slopeDirection = glm::normalize(slopeDirection);
            
            // Apply slope force to help character move up slopes
            float slopeAngle = glm::degrees(glm::acos(glm::dot(m_slopeNormal, glm::vec3(0.0f, 1.0f, 0.0f))));
            if (slopeAngle > 5.0f) { // Only apply on significant slopes
                m_velocity += slopeDirection * 2.0f * 0.016f; // Simplified
            }
        }
    }

    void CharacterController::updateColliderHeight() {
        // In a real implementation, this would adjust the collider height
        // based on the crouching state
        float targetHeight = m_isCrouching ? m_crouchHeight : m_standHeight;
        // Apply height change to collider component
    }
}