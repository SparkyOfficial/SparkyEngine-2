#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <vulkan/vulkan.h>

namespace Sparky {

    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoord;

        static VkVertexInputBindingDescription getBindingDescription();
        static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
        
        bool operator==(const Vertex& other) const;
    };

    struct Mesh {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        
        // Vulkan resources
        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        VkBuffer indexBuffer;
        VkDeviceMemory indexBufferMemory;
        
        Mesh();
        ~Mesh();
        
        void cleanup(VkDevice device);
        
        // Factory methods for creating basic shapes
        static std::unique_ptr<Mesh> createCube(float size);
        static std::unique_ptr<Mesh> createPlane(float width, float height);
        static std::unique_ptr<Mesh> createSphere(float radius, int sectors, int stacks);
        static std::unique_ptr<Mesh> createQuad();
        
        // Getters
        size_t getVertexCount() const { return vertices.size(); }
        size_t getIndexCount() const { return indices.size(); }
    };
}