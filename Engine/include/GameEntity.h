#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

namespace Sparky {
    class GameEntity {
    public:
        GameEntity(const std::string& name);
        virtual ~GameEntity();

        // Position, rotation, scale
        void setPosition(const glm::vec3& position);
        void setRotation(const glm::vec3& rotation);
        void setScale(const glm::vec3& scale);

        glm::vec3 getPosition() const { return position; }
        glm::vec3 getRotation() const { return rotation; }
        glm::vec3 getScale() const { return scale; }

        // Transformation matrix
        glm::mat4 getTransformMatrix() const;

        // Entity properties
        const std::string& getName() const { return name; }

        // Virtual functions for derived classes
        virtual void update(float deltaTime);
        virtual void render();

    protected:
        std::string name;
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;
    };
}