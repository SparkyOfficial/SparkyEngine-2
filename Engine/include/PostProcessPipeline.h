#pragma once

#ifdef HAS_GLFW
#include <vulkan/vulkan.h>
#endif

#include "VulkanRenderer.h"
#include <memory>

namespace Sparky {
    /**
     * @brief Post-Processing Pipeline
     * 
     * Implements various post-processing effects including:
     * - HDR tone mapping
     * - Bloom
     * - Gamma correction
     * - Chromatic aberration
     * - Film grain
     * - Motion blur
     * - Depth of field
     */
    class PostProcessPipeline {
    public:
        PostProcessPipeline();
        ~PostProcessPipeline();

        // Initialization
        bool initialize(VulkanRenderer* renderer);
        void cleanup();

        // Effect controls
        void setHDREnabled(bool enabled);
        bool isHDREnabled() const;

        void setBloomEnabled(bool enabled);
        bool isBloomEnabled() const;
        
        void setExposure(float exposure);
        float getExposure() const;
        
        void setGamma(float gamma);
        float getGamma() const;
        
        void setChromaticAberrationEnabled(bool enabled);
        bool isChromaticAberrationEnabled() const;
        
        void setChromaticAberrationIntensity(float intensity);
        float getChromaticAberrationIntensity() const;
        
        void setFilmGrainEnabled(bool enabled);
        bool isFilmGrainEnabled() const;
        
        void setFilmGrainIntensity(float intensity);
        float getFilmGrainIntensity() const;
        
        void setMotionBlurEnabled(bool enabled);
        bool isMotionBlurEnabled() const;
        
        void setMotionBlurIntensity(float intensity);
        float getMotionBlurIntensity() const;
        
        void setDepthOfFieldEnabled(bool enabled);
        bool isDepthOfFieldEnabled() const;
        
        void setDepthOfFieldFocusDistance(float distance);
        float getDepthOfFieldFocusDistance() const;
        
        void setDepthOfFieldAperture(float aperture);
        float getDepthOfFieldAperture() const;

        // Rendering
        void beginPostProcessPass();
        void endPostProcessPass();
        
        // Get post-process texture ID for shader binding
        unsigned int getPostProcessTextureId() const;

    private:
        // Private methods
        void createPostProcessResources();
        void destroyPostProcessResources();
        void updatePostProcessUniforms();

        // Vulkan resources
        VulkanRenderer* m_renderer;
        
        // Effect states
        bool m_hdrEnabled;
        bool m_bloomEnabled;
        bool m_chromaticAberrationEnabled;
        bool m_filmGrainEnabled;
        bool m_motionBlurEnabled;
        bool m_depthOfFieldEnabled;
        
        // Effect parameters
        float m_exposure;
        float m_gamma;
        float m_chromaticAberrationIntensity;
        float m_filmGrainIntensity;
        float m_motionBlurIntensity;
        float m_dofFocusDistance;
        float m_dofAperture;
        
        // Vulkan-specific resources
#ifdef HAS_GLFW
        VkImage m_postProcessImage;
        VkDeviceMemory m_postProcessMemory;
        VkImageView m_postProcessImageView;
        VkSampler m_postProcessSampler;
        VkFramebuffer m_postProcessFramebuffer;
        VkRenderPass m_postProcessRenderPass;
        VkBuffer m_uniformBuffer;
        VkDeviceMemory m_uniformBufferMemory;
        void* m_uniformBufferMapped;
#else
        void* m_postProcessImage;
        void* m_postProcessMemory;
        void* m_postProcessImageView;
        void* m_postProcessSampler;
        void* m_postProcessFramebuffer;
        void* m_postProcessRenderPass;
        void* m_uniformBuffer;
        void* m_uniformBufferMemory;
        void* m_uniformBufferMapped;
#endif
        
        // State tracking
        bool m_initialized;
    };
}