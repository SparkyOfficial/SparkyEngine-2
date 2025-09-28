#include "../include/GameObject.h"

namespace Sparky {

    GameObject::GameObject() : position(0.0f), rotation(0.0f), scale(1.0f) {
    }

    GameObject::~GameObject() {
    }

    void GameObject::update(float deltaTime) {
        for (auto& component : components) {
            component->update(deltaTime);
        }
    }

    void GameObject::render() {
        for (auto& component : components) {
            component->render();
        }
    }

    void GameObject::setPosition(const glm::vec3& position) {
        this->position = position;
    }

    void GameObject::setRotation(const glm::vec3& rotation) {
        this->rotation = rotation;
    }

    void GameObject::setScale(const glm::vec3& scale) {
        this->scale = scale;
    }

    glm::mat4 GameObject::getTransformMatrix() const {
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, position);
        transform = glm::rotate(transform, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        transform = glm::rotate(transform, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        transform = glm::rotate(transform, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        transform = glm::scale(transform, scale);
        return transform;
    }

    void GameObject::addComponent(std::unique_ptr<Component> component) {
        component->setOwner(this);
        components.push_back(std::move(component));
    }

    void GameObject::setName(const std::string& name) {
        this->name = name;
    }
}