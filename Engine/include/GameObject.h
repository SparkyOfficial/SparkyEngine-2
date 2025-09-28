#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Component.h"

namespace Sparky {
    class GameObject {
    public:
        GameObject();
        ~GameObject();

        void update(float deltaTime);
        void render();

        // Transform
        void setPosition(const glm::vec3& position);
        void setRotation(const glm::vec3& rotation);
        void setScale(const glm::vec3& scale);

        glm::vec3 getPosition() const { return position; }
        glm::vec3 getRotation() const { return rotation; }
        glm::vec3 getScale() const { return scale; }

        glm::mat4 getTransformMatrix() const;

        // Component management
        void addComponent(std::unique_ptr<Component> component);
        template<typename T>
        T* getComponent() const;

        // Name
        void setName(const std::string& name);
        const std::string& getName() const { return name; }

    private:
        std::string name;
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;
        std::vector<std::unique_ptr<Component>> components;
    };

    template<typename T>
    T* GameObject::getComponent() const {
        for (const auto& component : components) {
            T* result = dynamic_cast<T*>(component.get());
            if (result) {
                return result;
            }
        }
        return nullptr;
    }
}