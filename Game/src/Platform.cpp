#include "Platform.h"
#include "../../Engine/include/Mesh.h"
#include "../../Engine/include/VulkanRenderer.h"
#include "../../Engine/include/RenderComponent.h"
#include "../../Engine/include/AssetManager.h"
#include <glm/glm.hpp>
#include <vector>

namespace Sparky {

    Platform::Platform(const std::string& name) : GameObject() {
        setName(name);
        // The mesh will be created when setSize is called
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
        
        // Create or update the mesh based on the size
        createCubeMesh();
    }

    bool Platform::checkCollision(const glm::vec3& point) const {
        glm::vec3 pos = getPosition();
        
        // Simple AABB collision check
        return (point.x >= pos.x - size.x/2.0f && point.x <= pos.x + size.x/2.0f) &&
               (point.y >= pos.y - size.y/2.0f && point.y <= pos.y + size.y/2.0f) &&
               (point.z >= pos.z - size.z/2.0f && point.z <= pos.z + size.z/2.0f);
    }

    void Platform::createCubeMesh() {
        // Remove any existing render component
        removeComponent<RenderComponent>();
        
        // Create a new cube mesh with the specified size
        auto cubeMesh = Mesh::createCube(1.0f); // Unit cube, scaled by the GameObject's scale
        
        // Add a render component with the cube mesh
        auto renderComponent = std::make_unique<RenderComponent>();
        renderComponent->setMesh(std::move(cubeMesh));
        addComponent(std::move(renderComponent));
    }
}