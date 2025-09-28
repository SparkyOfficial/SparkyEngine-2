#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <memory>

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

        // Virtual methods
        virtual void update(float deltaTime);
        virtual void render();
    };
}