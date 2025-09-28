#pragma once

#include <string>
#include <glm/glm.hpp>

namespace Sparky {
    class ShaderProgram;

    class Material {
    public:
        Material();
        Material(const std::string& name);
        ~Material();

        // Material properties
        void setAmbient(const glm::vec3& ambient);
        void setDiffuse(const glm::vec3& diffuse);
        void setSpecular(const glm::vec3& specular);
        void setShininess(float shininess);
        void setEmissive(const glm::vec3& emissive);

        const glm::vec3& getAmbient() const { return ambient; }
        const glm::vec3& getDiffuse() const { return diffuse; }
        const glm::vec3& getSpecular() const { return specular; }
        float getShininess() const { return shininess; }
        const glm::vec3& getEmissive() const { return emissive; }

        // Texture support
        void setDiffuseTexture(int textureId);
        void setSpecularTexture(int textureId);
        void setNormalTexture(int textureId);
        void setEmissiveTexture(int textureId);

        int getDiffuseTexture() const { return diffuseTexture; }
        int getSpecularTexture() const { return specularTexture; }
        int getNormalTexture() const { return normalTexture; }
        int getEmissiveTexture() const { return emissiveTexture; }

        // Shader program
        void setShaderProgram(ShaderProgram* shader);
        ShaderProgram* getShaderProgram() const { return shaderProgram; }

        const std::string& getName() const { return name; }

        // Apply material to current rendering context
        void apply() const;

    private:
        std::string name;
        
        // Basic material properties
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        float shininess;
        glm::vec3 emissive;

        // Textures
        int diffuseTexture;
        int specularTexture;
        int normalTexture;
        int emissiveTexture;

        // Shader program
        ShaderProgram* shaderProgram;
    };
}