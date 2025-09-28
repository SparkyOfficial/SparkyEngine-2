#include "../include/GameObject.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Sparky {

    GameObject::GameObject(const std::string& name) : position(0.0f), rotation(0.0f), scale(1.0f), name(name) {
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

    // Remove the duplicate methods since they're inline in the header
    // Also remove getTransformMatrix since it's not declared in the header
}