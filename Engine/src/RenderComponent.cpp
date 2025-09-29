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
        
        VkDeviceSize bufferSize = sizeof(mesh->vertices[0]) * mesh->vertices.size();
        
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        VulkanRenderer::createBuffer(physicalDevice, device, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                                    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                                    stagingBuffer, stagingBufferMemory);
        
        void* data;
        vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, mesh->vertices.data(), (size_t)bufferSize);
        vkUnmapMemory(device, stagingBufferMemory);
        
        VulkanRenderer::createBuffer(physicalDevice, device, bufferSize, 
                                    VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, 
                                    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
                                    mesh->vertexBuffer, mesh->vertexBufferMemory);
        
        VulkanRenderer::copyBuffer(device, commandPool, graphicsQueue, stagingBuffer, mesh->vertexBuffer, bufferSize);
        
        vkDestroyBuffer(device, stagingBuffer, nullptr);
        vkFreeMemory(device, stagingBufferMemory, nullptr);
        
        SPARKY_LOG_DEBUG("Vertex buffer created for mesh with " + std::to_string(mesh->vertices.size()) + " vertices");
    }

    void RenderComponent::createIndexBuffer(VkPhysicalDevice physicalDevice, VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue) {
        if (!mesh) {
            SPARKY_LOG_WARNING("Cannot create index buffer: no mesh assigned");
            return;
        }
        
        VkDeviceSize bufferSize = sizeof(mesh->indices[0]) * mesh->indices.size();
        
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        VulkanRenderer::createBuffer(physicalDevice, device, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                                    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                                    stagingBuffer, stagingBufferMemory);
        
        void* data;
        vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, mesh->indices.data(), (size_t)bufferSize);
        vkUnmapMemory(device, stagingBufferMemory);
        
        VulkanRenderer::createBuffer(physicalDevice, device, bufferSize, 
                                    VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, 
                                    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
                                    mesh->indexBuffer, mesh->indexBufferMemory);
        
        VulkanRenderer::copyBuffer(device, commandPool, graphicsQueue, stagingBuffer, mesh->indexBuffer, bufferSize);
        
        vkDestroyBuffer(device, stagingBuffer, nullptr);
        vkFreeMemory(device, stagingBufferMemory, nullptr);
        
        SPARKY_LOG_DEBUG("Index buffer created for mesh with " + std::to_string(mesh->indices.size()) + " indices");
    }

    void RenderComponent::cleanup(VkDevice device) {
        if (mesh) {
            mesh->cleanup(device);
        }
    }
}