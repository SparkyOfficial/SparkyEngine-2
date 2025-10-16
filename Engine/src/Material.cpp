#include "../include/Material.h"
#include "../include/Logger.h"
#include "../include/Texture.h"
#include "../include/ShaderProgram.h"

namespace Sparky {

    Material::Material() : name("DefaultMaterial"), ambient(0.2f, 0.2f, 0.2f), 
                          diffuse(0.8f, 0.8f, 0.8f), specular(1.0f, 1.0f, 1.0f), 
                          shininess(32.0f), emissive(0.0f, 0.0f, 0.0f),
                          roughness(0.5f), metalness(0.0f), useIBL(true),
                          texture(nullptr), normalMap(nullptr),
                          roughnessMap(nullptr), metalnessMap(nullptr),
                          shaderProgram(nullptr) {
    }

    Material::Material(const std::string& name) : name(name), ambient(0.2f, 0.2f, 0.2f), 
                                                 diffuse(0.8f, 0.8f, 0.8f), specular(1.0f, 1.0f, 1.0f), 
                                                 shininess(32.0f), emissive(0.0f, 0.0f, 0.0f),
                                                 roughness(0.5f), metalness(0.0f), useIBL(true),
                                                 texture(nullptr), normalMap(nullptr),
                                                 roughnessMap(nullptr), metalnessMap(nullptr),
                                                 shaderProgram(nullptr) {
    }

    Material::~Material() {
    }

    void Material::setAmbient(const glm::vec3& ambient) {
        this->ambient = ambient;
    }

    void Material::setDiffuse(const glm::vec3& diffuse) {
        this->diffuse = diffuse;
    }

    void Material::setSpecular(const glm::vec3& specular) {
        this->specular = specular;
    }

    void Material::setShininess(float shininess) {
        this->shininess = shininess;
    }

    void Material::setEmissive(const glm::vec3& emissive) {
        this->emissive = emissive;
    }
    
    void Material::setRoughness(float roughness) {
        this->roughness = roughness;
    }
    
    void Material::setMetalness(float metalness) {
        this->metalness = metalness;
    }
    
    void Material::setNormalMap(Texture* normalMap) {
        this->normalMap = normalMap;
    }
    
    void Material::setRoughnessMap(Texture* roughnessMap) {
        this->roughnessMap = roughnessMap;
    }
    
    void Material::setMetalnessMap(Texture* metalnessMap) {
        this->metalnessMap = metalnessMap;
    }
    
    void Material::setUseIBL(bool useIBL) {
        this->useIBL = useIBL;
    }

    void Material::setTexture(Texture* texture) {
        this->texture = texture;
    }

    void Material::setShaderProgram(ShaderProgram* shader) {
        this->shaderProgram = shader;
    }

    void Material::apply() const {
        // Proper implementation for binding material properties to shader uniforms
        SPARKY_LOG_DEBUG("Applying material: " + name);
        
        if (shaderProgram) {
            // Bind material properties to shader uniforms
            shaderProgram->setVec3("material.ambient", ambient);
            shaderProgram->setVec3("material.diffuse", diffuse);
            shaderProgram->setVec3("material.specular", specular);
            shaderProgram->setFloat("material.shininess", shininess);
            shaderProgram->setVec3("material.emissive", emissive);
            shaderProgram->setFloat("material.roughness", roughness);
            shaderProgram->setFloat("material.metalness", metalness);
            shaderProgram->setInt("material.hasTexture", texture ? 1 : 0);
            shaderProgram->setInt("material.hasNormalMap", normalMap ? 1 : 0);
            shaderProgram->setInt("material.hasRoughnessMap", roughnessMap ? 1 : 0);
            shaderProgram->setInt("material.hasMetalnessMap", metalnessMap ? 1 : 0);
            shaderProgram->setInt("material.useIBL", useIBL ? 1 : 0);
            
            // Bind textures to the appropriate texture units
            // Activate the texture unit and bind the texture
            if (texture) {
                // In a complete implementation, we would activate the texture unit and bind the texture
                // For now, we'll just set the uniform to indicate the texture unit
                shaderProgram->setInt("material.texture", 0); // Texture unit 0
            }
            
            SPARKY_LOG_DEBUG("Material properties bound to shader successfully");
        } else {
            SPARKY_LOG_WARNING("Material::apply() called with null shader program");
        }
    }
}