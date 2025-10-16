#pragma once

#include "Material.h"
#include <glm/glm.hpp>
#include <memory>

namespace Sparky {
    /**
     * @brief Physically Based Rendering Material
     * 
     * Implements a physically based rendering material system with:
     * - Albedo (base color)
     * - Normal mapping
     * - Metallic workflow
     * - Roughness workflow
     * - Ambient Occlusion
     * - Emissive properties
     */
    class PBRMaterial : public Material {
    public:
        PBRMaterial(const std::string& name = "PBRMaterial");
        virtual ~PBRMaterial() = default;

        // Albedo (base color) properties
        void setAlbedo(const glm::vec3& albedo);
        glm::vec3 getAlbedo() const;

        // Normal mapping
        void setNormalMapEnabled(bool enabled);
        bool isNormalMapEnabled() const;

        // Metallic properties
        void setMetallic(float metallic);
        float getMetallic() const;

        // Roughness properties
        void setRoughness(float roughness);
        float getRoughness() const;

        // Ambient Occlusion
        void setAo(float ao);
        float getAo() const;

        // Emissive properties
        void setEmissive(const glm::vec3& emissive);
        glm::vec3 getEmissive() const;

        // Texture support
        void setAlbedoTexture(unsigned int textureId);
        unsigned int getAlbedoTexture() const;

        void setNormalTexture(unsigned int textureId);
        unsigned int getNormalTexture() const;

        void setMetallicTexture(unsigned int textureId);
        unsigned int getMetallicTexture() const;

        void setRoughnessTexture(unsigned int textureId);
        unsigned int getRoughnessTexture() const;

        void setAoTexture(unsigned int textureId);
        unsigned int getAoTexture() const;

        void setEmissiveTexture(unsigned int textureId);
        unsigned int getEmissiveTexture() const;

        // PBR-specific rendering parameters
        void setUseIBL(bool useIBL);
        bool getUseIBL() const;

        void setUseDirectLighting(bool useDirect);
        bool getUseDirectLighting() const;

        // Material blending
        void setBlendMode(int blendMode);
        int getBlendMode() const;

        // Alpha testing
        void setAlphaTest(bool alphaTest);
        bool getAlphaTest() const;

        void setAlphaThreshold(float threshold);
        float getAlphaThreshold() const;

    private:
        // PBR properties
        glm::vec3 m_albedo;
        float m_metallic;
        float m_roughness;
        float m_ao;
        glm::vec3 m_emissive;

        // Texture IDs
        unsigned int m_albedoTexture;
        unsigned int m_normalTexture;
        unsigned int m_metallicTexture;
        unsigned int m_roughnessTexture;
        unsigned int m_aoTexture;
        unsigned int m_emissiveTexture;

        // Feature flags
        bool m_normalMapEnabled;
        bool m_useIBL;
        bool m_useDirectLighting;
        bool m_alphaTest;
        float m_alphaThreshold;
        int m_blendMode;
    };
}