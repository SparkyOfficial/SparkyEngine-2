#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <vulkan/vulkan.h>
#include <memory>

namespace Sparky {
    struct Vertex {
        glm::vec3 position;
        glm::vec3 color;
        glm::vec2 texCoord;

        static VkVertexInputBindingDescription getBindingDescription();
        static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions();
    };

    class Mesh {
    public:
        Mesh();
        ~Mesh();

        void setVertices(const std::vector<Vertex>& vertices);
        void setIndices(const std::vector<uint32_t>& indices);

        const std::vector<Vertex>& getVertices() const { return vertices; }
        const std::vector<uint32_t>& getIndices() const { return indices; }
        
        // Static methods to create common mesh types
        static std::unique_ptr<Mesh> createCube(float size = 1.0f);
        static std::unique_ptr<Mesh> createPlane(float width = 1.0f, float height = 1.0f);
        static std::unique_ptr<Mesh> createSphere(float radius = 1.0f, int sectors = 20, int stacks = 20);

    private:
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
    };
}