#include "../include/RenderComponent.h"

namespace Sparky {

    RenderComponent::RenderComponent() : mesh(nullptr) {
    }

    RenderComponent::~RenderComponent() {
    }

    void RenderComponent::update(float deltaTime) {
        // Render components don't need update logic
    }

    void RenderComponent::render() {
        // In a real implementation, this would render the mesh
        // For now, we'll add a placeholder that doesn't spam the log
        // SPARKY_LOG_DEBUG("Rendering mesh");
    }

    void RenderComponent::setMesh(Mesh* mesh) {
        this->mesh = mesh;
    }

    void RenderComponent::setMaterial(const std::string& material) {
        this->material = material;
    }
}