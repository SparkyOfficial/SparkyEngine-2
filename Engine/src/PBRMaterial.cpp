#include "../include/PBRMaterial.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Sparky {
    PBRMaterial::PBRMaterial(const std::string& name)
        : Material(name)
        , m_albedo(1.0f, 1.0f, 1.0f)
        , m_metallic(0.0f)
        , m_roughness(0.5f)
        , m_ao(1.0f)
        , m_emissive(0.0f, 0.0f, 0.0f)
        , m_albedoTexture(0)
        , m_normalTexture(0)
        , m_metallicTexture(0)
        , m_roughnessTexture(0)
        , m_aoTexture(0)
        , m_emissiveTexture(0)
        , m_normalMapEnabled(false)
        , m_useIBL(true)
        , m_useDirectLighting(true)
        , m_alphaTest(false)
        , m_alphaThreshold(0.5f)
        , m_blendMode(0) {
    }

    void PBRMaterial::setAlbedo(const glm::vec3& albedo) {
        m_albedo = albedo;
    }

    glm::vec3 PBRMaterial::getAlbedo() const {
        return m_albedo;
    }

    void PBRMaterial::setNormalMapEnabled(bool enabled) {
        m_normalMapEnabled = enabled;
    }

    bool PBRMaterial::isNormalMapEnabled() const {
        return m_normalMapEnabled;
    }

    void PBRMaterial::setMetallic(float metallic) {
        m_metallic = metallic;
    }

    float PBRMaterial::getMetallic() const {
        return m_metallic;
    }

    void PBRMaterial::setRoughness(float roughness) {
        m_roughness = roughness;
    }

    float PBRMaterial::getRoughness() const {
        return m_roughness;
    }

    void PBRMaterial::setAo(float ao) {
        m_ao = ao;
    }

    float PBRMaterial::getAo() const {
        return m_ao;
    }

    void PBRMaterial::setEmissive(const glm::vec3& emissive) {
        m_emissive = emissive;
    }

    glm::vec3 PBRMaterial::getEmissive() const {
        return m_emissive;
    }

    void PBRMaterial::setAlbedoTexture(unsigned int textureId) {
        m_albedoTexture = textureId;
    }

    unsigned int PBRMaterial::getAlbedoTexture() const {
        return m_albedoTexture;
    }

    void PBRMaterial::setNormalTexture(unsigned int textureId) {
        m_normalTexture = textureId;
    }

    unsigned int PBRMaterial::getNormalTexture() const {
        return m_normalTexture;
    }

    void PBRMaterial::setMetallicTexture(unsigned int textureId) {
        m_metallicTexture = textureId;
    }

    unsigned int PBRMaterial::getMetallicTexture() const {
        return m_metallicTexture;
    }

    void PBRMaterial::setRoughnessTexture(unsigned int textureId) {
        m_roughnessTexture = textureId;
    }

    unsigned int PBRMaterial::getRoughnessTexture() const {
        return m_roughnessTexture;
    }

    void PBRMaterial::setAoTexture(unsigned int textureId) {
        m_aoTexture = textureId;
    }

    unsigned int PBRMaterial::getAoTexture() const {
        return m_aoTexture;
    }

    void PBRMaterial::setEmissiveTexture(unsigned int textureId) {
        m_emissiveTexture = textureId;
    }

    unsigned int PBRMaterial::getEmissiveTexture() const {
        return m_emissiveTexture;
    }

    void PBRMaterial::setUseIBL(bool useIBL) {
        m_useIBL = useIBL;
    }

    bool PBRMaterial::getUseIBL() const {
        return m_useIBL;
    }

    void PBRMaterial::setUseDirectLighting(bool useDirect) {
        m_useDirectLighting = useDirect;
    }

    bool PBRMaterial::getUseDirectLighting() const {
        return m_useDirectLighting;
    }

    void PBRMaterial::setBlendMode(int blendMode) {
        m_blendMode = blendMode;
    }

    int PBRMaterial::getBlendMode() const {
        return m_blendMode;
    }

    void PBRMaterial::setAlphaTest(bool alphaTest) {
        m_alphaTest = alphaTest;
    }

    bool PBRMaterial::getAlphaTest() const {
        return m_alphaTest;
    }

    void PBRMaterial::setAlphaThreshold(float threshold) {
        m_alphaThreshold = threshold;
    }

    float PBRMaterial::getAlphaThreshold() const {
        return m_alphaThreshold;
    }
}