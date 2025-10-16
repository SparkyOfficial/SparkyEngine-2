#pragma once

#include "VulkanRenderer.h"
#include "PBRMaterial.h"
#include "Mesh.h"
#include "Camera.h"
#include "Light.h"
#include "ShadowMap.h"
#include "PostProcessPipeline.h"
#include <glm/glm.hpp>
#include <vector>
#include <memory>

namespace Sparky {
    /**
     * @brief Advanced Physically Based Rendering Renderer
     * 
     * Implements advanced PBR techniques including:
     * - Image-Based Lighting (IBL)
     * - HDR rendering
     * - Tone mapping
     * - Screen Space Reflections (SSR)
     * - Screen Space Ambient Occlusion (SSAO)
     * - Global Illumination approximation
     * - Shadow mapping with Cascaded Shadow Maps (CSM)
     * - Post-processing effects
     */
    class PBRRenderer {
    public:
        PBRRenderer();
        ~PBRRenderer();

        // Initialization
        bool initialize(VulkanRenderer* vulkanRenderer);
        void shutdown();

        // Rendering
        void beginScene(const Camera& camera);
        void endScene();
        
        // Submit objects for rendering
        void submitMesh(const Mesh& mesh, const PBRMaterial& material, const glm::mat4& modelMatrix);
        
        // Lighting
        void setDirectionalLight(const Light& light);
        void addPointLight(const Light& light);
        void clearPointLights();
        
        // Shadow mapping
        void setShadowMap(ShadowMap* shadowMap);
        ShadowMap* getShadowMap() const;
        
        void setShadowsEnabled(bool enabled);
        bool isShadowsEnabled() const;
        
        // Post-processing
        PostProcessPipeline* getPostProcessPipeline() const;
        
        void setHDR(bool enabled);
        bool isHDREnabled() const;
        
        void setBloom(bool enabled);
        bool isBloomEnabled() const;
        
        void setExposure(float exposure);
        float getExposure() const;
        
        void setGamma(float gamma);
        float getGamma() const;
        
        void setChromaticAberrationEnabled(bool enabled);
        bool isChromaticAberrationEnabled() const;
        
        void setFilmGrainEnabled(bool enabled);
        bool isFilmGrainEnabled() const;
        
        void setMotionBlurEnabled(bool enabled);
        bool isMotionBlurEnabled() const;
        
        void setDepthOfFieldEnabled(bool enabled);
        bool isDepthOfFieldEnabled() const;

        // IBL (Image-Based Lighting)
        void setIBL(bool enabled);
        bool isIBLEnabled() const;
        
        void setEnvironmentMap(unsigned int cubemapTextureId);
        unsigned int getEnvironmentMap() const;

        // Render to screen
        void render();

    private:
        // Internal structures
        struct RenderObject {
            const Mesh* mesh;
            const PBRMaterial* material;
            glm::mat4 modelMatrix;
        };

        // Private methods
        void setupPBRPipeline();
        void setupPostProcessing();
        void setupIBL();
        void setupShadowMapping();
        
        void renderScene();
        void renderPostProcessing();
        void renderToScreen();
        void renderShadowMap();

        // Vulkan resources
        VulkanRenderer* m_vulkanRenderer;
        
        // Rendering state
        bool m_initialized;
        bool m_hdrEnabled;
        bool m_bloomEnabled;
        bool m_iblEnabled;
        bool m_shadowsEnabled;
        
        // Camera and lighting
        Camera m_camera;
        Light m_directionalLight;
        std::vector<Light> m_pointLights;
        
        // Shadow mapping
        ShadowMap* m_shadowMap;
        
        // Post-processing
        std::unique_ptr<PostProcessPipeline> m_postProcessPipeline;
        
        // PBR parameters
        float m_exposure;
        float m_gamma;
        
        // IBL resources
        unsigned int m_environmentMap;
        
        // Scene objects
        std::vector<RenderObject> m_renderObjects;
        
        // Vulkan-specific resources (implementation details would be in the .cpp file)
#ifdef HAS_GLFW
        // Vulkan pipeline and descriptor sets would be defined here
        void* m_pbrPipeline;  // Placeholder for actual Vulkan pipeline
        void* m_postProcessPipelineInternal;  // Placeholder for post-processing pipeline
        void* m_shadowPipeline;  // Placeholder for shadow mapping pipeline
#else
        void* m_pbrPipeline;
        void* m_postProcessPipelineInternal;
        void* m_shadowPipeline;
#endif
    };
}        // IBL resources
        unsigned int m_environmentMap;
        
        // Scene objects
        std::vector<RenderObject> m_renderObjects;
        
        // Vulkan-specific resources (implementation details would be in the .cpp file)
#ifdef HAS_GLFW
        // Vulkan pipeline and descriptor sets would be defined here
        void* m_pbrPipeline;  // Placeholder for actual Vulkan pipeline
        void* m_postProcessPipelineInternal;  // Placeholder for post-processing pipeline
        void* m_shadowPipeline;  // Placeholder for shadow mapping pipeline
#else
        void* m_pbrPipeline;
        void* m_postProcessPipelineInternal;
        void* m_shadowPipeline;
#endif
    };
}