#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#ifdef HAS_VULKAN
#include <vulkan/vulkan.h>
#endif

namespace Sparky {

    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoord;
        glm::vec3 tangent;
        glm::vec3 bitangent;

#ifdef HAS_VULKAN
        static VkVertexInputBindingDescription getBindingDescription();
        static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
#endif
        
        bool operator==(const Vertex& other) const;
    };

    struct Mesh {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        
#ifdef HAS_VULKAN
        // Vulkan resources
        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        VkBuffer indexBuffer;
        VkDeviceMemory indexBufferMemory;
#endif
        
        Mesh();
        ~Mesh();
        
#ifdef HAS_VULKAN
        void cleanup(VkDevice device);
#endif
        
        // Factory methods for creating basic shapes
        static std::unique_ptr<Mesh> createCube(float size);
        static std::unique_ptr<Mesh> createPlane(float width, float height);
        static std::unique_ptr<Mesh> createSphere(float radius, int sectors, int stacks);
        static std::unique_ptr<Mesh> createQuad();
        
        // Getters
        const std::vector<Vertex>& getVertices() const { return vertices; }
        const std::vector<uint32_t>& getIndices() const { return indices; }
        size_t getVertexCount() const { return vertices.size(); }
        size_t getIndexCount() const { return indices.size(); }
    };
}