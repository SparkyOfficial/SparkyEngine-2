#include "../include/ShadowMap.h"
#include "../include/VulkanRenderer.h"
#include <cstring> // For memset
#include <algorithm> // For std::max, std::min

namespace Sparky {
    ShadowMap::ShadowMap()
        : m_renderer(nullptr)
        , m_width(1024)
        , m_height(1024)
        , m_numCascades(4)
        , m_shadowBias(0.005f)
        , m_shadowBlur(0.0f)
        , m_lightDirection(0.0f, -1.0f, 0.0f)
        , m_lightPosition(0.0f, 0.0f, 0.0f)
        , m_isDirectionalLight(true)
        , m_initialized(false) {
        // Initialize cascade splits
        m_cascadeSplits[0] = 0.1f;
        m_cascadeSplits[1] = 0.3f;
        m_cascadeSplits[2] = 0.6f;
        m_cascadeSplits[3] = 1.0f;
        
        // Initialize light space matrices
        for (int i = 0; i < 4; ++i) {
            m_lightSpaceMatrices[i] = glm::mat4(1.0f);
        }
        
        // Initialize Vulkan resources to null
#ifdef HAS_GLFW
        m_shadowMapImage = VK_NULL_HANDLE;
        m_shadowMapMemory = VK_NULL_HANDLE;
        m_shadowMapImageView = VK_NULL_HANDLE;
        m_shadowMapSampler = VK_NULL_HANDLE;
        m_shadowMapFramebuffer = VK_NULL_HANDLE;
        m_shadowMapRenderPass = VK_NULL_HANDLE;
#endif
    }

    ShadowMap::~ShadowMap() {
        cleanup();
    }

    bool ShadowMap::initialize(VulkanRenderer* renderer, uint32_t width, uint32_t height) {
        if (!renderer) {
            return false;
        }
        
        m_renderer = renderer;
        m_width = width;
        m_height = height;
        
#ifdef HAS_GLFW
        createShadowMapResources();
#endif
        
        m_initialized = true;
        return true;
    }

    void ShadowMap::cleanup() {
        if (!m_initialized) {
            return;
        }
        
#ifdef HAS_GLFW
        destroyShadowMapResources();
#endif
        
        m_initialized = false;
    }

    void ShadowMap::setShadowMapSize(uint32_t width, uint32_t height) {
        m_width = width;
        m_height = height;
    }

    void ShadowMap::getShadowMapSize(uint32_t& width, uint32_t& height) const {
        width = m_width;
        height = m_height;
    }

    void ShadowMap::setNumCascades(int numCascades) {
        m_numCascades = std::max(1, std::min(numCascades, 4)); // Clamp between 1 and 4
    }

    int ShadowMap::getNumCascades() const {
        return m_numCascades;
    }

    void ShadowMap::setCascadeSplits(const float* splits) {
        if (splits) {
            for (int i = 0; i < 4; ++i) {
                m_cascadeSplits[i] = splits[i];
            }
        }
    }

    const float* ShadowMap::getCascadeSplits() const {
        return m_cascadeSplits;
    }

    void ShadowMap::setShadowBias(float bias) {
        m_shadowBias = bias;
    }

    float ShadowMap::getShadowBias() const {
        return m_shadowBias;
    }

    void ShadowMap::setShadowBlur(float blur) {
        m_shadowBlur = blur;
    }

    float ShadowMap::getShadowBlur() const {
        return m_shadowBlur;
    }

    void ShadowMap::setLightDirection(const glm::vec3& direction) {
        m_lightDirection = glm::normalize(direction);
        m_isDirectionalLight = true;
        updateLightSpaceMatrices();
    }

    const glm::vec3& ShadowMap::getLightDirection() const {
        return m_lightDirection;
    }

    void ShadowMap::setLightPosition(const glm::vec3& position) {
        m_lightPosition = position;
        m_isDirectionalLight = false;
        updateLightSpaceMatrices();
    }

    const glm::vec3& ShadowMap::getLightPosition() const {
        return m_lightPosition;
    }

    void ShadowMap::beginShadowPass() {
        // In a real implementation, this would:
        // 1. Begin the shadow map render pass
        // 2. Bind the shadow map framebuffer
        // 3. Clear the depth buffer
        // 4. Set up viewport and scissor for shadow map resolution
    }

    void ShadowMap::endShadowPass() {
        // In a real implementation, this would:
        // 1. End the shadow map render pass
        // 2. Transition the shadow map image layout for shader reading
    }

    unsigned int ShadowMap::getShadowMapTextureId() const {
        // In a real implementation, this would return the Vulkan texture ID
        // or a handle to the shadow map texture that can be bound in shaders
        return 0; // Placeholder
    }

    const glm::mat4& ShadowMap::getLightSpaceMatrix(int cascadeIndex) const {
        if (cascadeIndex >= 0 && cascadeIndex < 4) {
            return m_lightSpaceMatrices[cascadeIndex];
        }
        return m_lightSpaceMatrices[0]; // Return first matrix as fallback
    }

    void ShadowMap::createShadowMapResources() {
#ifdef HAS_GLFW
        // In a real implementation, this would:
        // 1. Create the shadow map image (depth format)
        // 2. Allocate memory for the shadow map image
        // 3. Create the image view for the shadow map
        // 4. Create the sampler for shadow map sampling
        // 5. Create the render pass for shadow mapping
        // 6. Create the framebuffer for shadow mapping
        
        // Placeholder implementation - in a real engine, this would contain
        // actual Vulkan API calls to create these resources
#endif
    }

    void ShadowMap::destroyShadowMapResources() {
#ifdef HAS_GLFW
        // In a real implementation, this would:
        // 1. Destroy the shadow map framebuffer
        // 2. Destroy the shadow map render pass
        // 3. Destroy the shadow map sampler
        // 4. Destroy the shadow map image view
        // 5. Free the shadow map image memory
        // 6. Destroy the shadow map image
        
        // Placeholder implementation - in a real engine, this would contain
        // actual Vulkan API calls to destroy these resources
        
        m_shadowMapImage = VK_NULL_HANDLE;
        m_shadowMapMemory = VK_NULL_HANDLE;
        m_shadowMapImageView = VK_NULL_HANDLE;
        m_shadowMapSampler = VK_NULL_HANDLE;
        m_shadowMapFramebuffer = VK_NULL_HANDLE;
        m_shadowMapRenderPass = VK_NULL_HANDLE;
#endif
    }

    void ShadowMap::updateLightSpaceMatrices() {
        // In a real implementation, this would:
        // 1. Calculate the light space matrices for each cascade
        // 2. For directional lights, calculate the view matrix from the light direction
        // 3. For point lights, calculate the view matrix from the light position
        // 4. Calculate the projection matrix for the light's view frustum
        // 5. Combine view and projection matrices to get light space matrices
        
        // Placeholder implementation
        for (int i = 0; i < 4; ++i) {
            m_lightSpaceMatrices[i] = glm::mat4(1.0f);
        }
    }
}