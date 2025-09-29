#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>

#include "Component.h"

namespace Sparky {
    class GameObject {
    protected:
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;
        std::string name;
        std::vector<std::unique_ptr<Component>> components;

    public:
        GameObject(const std::string& name = "GameObject");
        virtual ~GameObject();

        // Getters and setters
        const glm::vec3& getPosition() const { return position; }
        void setPosition(const glm::vec3& pos) { position = pos; }

        const glm::vec3& getRotation() const { return rotation; }
        void setRotation(const glm::vec3& rot) { rotation = rot; }

        const glm::vec3& getScale() const { return scale; }
        void setScale(const glm::vec3& s) { scale = s; }

        const std::string& getName() const { return name; }
        void setName(const std::string& n) { name = n; }

        // Transform matrix calculation
        glm::mat4 getTransformMatrix() const {
            glm::mat4 transform = glm::mat4(1.0f);
            transform = glm::translate(transform, position);
            transform = glm::rotate(transform, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
            transform = glm::rotate(transform, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
            transform = glm::rotate(transform, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
            transform = glm::scale(transform, scale);
            return transform;
        }

        // Component management
        template<typename T, typename... Args>
        T* addComponent(Args&&... args) {
            static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");
            auto component = std::make_unique<T>(std::forward<Args>(args)...);
            T* componentPtr = component.get();
            components.push_back(std::move(component));
            componentPtr->setOwner(this);
            return componentPtr;
        }

        template<typename T>
        T* getComponent() {
            static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");
            for (auto& component : components) {
                if (T* castComponent = dynamic_cast<T*>(component.get())) {
                    return castComponent;
                }
            }
            return nullptr;
        }

        template<typename T>
        const T* getComponent() const {
            static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");
            for (const auto& component : components) {
                if (const T* castComponent = dynamic_cast<const T*>(component.get())) {
                    return castComponent;
                }
            }
            return nullptr;
        }

        template<typename T>
        void removeComponent() {
            static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");
            components.erase(
                std::remove_if(components.begin(), components.end(),
                    [](const std::unique_ptr<Component>& component) {
                        return dynamic_cast<T*>(component.get()) != nullptr;
                    }),
                components.end()
            );
        }

        // Virtual methods
        virtual void update(float deltaTime);
        virtual void render();
    };
}