#pragma once

#include <glm/glm.hpp>
#include <memory>

#ifdef HAS_GLFW
#include <vulkan/vulkan.h>
#endif

namespace Sparky {
    class VulkanRenderer;
    
    /**
     * @brief Shadow Map System
     * 
     * Implements cascaded shadow maps (CSM) for directional lights and
     * omnidirectional shadow maps for point lights.
     */
    class ShadowMap {
    public:
        ShadowMap();
        ~ShadowMap();
        
        // Initialization
        bool initialize(VulkanRenderer* renderer, uint32_t width, uint32_t height);
        void cleanup();
        
        // Shadow map properties
        void setShadowMapSize(uint32_t width, uint32_t height);
        void getShadowMapSize(uint32_t& width, uint32_t& height) const;
        
        // Cascaded Shadow Maps for directional lights
        void setNumCascades(int numCascades);
        int getNumCascades() const;
        
        void setCascadeSplits(const float* splits);
        const float* getCascadeSplits() const;
        
        // Shadow quality settings
        void setShadowBias(float bias);
        float getShadowBias() const;
        
        void setShadowBlur(float blur);
        float getShadowBlur() const;
        
        // Light setup
        void setLightDirection(const glm::vec3& direction);
        const glm::vec3& getLightDirection() const;
        
        void setLightPosition(const glm::vec3& position);
        const glm::vec3& getLightPosition() const;
        
        // Shadow rendering
        void beginShadowPass();
        void endShadowPass();
        
        // Get shadow map texture ID for shader binding
        unsigned int getShadowMapTextureId() const;
        
        // Get light space matrices for each cascade
        const glm::mat4& getLightSpaceMatrix(int cascadeIndex) const;
        
    private:
        // Private methods
        void createShadowMapResources();
        void destroyShadowMapResources();
        void updateLightSpaceMatrices();
        
        // Vulkan resources
        VulkanRenderer* m_renderer;
        
        // Shadow map properties
        uint32_t m_width;
        uint32_t m_height;
        int m_numCascades;
        float m_cascadeSplits[4]; // Up to 4 cascades
        
        // Shadow quality
        float m_shadowBias;
        float m_shadowBlur;
        
        // Light properties
        glm::vec3 m_lightDirection;
        glm::vec3 m_lightPosition;
        bool m_isDirectionalLight;
        
        // Light space matrices for each cascade
        glm::mat4 m_lightSpaceMatrices[4];
        
        // Vulkan-specific resources
#ifdef HAS_GLFW
        VkImage m_shadowMapImage;
        VkDeviceMemory m_shadowMapMemory;
        VkImageView m_shadowMapImageView;
        VkSampler m_shadowMapSampler;
        VkFramebuffer m_shadowMapFramebuffer;
        VkRenderPass m_shadowMapRenderPass;
#else
        void* m_shadowMapImage;
        void* m_shadowMapMemory;
        void* m_shadowMapImageView;
        void* m_shadowMapSampler;
        void* m_shadowMapFramebuffer;
        void* m_shadowMapRenderPass;
#endif
        
        // State tracking
        bool m_initialized;
    };
}