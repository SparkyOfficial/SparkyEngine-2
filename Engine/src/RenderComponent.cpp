#include "../include/RenderComponent.h"
#include "../include/VulkanRenderer.h"
#include "../include/GameObject.h"

namespace Sparky {

    RenderComponent::RenderComponent() : mesh(nullptr) {
    }

    RenderComponent::~RenderComponent() {
    }

    void RenderComponent::update(float deltaTime) {
        // Render components don't need update logic
    }

    void RenderComponent::render() {
        // Render the mesh if it exists
        if (mesh) {
            // The actual rendering is handled by the Vulkan renderer during the render pass
            // This method is called to prepare for rendering
        }
    }

    void RenderComponent::setMesh(Mesh* mesh) {
        this->mesh = mesh;
    }

    void RenderComponent::setMaterial(const std::string& material) {
        this->material = material;
    }
}