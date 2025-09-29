#include "../include/MeshRenderer.h"
#include "../include/Logger.h"
#include <cstring>
#include <string>

namespace Sparky {

    MeshRenderer::MeshRenderer() : physicalDevice(nullptr), device(nullptr), commandPool(nullptr), 
                                   graphicsQueue(nullptr) {
    }

    MeshRenderer::~MeshRenderer() {
        cleanup();
    }

    void MeshRenderer::initialize(VkPhysicalDevice physicalDevice, VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue) {
        this->physicalDevice = physicalDevice;
        this->device = device;
        this->commandPool = commandPool;
        this->graphicsQueue = graphicsQueue;
    }

    void MeshRenderer::cleanup() {
        // Clean up all vertex buffers
        for (auto& pair : vertexBufferMemories) {
            if (pair.second) {
                vkFreeMemory(device, pair.second, nullptr);
            }
        }
        
        for (auto& pair : vertexBuffers) {
            if (pair.second) {
                vkDestroyBuffer(device, pair.second, nullptr);
            }
        }
        
        // Clean up all index buffers
        for (auto& pair : indexBufferMemories) {
            if (pair.second) {
                vkFreeMemory(device, pair.second, nullptr);
            }
        }
        
        for (auto& pair : indexBuffers) {
            if (pair.second) {
                vkDestroyBuffer(device, pair.second, nullptr);
            }
        }
        
        vertexBuffers.clear();
        vertexBufferMemories.clear();
        indexBuffers.clear();
        indexBufferMemories.clear();
    }

    void MeshRenderer::createVertexBuffer(const Mesh& mesh) {
        VkDeviceSize bufferSize = sizeof(mesh.getVertices()[0]) * mesh.getVertices().size();
        
        SPARKY_LOG_DEBUG("Creating vertex buffer with size: " + std::to_string(bufferSize) + " bytes");

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                 stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, mesh.getVertices().data(), (size_t)bufferSize);
        vkUnmapMemory(device, stagingBufferMemory);

        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, 
                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

        copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

        vkDestroyBuffer(device, stagingBuffer, nullptr);
        vkFreeMemory(device, stagingBufferMemory, nullptr);
        
        // Store the buffers for this mesh
        vertexBuffers[&mesh] = vertexBuffer;
        vertexBufferMemories[&mesh] = vertexBufferMemory;
        
        SPARKY_LOG_DEBUG("Vertex buffer created successfully for mesh at " + std::to_string(reinterpret_cast<uint64_t>(&mesh)));
    }

    void MeshRenderer::createIndexBuffer(const Mesh& mesh) {
        if (mesh.getIndices().empty()) {
            SPARKY_LOG_DEBUG("Skipping index buffer creation - no indices");
            return;
        }
        
        VkDeviceSize bufferSize = sizeof(mesh.getIndices()[0]) * mesh.getIndices().size();
        
        SPARKY_LOG_DEBUG("Creating index buffer with size: " + std::to_string(bufferSize) + " bytes");

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                 stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, mesh.getIndices().data(), (size_t)bufferSize);
        vkUnmapMemory(device, stagingBufferMemory);

        VkBuffer indexBuffer;
        VkDeviceMemory indexBufferMemory;
        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, 
                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

        copyBuffer(stagingBuffer, indexBuffer, bufferSize);

        vkDestroyBuffer(device, stagingBuffer, nullptr);
        vkFreeMemory(device, stagingBufferMemory, nullptr);
        
        // Store the buffers for this mesh
        indexBuffers[&mesh] = indexBuffer;
        indexBufferMemories[&mesh] = indexBufferMemory;
        
        SPARKY_LOG_DEBUG("Index buffer created successfully for mesh at " + std::to_string(reinterpret_cast<uint64_t>(&mesh)));
    }

    void MeshRenderer::renderMesh(const Mesh& mesh, VkCommandBuffer commandBuffer) {
        // Only render if we have a valid command buffer
        if (commandBuffer == VK_NULL_HANDLE) {
            return;
        }
        
        // Get the vertex buffer for this mesh
        auto vertexBufferIt = vertexBuffers.find(&mesh);
        if (vertexBufferIt == vertexBuffers.end()) {
            SPARKY_LOG_WARNING("No vertex buffer found for mesh");
            return;
        }
        
        VkBuffer vertexBuffer = vertexBufferIt->second;
        
        // Bind vertex buffer
        VkBuffer vertexBuffers[] = {vertexBuffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

        // Bind index buffer if it exists
        auto indexBufferIt = indexBuffers.find(&mesh);
        if (indexBufferIt != indexBuffers.end() && mesh.getIndices().size() > 0) {
            VkBuffer indexBuffer = indexBufferIt->second;
            vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);
            vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(mesh.getIndices().size()), 1, 0, 0, 0);
        } else {
            // Draw without index buffer
            vkCmdDraw(commandBuffer, static_cast<uint32_t>(mesh.getVertices().size()), 1, 0, 0);
        }
    }

    void MeshRenderer::renderMeshes() {
        // This method would be implemented to render all meshes
        // For now, it's just a placeholder
        SPARKY_LOG_DEBUG("Rendering all meshes");
    }

    VkBuffer MeshRenderer::getVertexBuffer(const Mesh& mesh) const {
        auto it = vertexBuffers.find(&mesh);
        if (it != vertexBuffers.end()) {
            return it->second;
        }
        return VK_NULL_HANDLE;
    }

    VkBuffer MeshRenderer::getIndexBuffer(const Mesh& mesh) const {
        auto it = indexBuffers.find(&mesh);
        if (it != indexBuffers.end()) {
            return it->second;
        }
        return VK_NULL_HANDLE;
    }

    void MeshRenderer::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to create buffer!");
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

        if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate buffer memory!");
        }

        vkBindBufferMemory(device, buffer, bufferMemory, 0);
    }

    void MeshRenderer::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = commandPool;
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer, &beginInfo);

        VkBufferCopy copyRegion{};
        copyRegion.srcOffset = 0;
        copyRegion.dstOffset = 0;
        copyRegion.size = size;
        vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(graphicsQueue);

        vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
    }

    uint32_t MeshRenderer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }

        throw std::runtime_error("failed to find suitable memory type!");
    }
}