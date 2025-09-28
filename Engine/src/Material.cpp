#include "../include/Material.h"
#include "../include/Logger.h"

namespace Sparky {

    Material::Material() : name("DefaultMaterial"), ambient(0.2f, 0.2f, 0.2f), 
                          diffuse(0.8f, 0.8f, 0.8f), specular(1.0f, 1.0f, 1.0f), 
                          shininess(32.0f), emissive(0.0f, 0.0f, 0.0f),
                          diffuseTexture(-1), specularTexture(-1), 
                          normalTexture(-1), emissiveTexture(-1),
                          shaderProgram(nullptr) {
    }

    Material::Material(const std::string& name) : name(name), ambient(0.2f, 0.2f, 0.2f), 
                                                 diffuse(0.8f, 0.8f, 0.8f), specular(1.0f, 1.0f, 1.0f), 
                                                 shininess(32.0f), emissive(0.0f, 0.0f, 0.0f),
                                                 diffuseTexture(-1), specularTexture(-1), 
                                                 normalTexture(-1), emissiveTexture(-1),
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

    void Material::setDiffuseTexture(int textureId) {
        this->diffuseTexture = textureId;
    }

    void Material::setSpecularTexture(int textureId) {
        this->specularTexture = textureId;
    }

    void Material::setNormalTexture(int textureId) {
        this->normalTexture = textureId;
    }

    void Material::setEmissiveTexture(int textureId) {
        this->emissiveTexture = textureId;
    }

    void Material::setShaderProgram(ShaderProgram* shader) {
        this->shaderProgram = shader;
    }

    void Material::apply() const {
        // In a real implementation, this would bind the material properties to the shader
        SPARKY_LOG_DEBUG("Applying material: " + name);
        
        if (shaderProgram) {
            // Bind material properties to shader uniforms
            // This would typically involve:
            // - Setting uniform values for ambient, diffuse, specular, shininess
            // - Binding texture units
            // - Setting other material-specific parameters
            
            // Example (pseudo-code):
            // shaderProgram->setVec3("material.ambient", ambient);
            // shaderProgram->setVec3("material.diffuse", diffuse);
            // shaderProgram->setVec3("material.specular", specular);
            // shaderProgram->setFloat("material.shininess", shininess);
            // shaderProgram->setVec3("material.emissive", emissive);
            
            // Bind textures if available
            // if (diffuseTexture >= 0) {
            //     glActiveTexture(GL_TEXTURE0);
            //     glBindTexture(GL_TEXTURE_2D, diffuseTexture);
            //     shaderProgram->setInt("material.diffuseTexture", 0);
            // }
        }
    }
}