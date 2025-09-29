#pragma once

#include <string>
#include <vector>
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

namespace Sparky {

    class ShaderProgram {
    public:
        ShaderProgram();
        ~ShaderProgram();

        bool loadShaders(const std::string& vertexPath, const std::string& fragmentPath);
        void use();
        
        // Uniform setters
        void setMat4(const std::string& name, const glm::mat4& value);
        void setVec3(const std::string& name, const glm::vec3& value);
        void setFloat(const std::string& name, float value);
        void setInt(const std::string& name, int value);

    private:
        VkShaderModule vertexShaderModule;
        VkShaderModule fragmentShaderModule;
        
        std::vector<char> readFile(const std::string& filename);
        VkShaderModule createShaderModule(const std::vector<uint32_t>& code);
    };

}