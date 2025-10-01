#pragma once

#include <string>
#include <glm/glm.hpp>
#include <vector>

#ifdef HAS_GLFW
#include <vulkan/vulkan.h>
#endif

namespace Sparky {
    class ShaderProgram;
    class Texture;

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
        void setTexture(Texture* texture);
        Texture* getTexture() const { return texture; }

        // Vulkan-specific
#ifdef HAS_GLFW
        std::vector<VkDescriptorSet> descriptorSets;
#else
        std::vector<void*> descriptorSets;
#endif

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

        // Texture
        Texture* texture;

        // Shader program
        ShaderProgram* shaderProgram;
    };
}