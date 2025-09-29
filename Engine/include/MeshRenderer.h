#pragma once

#include <vulkan/vulkan.h>
#include <stdexcept>
#include <vector>
#include <unordered_map>
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
        
        // Getter methods for buffer access by mesh
        VkBuffer getVertexBuffer(const Mesh& mesh) const;
        VkBuffer getIndexBuffer(const Mesh& mesh) const;

    private:
        VkPhysicalDevice physicalDevice;
        VkDevice device;
        VkCommandPool commandPool;
        VkQueue graphicsQueue;
        
        // Store buffers for each mesh
        std::unordered_map<const Mesh*, VkBuffer> vertexBuffers;
        std::unordered_map<const Mesh*, VkDeviceMemory> vertexBufferMemories;
        std::unordered_map<const Mesh*, VkBuffer> indexBuffers;
        std::unordered_map<const Mesh*, VkDeviceMemory> indexBufferMemories;

        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    };
}