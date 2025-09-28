#pragma once

#include <vector>
#include <glm/glm.hpp>

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

    private:
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
    };
}