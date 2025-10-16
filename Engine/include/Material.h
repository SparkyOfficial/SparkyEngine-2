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
        
        // Advanced PBR material properties
        void setRoughness(float roughness);
        void setMetalness(float metalness);
        void setNormalMap(Texture* normalMap);
        void setRoughnessMap(Texture* roughnessMap);
        void setMetalnessMap(Texture* metalnessMap);
        
        // IBL properties
        void setUseIBL(bool useIBL);
        bool getUseIBL() const { return useIBL; }

        const glm::vec3& getAmbient() const { return ambient; }
        const glm::vec3& getDiffuse() const { return diffuse; }
        const glm::vec3& getSpecular() const { return specular; }
        float getShininess() const { return shininess; }
        const glm::vec3& getEmissive() const { return emissive; }
        float getRoughness() const { return roughness; }
        float getMetalness() const { return metalness; }
        Texture* getNormalMap() const { return normalMap; }
        Texture* getRoughnessMap() const { return roughnessMap; }
        Texture* getMetalnessMap() const { return metalnessMap; }

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
        
        // Advanced PBR material properties
        float roughness;
        float metalness;
        Texture* normalMap;
        Texture* roughnessMap;
        Texture* metalnessMap;
        bool useIBL;

        // Texture
        Texture* texture;

        // Shader program
        ShaderProgram* shaderProgram;
    };
}