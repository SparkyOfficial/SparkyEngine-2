#include "Player.h"
#include "../../Engine/include/InputManager.h"
#include "../../Engine/include/PhysicsComponent.h"
#include "../../Engine/include/AnimationComponent.h"
#include "../../Engine/include/Animation.h"
#include "../../Engine/include/AnimationController.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#ifdef HAS_GLFW
#include <GLFW/glfw3.h>
#endif

namespace Sparky {

    Player::Player() : GameObject(), camera(nullptr), jumpForce(10.0f), moveSpeed(5.0f), 
                      mouseLocked(false), lastX(400), lastY(300), firstMouse(true),
                      animationController(nullptr) {
        setName("Player");
        
        // Add physics component
        addComponent<PhysicsComponent>();
        
        // Add animation component
        auto animationComponent = addComponent<AnimationComponent>();
        
        // Create a simple idle animation
        auto idleAnimation = std::make_unique<Animation>("idle");
        auto idleTrack = std::make_unique<AnimationTrack>("player");
        
        // Add some keyframes for a simple idle bobbing motion
        Keyframe keyframe1;
        keyframe1.time = 0.0f;
        keyframe1.position = glm::vec3(0.0f, 0.0f, 0.0f);
        keyframe1.rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        keyframe1.scale = glm::vec3(1.0f);
        idleTrack->addKeyframe(keyframe1);
        
        Keyframe keyframe2;
        keyframe2.time = 0.5f;
        keyframe2.position = glm::vec3(0.0f, 0.05f, 0.0f);
        keyframe2.rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        keyframe2.scale = glm::vec3(1.0f);
        idleTrack->addKeyframe(keyframe2);
        
        Keyframe keyframe3;
        keyframe3.time = 1.0f;
        keyframe3.position = glm::vec3(0.0f, 0.0f, 0.0f);
        keyframe3.rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        keyframe3.scale = glm::vec3(1.0f);
        idleTrack->addKeyframe(keyframe3);
        
        idleAnimation->addTrack(std::move(idleTrack));
        animationComponent->addAnimation(std::move(idleAnimation));
        
        // Create a simple walk animation
        auto walkAnimation = std::make_unique<Animation>("walk");
        auto walkTrack = std::make_unique<AnimationTrack>("player");
        
        // Add keyframes for walking motion
        for (int i = 0; i <= 4; ++i) {
            Keyframe keyframe;
            keyframe.time = i * 0.25f;
            keyframe.position = glm::vec3(0.0f, 0.05f * sin(i * 1.57f), 0.0f); // Bobbing motion
            keyframe.rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
            keyframe.scale = glm::vec3(1.0f);
            walkTrack->addKeyframe(keyframe);
        }
        
        walkAnimation->addTrack(std::move(walkTrack));
        animationComponent->addAnimation(std::move(walkAnimation));
        
        // Create animation controller
        animationController = std::make_unique<AnimationController>(animationComponent);
        
        // Add states
        AnimationState idleState;
        idleState.name = "idle";
        idleState.animationName = "idle";
        idleState.playbackSpeed = 1.0f;
        idleState.looping = true;
        animationController->addState(idleState);
        
        AnimationState walkState;
        walkState.name = "walk";
        walkState.animationName = "walk";
        walkState.playbackSpeed = 1.0f;
        walkState.looping = true;
        animationController->addState(walkState);
        
        // Add transitions
        AnimationTransition idleToWalk;
        idleToWalk.fromState = "idle";
        idleToWalk.toState = "walk";
        idleToWalk.transitionTime = 0.2f;
        idleToWalk.condition = [this]() { return isMoving; };
        animationController->addTransition(idleToWalk);
        
        AnimationTransition walkToIdle;
        walkToIdle.fromState = "walk";
        walkToIdle.toState = "idle";
        walkToIdle.transitionTime = 0.2f;
        walkToIdle.condition = [this]() { return !isMoving; };
        animationController->addTransition(walkToIdle);
        
        // Start with idle state
        animationController->setState("idle");
    }

    Player::~Player() {
    }

    void Player::update(float deltaTime) {
        GameObject::update(deltaTime);
        
        // Process player input
        processInput(deltaTime);
        
        // Update animation controller
        if (animationController) {
            animationController->update(deltaTime);
        }
        
        // Update camera position to follow player
        if (camera) {
            // Position camera at player position with slight offset for head height
            glm::vec3 playerPos = getPosition();
            camera->Position = playerPos + glm::vec3(0.0f, 1.8f, 0.0f); // Head height
        }
    }

    void Player::render() {
        GameObject::render();
    }

    void Player::processInput(float deltaTime) {
        // Reset movement flag
        isMoving = false;
        
        // Get input manager instance
        InputManager& inputManager = InputManager::getInstance();
        
        // Mouse movement for camera
        if (mouseLocked) {
            glm::vec2 mouseDelta = inputManager.getMouseDelta();
            if (camera) {
                camera->ProcessMouseMovement(mouseDelta.x, -mouseDelta.y);
            }
        }
        
#ifdef HAS_GLFW
#ifdef HAS_GLFW
        // Keyboard movement
        if (inputManager.isKeyPressed(GLFW_KEY_W) || 
            inputManager.isKeyPressed(GLFW_KEY_S) || 
            inputManager.isKeyPressed(GLFW_KEY_A) || 
            inputManager.isKeyPressed(GLFW_KEY_D)) {
            isMoving = true;
        }
        
        if (inputManager.isKeyPressed(GLFW_KEY_W)) {
            moveForward(deltaTime);
        }
        if (inputManager.isKeyPressed(GLFW_KEY_S)) {
            moveBackward(deltaTime);
        }
        if (inputManager.isKeyPressed(GLFW_KEY_A)) {
            moveLeft(deltaTime);
        }
        if (inputManager.isKeyPressed(GLFW_KEY_D)) {
            moveRight(deltaTime);
        }
        if (inputManager.isKeyPressed(GLFW_KEY_SPACE)) {
            jump();
        }
        if (inputManager.isKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
            moveDown(deltaTime);
        }
        
        // Toggle mouse lock
        if (inputManager.isKeyJustPressed(GLFW_KEY_ESCAPE)) {
            mouseLocked = !mouseLocked;
            inputManager.setCursorMode(mouseLocked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
        }
#else
        // Fallback implementation for non-GLFW builds
        // For now, we'll just keep isMoving as false
        isMoving = false;
#endif
#else
        // Fallback implementation for non-GLFW builds
        // For now, we'll just keep isMoving as false
        isMoving = false;
#endif
    }

    void Player::moveForward(float deltaTime) {
        if (camera) {
            glm::vec3 front = camera->Front;
            front.y = 0.0f; // Keep movement horizontal
            front = glm::normalize(front);
            
            glm::vec3 pos = getPosition();
            pos += front * moveSpeed * deltaTime;
            setPosition(pos);
        }
    }

    void Player::moveBackward(float deltaTime) {
        if (camera) {
            glm::vec3 front = camera->Front;
            front.y = 0.0f; // Keep movement horizontal
            front = glm::normalize(front);
            
            glm::vec3 pos = getPosition();
            pos -= front * moveSpeed * deltaTime;
            setPosition(pos);
        }
    }

    void Player::moveLeft(float deltaTime) {
        if (camera) {
            glm::vec3 right = camera->Right;
            
            glm::vec3 pos = getPosition();
            pos -= right * moveSpeed * deltaTime;
            setPosition(pos);
        }
    }

    void Player::moveRight(float deltaTime) {
        if (camera) {
            glm::vec3 right = camera->Right;
            
            glm::vec3 pos = getPosition();
            pos += right * moveSpeed * deltaTime;
            setPosition(pos);
        }
    }

    void Player::moveUp(float deltaTime) {
        glm::vec3 pos = getPosition();
        pos.y += moveSpeed * deltaTime;
        setPosition(pos);
    }

    void Player::moveDown(float deltaTime) {
        glm::vec3 pos = getPosition();
        pos.y -= moveSpeed * deltaTime;
        setPosition(pos);
    }

    void Player::jump() {
        PhysicsComponent* physics = getComponent<PhysicsComponent>();
        if (physics && physics->isOnGround()) {
            glm::vec3 velocity = physics->getVelocity();
            velocity.y = jumpForce;
            physics->setVelocity(velocity);
            physics->setOnGround(false);
        }
    }

    void Player::setCamera(Camera* camera) {
        this->camera = camera;
    }

    bool Player::isOnGround() const {
        const PhysicsComponent* physics = getComponent<PhysicsComponent>();
        return physics ? physics->isOnGround() : false;
    }

    void Player::setOnGround(bool onGround) {
        PhysicsComponent* physics = getComponent<PhysicsComponent>();
        if (physics) {
            physics->setOnGround(onGround);
        }
    }
}