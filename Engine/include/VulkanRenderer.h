#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <iostream>
#include <optional>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "MeshRenderer.h"

// Forward declaration
namespace Sparky {
    class RenderSystem;
}

namespace Sparky {
    // Uniform buffer object structure
    struct UniformBufferObject {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
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

    class VulkanRenderer {
    public:
        VulkanRenderer();
        ~VulkanRenderer();

        bool initialize(void* windowHandle);
        void cleanup();
        void render();
        MeshRenderer& getMeshRenderer() { return meshRenderer; }

        // Getters for other classes to use
        VkDevice getDevice() const { return device; }
        VkPhysicalDevice getPhysicalDevice() const { return physicalDevice; }
        VkCommandPool getCommandPool() const { return commandPool; }
        VkQueue getGraphicsQueue() const { return graphicsQueue; }

    private:
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
        
        // Uniform buffers
        std::vector<VkBuffer> uniformBuffers;
        std::vector<VkDeviceMemory> uniformBuffersMemory;
        std::vector<void*> uniformBuffersMapped;
        
        // Descriptor pools and sets
        VkDescriptorPool descriptorPool;
        std::vector<VkDescriptorSet> descriptorSets;
        
        // Depth resources
        VkImage depthImage;
        VkDeviceMemory depthImageMemory;
        VkImageView depthImageView;

        // Mesh renderer
        MeshRenderer meshRenderer;

        // Window handle
        void* windowHandle;
        
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
        void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
        VkCommandBuffer beginSingleTimeCommands();
        void endSingleTimeCommands(VkCommandBuffer commandBuffer);
    };
}