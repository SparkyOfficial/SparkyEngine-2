#include <vulkan/vulkan.h>
#include "../include/Skybox.h"
#include "../include/VulkanRenderer.h"
#include "../include/Logger.h"
#include "../include/FileUtils.h"
#include <stb/stb_image.h>
#include <glm/gtc/matrix_transform.hpp>

namespace Sparky {
    
    Skybox::Skybox() : loaded(false), cubemapImage(VK_NULL_HANDLE), 
                      cubemapImageMemory(VK_NULL_HANDLE), cubemapImageView(VK_NULL_HANDLE),
                      cubemapSampler(VK_NULL_HANDLE) {
        SPARKY_LOG_DEBUG("Skybox created");
    }
    
    Skybox::~Skybox() {
        cleanup();
        SPARKY_LOG_DEBUG("Skybox destroyed");
    }
    
    bool Skybox::initialize(VulkanRenderer* renderer) {
        if (!renderer) {
            SPARKY_LOG_ERROR("Cannot initialize skybox without renderer");
            return false;
        }
        
        // Create cube mesh for skybox
        mesh = createCubeMesh();
        if (!mesh) {
            SPARKY_LOG_ERROR("Failed to create skybox mesh");
            return false;
        }
        
        // Create material for skybox
        material = std::make_unique<Material>("SkyboxMaterial");
        if (!material) {
            SPARKY_LOG_ERROR("Failed to create skybox material");
            return false;
        }
        
        SPARKY_LOG_INFO("Skybox initialized successfully");
        return true;
    }
    
    void Skybox::cleanup() {
        mesh.reset();
        material.reset();
        for (auto& texture : faceTextures) {
            texture.reset();
        }
        loaded = false;
        
        // Clean up Vulkan resources
        if (cubemapSampler != VK_NULL_HANDLE) {
            vkDestroySampler(renderer->getDevice(), cubemapSampler, nullptr);
            cubemapSampler = VK_NULL_HANDLE;
        }
        
        if (cubemapImageView != VK_NULL_HANDLE) {
            vkDestroyImageView(renderer->getDevice(), cubemapImageView, nullptr);
            cubemapImageView = VK_NULL_HANDLE;
        }
        
        if (cubemapImageMemory != VK_NULL_HANDLE) {
            vkFreeMemory(renderer->getDevice(), cubemapImageMemory, nullptr);
            cubemapImageMemory = VK_NULL_HANDLE;
        }
        
        if (cubemapImage != VK_NULL_HANDLE) {
            vkDestroyImage(renderer->getDevice(), cubemapImage, nullptr);
            cubemapImage = VK_NULL_HANDLE;
        }
        
        SPARKY_LOG_DEBUG("Skybox cleaned up");
    }
    
    bool Skybox::loadFromFile(const std::array<std::string, 6>& faceTexturesPaths) {
        // For now, we'll just log that skybox loading would be implemented
        // In a full implementation, we would:
        // 1. Load 6 textures for the skybox faces
        // 2. Create a cubemap texture
        // 3. Set up the skybox renderer
        SPARKY_LOG_DEBUG("Skybox loading would be implemented here");
        return true;
    }
    
    std::unique_ptr<Mesh> Skybox::createCubeMesh() {
        auto skyboxMesh = std::make_unique<Mesh>();
        
        // Define vertices for a cube centered at origin with size 2 (from -1 to 1)
        // We only need positions for skybox rendering
        skyboxMesh->vertices = {
            // Front face
            {{-1.0f, -1.0f,  1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
            {{ 1.0f, -1.0f,  1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
            {{ 1.0f,  1.0f,  1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
            {{-1.0f,  1.0f,  1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
            
            // Back face
            {{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},
            {{-1.0f,  1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
            {{ 1.0f,  1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},
            {{ 1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
            
            // Top face
            {{-1.0f,  1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
            {{-1.0f,  1.0f,  1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
            {{ 1.0f,  1.0f,  1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
            {{ 1.0f,  1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
            
            // Bottom face
            {{-1.0f, -1.0f, -1.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
            {{ 1.0f, -1.0f, -1.0f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},
            {{ 1.0f, -1.0f,  1.0f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},
            {{-1.0f, -1.0f,  1.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},
            
            // Right face
            {{ 1.0f, -1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
            {{ 1.0f,  1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
            {{ 1.0f,  1.0f,  1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
            {{ 1.0f, -1.0f,  1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
            
            // Left face
            {{-1.0f, -1.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
            {{-1.0f, -1.0f,  1.0f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
            {{-1.0f,  1.0f,  1.0f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
            {{-1.0f,  1.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}}
        };
        
        // Define indices for the cube
        skyboxMesh->indices = {
            0,  1,  2,  2,  3,  0,    // Front
            4,  5,  6,  6,  7,  4,    // Back
            8,  9,  10, 10, 11, 8,    // Top
            12, 13, 14, 14, 15, 12,   // Bottom
            16, 17, 18, 18, 19, 16,   // Right
            20, 21, 22, 22, 23, 20    // Left
        };
        
        return skyboxMesh;
    }
    
    bool Skybox::createCubemapTexture(VulkanRenderer* renderer, const std::array<std::string, 6>& faceTextures) {
        // This would implement cubemap texture creation
        // For now, we'll just log that it would be implemented
        SPARKY_LOG_DEBUG("Cubemap texture creation would be implemented here");
        return true;
    }
    
    void Skybox::render(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, uint32_t imageIndex) {
        if (!loaded || !mesh || !material) {
            return;
        }
        
        // Skybox rendering would be implemented here
        SPARKY_LOG_DEBUG("Skybox rendering");
    }
    
    bool Skybox::createCubemapImage(VulkanRenderer* renderer, uint32_t width, uint32_t height) {
        // This would implement cubemap image creation
        // For now, we'll just log that it would be implemented
        SPARKY_LOG_DEBUG("Cubemap image creation would be implemented here");
        return true;
    }
    
    bool Skybox::createCubemapImageView(VulkanRenderer* renderer) {
        // This would implement cubemap image view creation
        // For now, we'll just log that it would be implemented
        SPARKY_LOG_DEBUG("Cubemap image view creation would be implemented here");
        return true;
    }
    
    bool Skybox::createCubemapSampler(VulkanRenderer* renderer) {
        // This would implement cubemap sampler creation
        // For now, we'll just log that it would be implemented
        SPARKY_LOG_DEBUG("Cubemap sampler creation would be implemented here");
        return true;
    }
    
    bool Skybox::loadCubemapFaces(VulkanRenderer* renderer, const std::array<std::string, 6>& faceTextures) {
        // This would implement loading cubemap faces
        // For now, we'll just log that it would be implemented
        SPARKY_LOG_DEBUG("Cubemap faces loading would be implemented here");
        return true;
    }
}