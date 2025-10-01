#pragma once

#include <glm/glm.hpp>
#include <string>
#include <memory>
#include <array>

// Forward declarations
namespace Sparky {
    class Mesh;
    class Material;
    class Texture;
    class VulkanRenderer;
}

#ifdef HAS_GLFW
#include <vulkan/vulkan.h>
#endif

namespace Sparky {
    
    class Skybox {
    public:
        Skybox();
        ~Skybox();
        
        bool initialize(VulkanRenderer* renderer);
        void cleanup(VulkanRenderer* renderer = nullptr);
        
        // Load skybox from 6 textures (one for each face)
        bool loadFromFile(const std::array<std::string, 6>& faceTextures);
        
        // Render the skybox
#ifdef HAS_GLFW
        void render(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, uint32_t imageIndex);
#else
        void render(void* commandBuffer, void* pipelineLayout, uint32_t imageIndex);
#endif
        
        // Getters
        bool isLoaded() const { return loaded; }
        Material* getMaterial() { return material.get(); }
        Mesh* getMesh() { return mesh.get(); }
        
        // Vulkan-specific getters
#ifdef HAS_GLFW
        VkImageView getCubemapImageView() const { return cubemapImageView; }
        VkSampler getCubemapSampler() const { return cubemapSampler; }
#else
        void* getCubemapImageView() const { return nullptr; }
        void* getCubemapSampler() const { return nullptr; }
#endif
        
    private:
        std::unique_ptr<Mesh> mesh;
        std::unique_ptr<Material> material;
        std::array<std::unique_ptr<Texture>, 6> faceTextures;
        bool loaded;
        
        // Vulkan-specific resources
#ifdef HAS_GLFW
        VkImage cubemapImage;
        VkDeviceMemory cubemapImageMemory;
        VkImageView cubemapImageView;
        VkSampler cubemapSampler;
#else
        void* cubemapImage;
        void* cubemapImageMemory;
        void* cubemapImageView;
        void* cubemapSampler;
#endif
        
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