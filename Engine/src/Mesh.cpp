#include "../include/Mesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <vector>
#include <array>

// Define M_PI if not already defined
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace Sparky {

    // Implement the missing Vertex methods
    VkVertexInputBindingDescription Vertex::getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        
        return bindingDescription;
    }

    std::array<VkVertexInputAttributeDescription, 3> Vertex::getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};
        
        // Position attribute
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, position);
        
        // Color attribute
        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);
        
        // Texture coordinate attribute
        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, texCoord);
        
        return attributeDescriptions;
    }

    Mesh::Mesh() {
    }

    Mesh::~Mesh() {
    }

    void Mesh::setVertices(const std::vector<Vertex>& vertices) {
        this->vertices = vertices;
    }

    void Mesh::setIndices(const std::vector<uint32_t>& indices) {
        this->indices = indices;
    }

    std::unique_ptr<Mesh> Mesh::createCube(float size) {
        auto mesh = std::make_unique<Mesh>();
        
        float halfSize = size * 0.5f;
        
        // Define vertices for a cube
        std::vector<Vertex> vertices = {
            // Front face
            {{-halfSize, -halfSize,  halfSize}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}, // 0
            {{ halfSize, -halfSize,  halfSize}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}}, // 1
            {{ halfSize,  halfSize,  halfSize}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}}, // 2
            {{-halfSize,  halfSize,  halfSize}, {1.0f, 1.0f, 0.0f}, {0.0f, 1.0f}}, // 3
            
            // Back face
            {{-halfSize, -halfSize, -halfSize}, {1.0f, 0.0f, 1.0f}, {1.0f, 0.0f}}, // 4
            {{ halfSize, -halfSize, -halfSize}, {0.0f, 1.0f, 1.0f}, {0.0f, 0.0f}}, // 5
            {{ halfSize,  halfSize, -halfSize}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}, // 6
            {{-halfSize,  halfSize, -halfSize}, {0.5f, 0.5f, 0.5f}, {1.0f, 1.0f}}, // 7
            
            // Left face
            {{-halfSize, -halfSize, -halfSize}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}, // 8
            {{-halfSize, -halfSize,  halfSize}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}}, // 9
            {{-halfSize,  halfSize,  halfSize}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}}, // 10
            {{-halfSize,  halfSize, -halfSize}, {1.0f, 1.0f, 0.0f}, {0.0f, 1.0f}}, // 11
            
            // Right face
            {{ halfSize, -halfSize, -halfSize}, {1.0f, 0.0f, 1.0f}, {1.0f, 0.0f}}, // 12
            {{ halfSize, -halfSize,  halfSize}, {0.0f, 1.0f, 1.0f}, {0.0f, 0.0f}}, // 13
            {{ halfSize,  halfSize,  halfSize}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}, // 14
            {{ halfSize,  halfSize, -halfSize}, {0.5f, 0.5f, 0.5f}, {1.0f, 1.0f}}, // 15
            
            // Top face
            {{-halfSize,  halfSize, -halfSize}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}, // 16
            {{-halfSize,  halfSize,  halfSize}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}}, // 17
            {{ halfSize,  halfSize,  halfSize}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}}, // 18
            {{ halfSize,  halfSize, -halfSize}, {1.0f, 1.0f, 0.0f}, {0.0f, 1.0f}}, // 19
            
            // Bottom face
            {{-halfSize, -halfSize, -halfSize}, {1.0f, 0.0f, 1.0f}, {1.0f, 0.0f}}, // 20
            {{-halfSize, -halfSize,  halfSize}, {0.0f, 1.0f, 1.0f}, {0.0f, 0.0f}}, // 21
            {{ halfSize, -halfSize,  halfSize}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}, // 22
            {{ halfSize, -halfSize, -halfSize}, {0.5f, 0.5f, 0.5f}, {1.0f, 1.0f}}  // 23
        };
        
        // Define indices for the cube faces
        std::vector<uint32_t> indices = {
            // Front face
            0, 1, 2, 2, 3, 0,
            // Back face
            4, 6, 5, 6, 4, 7,
            // Left face
            8, 9, 10, 10, 11, 8,
            // Right face
            12, 14, 13, 14, 12, 15,
            // Top face
            16, 17, 18, 18, 19, 16,
            // Bottom face
            20, 22, 21, 22, 20, 23
        };
        
        mesh->setVertices(vertices);
        mesh->setIndices(indices);
        
        return mesh;
    }

    std::unique_ptr<Mesh> Mesh::createPlane(float width, float height) {
        auto mesh = std::make_unique<Mesh>();
        
        float halfWidth = width * 0.5f;
        float halfHeight = height * 0.5f;
        
        // Define vertices for a plane
        std::vector<Vertex> vertices = {
            {{-halfWidth, 0.0f, -halfHeight}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}}, // 0
            {{ halfWidth, 0.0f, -halfHeight}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}}, // 1
            {{ halfWidth, 0.0f,  halfHeight}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}, // 2
            {{-halfWidth, 0.0f,  halfHeight}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}  // 3
        };
        
        // Define indices for the plane
        std::vector<uint32_t> indices = {
            0, 1, 2, 2, 3, 0
        };
        
        mesh->setVertices(vertices);
        mesh->setIndices(indices);
        
        return mesh;
    }

    std::unique_ptr<Mesh> Mesh::createSphere(float radius, int sectors, int stacks) {
        auto mesh = std::make_unique<Mesh>();
        
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        
        float sectorStep = 2 * M_PI / sectors;
        float stackStep = M_PI / stacks;
        
        // Generate vertices
        for (int i = 0; i <= stacks; ++i) {
            float stackAngle = M_PI / 2 - i * stackStep;
            float xy = radius * cosf(stackAngle);
            float z = radius * sinf(stackAngle);
            
            for (int j = 0; j <= sectors; ++j) {
                float sectorAngle = j * sectorStep;
                float x = xy * cosf(sectorAngle);
                float y = xy * sinf(sectorAngle);
                
                Vertex vertex;
                vertex.position = glm::vec3(x, y, z);
                
                // Color based on position
                vertex.color = glm::vec3(
                    (x + radius) / (2 * radius),
                    (y + radius) / (2 * radius),
                    (z + radius) / (2 * radius)
                );
                
                // Texture coordinates
                vertex.texCoord = glm::vec2(
                    (float)j / sectors,
                    (float)i / stacks
                );
                
                vertices.push_back(vertex);
            }
        }
        
        // Generate indices
        for (int i = 0; i < stacks; ++i) {
            int k1 = i * (sectors + 1);
            int k2 = k1 + sectors + 1;
            
            for (int j = 0; j < sectors; ++j, ++k1, ++k2) {
                if (i != 0) {
                    indices.push_back(k1);
                    indices.push_back(k2);
                    indices.push_back(k1 + 1);
                }
                
                if (i != (stacks - 1)) {
                    indices.push_back(k1 + 1);
                    indices.push_back(k2);
                    indices.push_back(k2 + 1);
                }
            }
        }
        
        mesh->setVertices(vertices);
        mesh->setIndices(indices);
        
        return mesh;
    }
}