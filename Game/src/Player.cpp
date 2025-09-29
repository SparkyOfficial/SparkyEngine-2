#include "Player.h"
#include "../../Engine/include/InputManager.h"
#include "../../Engine/include/PhysicsComponent.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Sparky {

    Player::Player() : GameObject(), camera(nullptr), jumpForce(10.0f), moveSpeed(5.0f), 
                      mouseLocked(false), lastX(400), lastY(300), firstMouse(true) {
        setName("Player");
        
        // Add physics component
        addComponent<PhysicsComponent>();
    }

    Player::~Player() {
    }

    void Player::update(float deltaTime) {
        GameObject::update(deltaTime);
        
        // Process player input
        processInput(deltaTime);
        
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
        // Get input manager instance
        InputManager& inputManager = InputManager::getInstance();
        
        // Mouse movement for camera
        if (mouseLocked) {
            glm::vec2 mouseDelta = inputManager.getMouseDelta();
            if (camera) {
                camera->ProcessMouseMovement(mouseDelta.x, -mouseDelta.y);
            }
        }
        
        // Keyboard movement
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