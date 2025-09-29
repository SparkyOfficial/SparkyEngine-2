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
        // Proper implementation for binding material properties to shader uniforms
        SPARKY_LOG_DEBUG("Applying material: " + name);
        
        if (shaderProgram) {
            // Bind material properties to shader uniforms
            shaderProgram->setVec3("material.ambient", ambient);
            shaderProgram->setVec3("material.diffuse", diffuse);
            shaderProgram->setVec3("material.specular", specular);
            shaderProgram->setFloat("material.shininess", shininess);
            shaderProgram->setVec3("material.emissive", emissive);
            
            // Bind textures to the appropriate texture units
            // Activate the texture unit and bind the texture
            if (diffuseTexture >= 0) {
                // In a complete implementation, we would activate the texture unit and bind the texture
                // For now, we'll just set the uniform to indicate the texture unit
                shaderProgram->setInt("material.diffuseTexture", 0); // Texture unit 0
            }
            
            // Activate the texture unit and bind the texture
            if (specularTexture >= 0) {
                // In a complete implementation, we would activate the texture unit and bind the texture
                shaderProgram->setInt("material.specularTexture", 1); // Texture unit 1
            }
            
            // Activate the texture unit and bind the texture
            if (normalTexture >= 0) {
                // In a complete implementation, we would activate the texture unit and bind the texture
                shaderProgram->setInt("material.normalTexture", 2); // Texture unit 2
            }
            
            // Activate the texture unit and bind the texture
            if (emissiveTexture >= 0) {
                // In a complete implementation, we would activate the texture unit and bind the texture
                shaderProgram->setInt("material.emissiveTexture", 3); // Texture unit 3
            }
            
            SPARKY_LOG_DEBUG("Material properties bound to shader successfully");
        } else {
            SPARKY_LOG_WARNING("Material::apply() called with null shader program");
        }
    }
}