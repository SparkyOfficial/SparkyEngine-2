#include "../include/RenderComponent.h"
#include "../include/Logger.h"

namespace Sparky {

    RenderComponent::RenderComponent() : Component(), visible(true) {
        SPARKY_LOG_DEBUG("RenderComponent created");
    }

    RenderComponent::~RenderComponent() {
        SPARKY_LOG_DEBUG("RenderComponent destroyed");
    }

    void RenderComponent::update(float deltaTime) {
        // Render components don't need to update every frame
        // But we could add animation or other time-based effects here
    }

    void RenderComponent::render() {
        // Render components don't render directly
        // The rendering is handled by the RenderSystem and VulkanRenderer
    }

    void RenderComponent::setMesh(std::unique_ptr<Mesh> mesh) {
        this->mesh = std::move(mesh);
        SPARKY_LOG_DEBUG("Mesh set for RenderComponent");
    }

    void RenderComponent::setMaterial(std::unique_ptr<Material> mat) {
        this->material = std::move(mat);
        SPARKY_LOG_DEBUG("Material set for RenderComponent");
    }

    void RenderComponent::createVertexBuffer(VkPhysicalDevice physicalDevice, VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue) {
        if (!mesh) {
            SPARKY_LOG_WARNING("Cannot create vertex buffer: no mesh assigned");
            return;
        }
        
        // Vertex buffer creation is now handled by the MeshRenderer class
        // This method is kept for API compatibility but doesn't do anything
        SPARKY_LOG_WARNING("Vertex buffer creation is now handled by MeshRenderer");
    }

    void RenderComponent::createIndexBuffer(VkPhysicalDevice physicalDevice, VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue) {
        if (!mesh) {
            SPARKY_LOG_WARNING("Cannot create index buffer: no mesh assigned");
            return;
        }
        
        // Index buffer creation is now handled by the MeshRenderer class
        // This method is kept for API compatibility but doesn't do anything
        SPARKY_LOG_WARNING("Index buffer creation is now handled by MeshRenderer");
    }

    void RenderComponent::cleanup(VkDevice device) {
        if (mesh) {
            mesh->cleanup(device);
        }
    }
}