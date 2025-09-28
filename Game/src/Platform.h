#pragma once

#include "../../Engine/include/GameObject.h"
#include "../../Engine/include/Mesh.h"
#include <memory>

namespace Sparky {
    class Platform : public GameObject {
    public:
        Platform(const std::string& name);
        ~Platform();

        void update(float deltaTime) override;
        void render() override;

        // Platform properties
        void setSize(const glm::vec3& size);
        glm::vec3 getSize() const { return size; }

        // Collision detection
        bool checkCollision(const glm::vec3& point) const;

    private:
        glm::vec3 size;
        std::unique_ptr<Mesh> mesh;
        
        void createCubeMesh();
    };
}