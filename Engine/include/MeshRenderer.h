#pragma once

#include <vulkan/vulkan.h>
#include <stdexcept>
#include <vector>
#include "Mesh.h"

namespace Sparky {
    class MeshRenderer {
    public:
        MeshRenderer();
        ~MeshRenderer();

        void initialize(VkPhysicalDevice physicalDevice, VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue);
        void cleanup();

        void createVertexBuffer(const Mesh& mesh);
        void createIndexBuffer(const Mesh& mesh);
        void renderMesh(const Mesh& mesh, VkCommandBuffer commandBuffer);
        
        // Getter methods for buffer access
        VkBuffer getVertexBuffer() const { return vertexBuffer; }
        VkBuffer getIndexBuffer() const { return indexBuffer; }

    private:
        VkPhysicalDevice physicalDevice;
        VkDevice device;
        VkCommandPool commandPool;
        VkQueue graphicsQueue;
        
        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        VkBuffer indexBuffer;
        VkDeviceMemory indexBufferMemory;

        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    };
}