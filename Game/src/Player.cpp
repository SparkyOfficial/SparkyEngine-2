#include "Player.h"
#include "../../Engine/include/InputManager.h"
#include "../../Engine/include/PhysicsComponent.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Sparky {

    Player::Player() : GameObject(), camera(nullptr), jumpForce(10.0f), moveSpeed(5.0f) {
        setName("Player");
        
        // Add physics component
        auto physicsComponent = std::make_unique<PhysicsComponent>();
        addComponent(std::move(physicsComponent));
    }

    Player::~Player() {
    }

    void Player::update(float deltaTime) {
        GameObject::update(deltaTime);
        
        // Update camera position to match player
        if (camera) {
            camera->setPosition(getPosition() + glm::vec3(0.0f, 1.5f, 0.0f)); // Slightly above player
        }
    }

    void Player::render() {
        GameObject::render();
    }

    void Player::moveForward(float deltaTime) {
        if (camera) {
            glm::vec3 front = camera->getFront();
            front.y = 0.0f; // Keep movement horizontal
            front = glm::normalize(front);
            
            glm::vec3 pos = getPosition();
            pos += front * moveSpeed * deltaTime;
            setPosition(pos);
        }
    }

    void Player::moveBackward(float deltaTime) {
        if (camera) {
            glm::vec3 front = camera->getFront();
            front.y = 0.0f; // Keep movement horizontal
            front = glm::normalize(front);
            
            glm::vec3 pos = getPosition();
            pos -= front * moveSpeed * deltaTime;
            setPosition(pos);
        }
    }

    void Player::moveLeft(float deltaTime) {
        if (camera) {
            glm::vec3 right = camera->getRight();
            
            glm::vec3 pos = getPosition();
            pos -= right * moveSpeed * deltaTime;
            setPosition(pos);
        }
    }

    void Player::moveRight(float deltaTime) {
        if (camera) {
            glm::vec3 right = camera->getRight();
            
            glm::vec3 pos = getPosition();
            pos += right * moveSpeed * deltaTime;
            setPosition(pos);
        }
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
        PhysicsComponent* physics = getComponent<PhysicsComponent>();
        return physics ? physics->isOnGround() : false;
    }

    void Player::setOnGround(bool onGround) {
        PhysicsComponent* physics = getComponent<PhysicsComponent>();
        if (physics) {
            physics->setOnGround(onGround);
        }
    }
}