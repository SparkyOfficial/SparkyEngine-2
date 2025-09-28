#include "../include/GameEntity.h"

namespace Sparky {

    GameEntity::GameEntity(const std::string& name) : name(name), position(0.0f), rotation(0.0f), scale(1.0f) {
    }

    GameEntity::~GameEntity() {
    }

    void GameEntity::setPosition(const glm::vec3& position) {
        this->position = position;
    }

    void GameEntity::setRotation(const glm::vec3& rotation) {
        this->rotation = rotation;
    }

    void GameEntity::setScale(const glm::vec3& scale) {
        this->scale = scale;
    }

    glm::mat4 GameEntity::getTransformMatrix() const {
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, position);
        transform = glm::rotate(transform, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        transform = glm::rotate(transform, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        transform = glm::rotate(transform, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        transform = glm::scale(transform, scale);
        return transform;
    }

    void GameEntity::update(float deltaTime) {
        // Base implementation - can be overridden by derived classes
    }

    void GameEntity::render() {
        // Base implementation - can be overridden by derived classes
    }
}