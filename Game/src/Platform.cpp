#include "Platform.h"
#include "../../Engine/include/Mesh.h"
#include "../../Engine/include/VulkanRenderer.h"
#include "../../Engine/include/RenderComponent.h"
#include "../../Engine/include/AssetManager.h"
#include <glm/glm.hpp>
#include <vector>

namespace Sparky {

    Platform::Platform(const std::string& name) : GameObject(), size(1.0f, 1.0f, 1.0f) {
        setName(name);
        createCubeMesh();
        
        // Add a render component
        auto renderComponent = std::make_unique<RenderComponent>();
        renderComponent->setMesh(&mesh);
        addComponent(std::move(renderComponent));
    }

    Platform::~Platform() {
    }

    void Platform::update(float deltaTime) {
        GameObject::update(deltaTime);
    }

    void Platform::render() {
        GameObject::render();
    }

    void Platform::setSize(const glm::vec3& size) {
        this->size = size;
        setScale(size);
    }

    bool Platform::checkCollision(const glm::vec3& point) const {
        glm::vec3 pos = getPosition();
        
        // Simple AABB collision check
        return (point.x >= pos.x - size.x/2.0f && point.x <= pos.x + size.x/2.0f) &&
               (point.y >= pos.y - size.y/2.0f && point.y <= pos.y + size.y/2.0f) &&
               (point.z >= pos.z - size.z/2.0f && point.z <= pos.z + size.z/2.0f);
    }

    void Platform::createCubeMesh() {
        // Create a simple cube mesh
        std::vector<Vertex> vertices = {
            // Front face
            {{-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
            {{ 0.5f, -0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
            {{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
            {{-0.5f,  0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

            // Back face
            {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
            {{ 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
            {{ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
            {{-0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
        };

        std::vector<uint32_t> indices = {
            // Front face
            0, 1, 2,
            2, 3, 0,

            // Back face
            4, 6, 5,
            6, 4, 7,

            // Left face
            4, 0, 3,
            3, 7, 4,

            // Right face
            1, 5, 6,
            6, 2, 1,

            // Top face
            3, 2, 6,
            6, 7, 3,

            // Bottom face
            4, 5, 1,
            1, 0, 4
        };

        mesh.setVertices(vertices);
        mesh.setIndices(indices);
    }
}