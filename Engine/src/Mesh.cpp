#include "../include/Mesh.h"
#include "../include/Logger.h"
#include <cmath>

// Define M_PI if it's not already defined
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace Sparky {

    VkVertexInputBindingDescription Vertex::getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        
        return bindingDescription;
    }

    std::vector<VkVertexInputAttributeDescription> Vertex::getAttributeDescriptions() {
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions(3);
        
        // Position attribute
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, position);
        
        // Normal attribute
        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, normal);
        
        // Texture coordinate attribute
        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, texCoord);
        
        return attributeDescriptions;
    }
    
    bool Vertex::operator==(const Vertex& other) const {
        return position == other.position && normal == other.normal && texCoord == other.texCoord;
    }

    Mesh::Mesh() : vertexBuffer(nullptr), vertexBufferMemory(nullptr), 
                  indexBuffer(nullptr), indexBufferMemory(nullptr) {
    }

    Mesh::~Mesh() {
    }

    void Mesh::cleanup(VkDevice device) {
        if (indexBufferMemory) {
            vkFreeMemory(device, indexBufferMemory, nullptr);
            indexBufferMemory = nullptr;
        }
        
        if (indexBuffer) {
            vkDestroyBuffer(device, indexBuffer, nullptr);
            indexBuffer = nullptr;
        }
        
        if (vertexBufferMemory) {
            vkFreeMemory(device, vertexBufferMemory, nullptr);
            vertexBufferMemory = nullptr;
        }
        
        if (vertexBuffer) {
            vkDestroyBuffer(device, vertexBuffer, nullptr);
            vertexBuffer = nullptr;
        }
    }

    std::unique_ptr<Mesh> Mesh::createCube(float size) {
        auto mesh = std::make_unique<Mesh>();
        
        float halfSize = size * 0.5f;
        
        // Define vertices with positions, normals, and texture coordinates
        mesh->vertices = {
            // Front face
            {{-halfSize, -halfSize,  halfSize}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
            {{ halfSize, -halfSize,  halfSize}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
            {{ halfSize,  halfSize,  halfSize}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
            {{-halfSize,  halfSize,  halfSize}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
            
            // Back face
            {{-halfSize, -halfSize, -halfSize}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},
            {{-halfSize,  halfSize, -halfSize}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
            {{ halfSize,  halfSize, -halfSize}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},
            {{ halfSize, -halfSize, -halfSize}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
            
            // Top face
            {{-halfSize,  halfSize, -halfSize}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
            {{-halfSize,  halfSize,  halfSize}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
            {{ halfSize,  halfSize,  halfSize}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
            {{ halfSize,  halfSize, -halfSize}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
            
            // Bottom face
            {{-halfSize, -halfSize, -halfSize}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
            {{ halfSize, -halfSize, -halfSize}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},
            {{ halfSize, -halfSize,  halfSize}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},
            {{-halfSize, -halfSize,  halfSize}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},
            
            // Right face
            {{ halfSize, -halfSize, -halfSize}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
            {{ halfSize,  halfSize, -halfSize}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
            {{ halfSize,  halfSize,  halfSize}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
            {{ halfSize, -halfSize,  halfSize}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
            
            // Left face
            {{-halfSize, -halfSize, -halfSize}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
            {{-halfSize, -halfSize,  halfSize}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
            {{-halfSize,  halfSize,  halfSize}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
            {{-halfSize,  halfSize, -halfSize}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}}
        };
        
        // Define indices
        mesh->indices = {
            0,  1,  2,  2,  3,  0,    // Front
            4,  5,  6,  6,  7,  4,    // Back
            8,  9,  10, 10, 11, 8,    // Top
            12, 13, 14, 14, 15, 12,   // Bottom
            16, 17, 18, 18, 19, 16,   // Right
            20, 21, 22, 22, 23, 20    // Left
        };
        
        return mesh;
    }

    std::unique_ptr<Mesh> Mesh::createPlane(float width, float height) {
        auto mesh = std::make_unique<Mesh>();
        
        float halfWidth = width * 0.5f;
        float halfHeight = height * 0.5f;
        
        // Define vertices with positions, normals, and texture coordinates
        mesh->vertices = {
            {{-halfWidth, 0.0f, -halfHeight}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
            {{ halfWidth, 0.0f, -halfHeight}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
            {{ halfWidth, 0.0f,  halfHeight}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
            {{-halfWidth, 0.0f,  halfHeight}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}}
        };
        
        // Define indices
        mesh->indices = {
            0, 1, 2, 2, 3, 0
        };
        
        return mesh;
    }

    std::unique_ptr<Mesh> Mesh::createSphere(float radius, int sectors, int stacks) {
        auto mesh = std::make_unique<Mesh>();
        
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
                
                // Vertex position
                glm::vec3 position(x, y, z);
                
                // Normal (normalized position for sphere)
                glm::vec3 normal = glm::normalize(position);
                
                // Texture coordinate
                float s = (float)j / sectors;
                float t = (float)i / stacks;
                glm::vec2 texCoord(s, t);
                
                mesh->vertices.push_back({position, normal, texCoord});
            }
        }
        
        // Generate indices
        for (int i = 0; i < stacks; ++i) {
            int k1 = i * (sectors + 1);
            int k2 = k1 + sectors + 1;
            
            for (int j = 0; j < sectors; ++j, ++k1, ++k2) {
                if (i != 0) {
                    mesh->indices.push_back(k1);
                    mesh->indices.push_back(k2);
                    mesh->indices.push_back(k1 + 1);
                }
                
                if (i != (stacks - 1)) {
                    mesh->indices.push_back(k1 + 1);
                    mesh->indices.push_back(k2);
                    mesh->indices.push_back(k2 + 1);
                }
            }
        }
        
        return mesh;
    }

    std::unique_ptr<Mesh> Mesh::createQuad() {
        auto mesh = std::make_unique<Mesh>();
        
        // Define vertices with positions, normals, and texture coordinates
        mesh->vertices = {
            {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
            {{ 0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
            {{ 0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
            {{-0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}}
        };
        
        // Define indices
        mesh->indices = {
            0, 1, 2, 2, 3, 0
        };
        
        return mesh;
    }
}