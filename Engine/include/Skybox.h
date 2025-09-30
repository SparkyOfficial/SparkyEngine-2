#pragma once

#include <glm/glm.hpp>
#include <string>
#include <memory>
#include <array>
#include <vulkan/vulkan.h>
#include "Mesh.h"
#include "Texture.h"
#include "Material.h"

namespace Sparky {
    
    class VulkanRenderer;
    
    class Skybox {
    public:
        Skybox();
        ~Skybox();
        
        bool initialize(VulkanRenderer* renderer);
        void cleanup();
        
        // Load skybox from 6 textures (one for each face)
        bool loadFromFile(const std::array<std::string, 6>& faceTextures);
        
        // Render the skybox
        void render(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, uint32_t imageIndex);
        
        // Getters
        bool isLoaded() const { return loaded; }
        Material* getMaterial() { return material.get(); }
        Mesh* getMesh() { return mesh.get(); }
        
        // Vulkan-specific getters
        VkImageView getCubemapImageView() const { return cubemapImageView; }
        VkSampler getCubemapSampler() const { return cubemapSampler; }
        
    private:
        std::unique_ptr<Mesh> mesh;
        std::unique_ptr<Material> material;
        std::array<std::unique_ptr<Texture>, 6> faceTextures;
        bool loaded;
        
        // Vulkan-specific resources
        VkImage cubemapImage;
        VkDeviceMemory cubemapImageMemory;
        VkImageView cubemapImageView;
        VkSampler cubemapSampler;
        
        // Create a cube mesh for the skybox
        std::unique_ptr<Mesh> createCubeMesh();
        
        // Create cubemap texture from 6 face textures
        bool createCubemapTexture(VulkanRenderer* renderer, const std::array<std::string, 6>& faceTextures);
        
        // Vulkan helper functions
        bool createCubemapImage(VulkanRenderer* renderer, uint32_t width, uint32_t height);
        bool createCubemapImageView(VulkanRenderer* renderer);
        bool createCubemapSampler(VulkanRenderer* renderer);
        bool loadCubemapFaces(VulkanRenderer* renderer, const std::array<std::string, 6>& faceTextures);
    };
}