#pragma once

#ifdef HAS_GLFW
#include <vulkan/vulkan.h>
#endif

#include <vector>
#include <iostream>
#include <optional>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <unordered_map>
#include "MeshRenderer.h"
#include "Texture.h"
#include "Skybox.h"  // Add this include

// Forward declarations
namespace Sparky {
    class RenderSystem;
    class Engine;  
    class Material;  
    class Light;     
    class Skybox;    // Add this forward declaration
    
#ifdef HAS_GLFW
    // Vulkan-specific forward declarations
    struct QueueFamilyIndices;
    struct SwapChainSupportDetails;
    struct UniformBufferObject;
    struct PushConstantData;
    struct MaterialUniformBufferObject;
#endif
}

namespace Sparky {
#ifdef HAS_GLFW
    // Uniform buffer object structure
    struct UniformBufferObject {
        glm::mat4 view;
        glm::mat4 proj;
    };

    // Add push constant structure
    struct PushConstantData {
        glm::mat4 model;
    };

    // Add material uniform buffer structure
    struct MaterialUniformBufferObject {
        glm::vec4 ambient;
        glm::vec4 diffuse;
        glm::vec4 specular;
        float shininess;
        int hasTexture;
        int padding[2]; // For alignment
    };

    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete() {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };
#endif

    class VulkanRenderer {
    public:
        VulkanRenderer();
        ~VulkanRenderer();

        bool initialize(void* windowHandle);
        void cleanup();
        void render();
        void renderMeshes();  // Add this method
        MeshRenderer& getMeshRenderer();
        
#ifndef HAS_GLFW
        // Texture management (fallback implementations)
        void createTextureImage(const std::string& filepath, Texture& texture) {}
        void createTextureImageView(Texture& texture) {}
        void createTextureSampler(Texture& texture) {}
        void transitionImageLayout(void* image, void* format, void* oldLayout, void* newLayout) {}
        void copyBufferToImage(void* buffer, void* image, uint32_t width, uint32_t height) {}
        
        // Material system (fallback implementations)
        void createMaterialDescriptorSetLayout() {}
        void createMaterialDescriptorPool() {}
        void createMaterialDescriptorSets(Material* material) {}
        void updateMaterialDescriptorSet(Material* material) {}
        
        // Lighting system (fallback implementations)
        void updateLightingUniformBuffer(uint32_t currentImage, const std::vector<std::unique_ptr<Light>>& lights) {}
#endif

        // Setter for engine reference
        void setEngine(Engine* engine) { this->engine = engine; }

#ifdef HAS_GLFW
        // Getters for other classes to use
        VkDevice getDevice() const { return device; }
        VkPhysicalDevice getPhysicalDevice() const { return physicalDevice; }
        VkCommandPool getCommandPool() const { return commandPool; }
        VkQueue getGraphicsQueue() const { return graphicsQueue; }
        VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }
        VkDescriptorSetLayout getMaterialDescriptorSetLayout() const { return materialDescriptorSetLayout; }
        VkSampler getTextureSampler() const { return textureSampler; }
#endif

    private:
#ifdef HAS_GLFW
        // Vulkan instance
        VkInstance instance;
        VkPhysicalDevice physicalDevice;
        VkDevice device;
        VkQueue graphicsQueue;
        VkQueue presentQueue;
        VkSurfaceKHR surface;
        VkSwapchainKHR swapChain;
        std::vector<VkImage> swapChainImages;
        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;
        std::vector<VkImageView> swapChainImageViews;
        VkRenderPass renderPass;
        VkPipelineLayout pipelineLayout;
        VkPipeline graphicsPipeline;
        VkCommandPool commandPool;
        
        // Framebuffers
        std::vector<VkFramebuffer> swapChainFramebuffers;
        
        // Command buffers
        std::vector<VkCommandBuffer> commandBuffers;
        
        // Synchronization objects
        VkSemaphore imageAvailableSemaphore;
        VkSemaphore renderFinishedSemaphore;
        VkFence inFlightFence;
        const int MAX_FRAMES_IN_FLIGHT = 2;
        size_t currentFrame;
        
        // Descriptor set layout
        VkDescriptorSetLayout descriptorSetLayout;
        VkDescriptorSetLayout materialDescriptorSetLayout;
        
        // Uniform buffers
        std::vector<VkBuffer> uniformBuffers;
        std::vector<VkDeviceMemory> uniformBuffersMemory;
        std::vector<void*> uniformBuffersMapped;
        
        // Material uniform buffers
        std::vector<VkBuffer> materialUniformBuffers;
        std::vector<VkDeviceMemory> materialUniformBuffersMemory;
        std::vector<void*> materialUniformBuffersMapped;
        
        // Lighting uniform buffers
        std::vector<VkBuffer> lightingUniformBuffers;
        std::vector<VkDeviceMemory> lightingUniformBuffersMemory;
        std::vector<void*> lightingUniformBuffersMapped;
        
        // Descriptor pools and sets
        VkDescriptorPool descriptorPool;
        VkDescriptorPool materialDescriptorPool;
        std::vector<VkDescriptorSet> descriptorSets;
        
        // Depth resources
        VkImage depthImage;
        VkDeviceMemory depthImageMemory;
        VkImageView depthImageView;

        // Texture sampler
        VkSampler textureSampler;

        // Debug messenger
        VkDebugUtilsMessengerEXT debugMessenger;

        // Vulkan validation layers
        const std::vector<const char*> validationLayers = {
            "VK_LAYER_KHRONOS_validation"
        };

        // Vulkan extensions
        const std::vector<const char*> deviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

#ifdef NDEBUG
        const bool enableValidationLayers = false;
#else
        const bool enableValidationLayers = true;
#endif

        // Helper functions
        bool checkValidationLayerSupport();
        void createInstance();
        void setupDebugMessenger();
        void createSurface();
        void pickPhysicalDevice();
        void createLogicalDevice();
        void createSwapChain();
        void createImageViews();
        void createRenderPass();
        void createDescriptorSetLayout();
        void createGraphicsPipeline();
        void createCommandPool();
        void createDepthResources();
        void createFramebuffers();
        void createUniformBuffers();
        void createMaterialUniformBuffers();
        void createLightingUniformBuffers();
        void createDescriptorPool();
        void createDescriptorSets();
        void createSyncObjects();
        void createCommandBuffers();
        void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
        
        void cleanupSwapChain();
        void recreateSwapChain();
        void cleanupUniformBuffers();
        void updateUniformBuffer(uint32_t currentImage);
        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
        VkShaderModule createShaderModule(const std::vector<uint32_t>& code);
        bool isDeviceSuitable(VkPhysicalDevice device);
        bool checkDeviceExtensionSupport(VkPhysicalDevice device);
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
        VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
        VkFormat findDepthFormat();
        void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
        VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
        VkCommandBuffer beginSingleTimeCommands();
        void endSingleTimeCommands(VkCommandBuffer commandBuffer);
        void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
        void createMaterialDescriptorSetLayout();
        void createMaterialDescriptorPool();
        void createMaterialDescriptorSets(Material* material);
        void updateMaterialDescriptorSet(Material* material);
        void updateLightingUniformBuffer(uint32_t currentImage, const std::vector<std::unique_ptr<Light>>& lights);
#endif

        // Mesh renderer
        MeshRenderer meshRenderer;

        // Window handle
        void* windowHandle;
        
        // Engine reference
        Engine* engine;  // Add this member
        
        // Skybox support
        std::unique_ptr<Skybox> skybox;  // Add this member
    };
}
