#pragma once

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include <vulkan/vulkan.h>

namespace Sparky {

    class ShaderProgram {
    private:
        VkShaderModule vertexShaderModule;
        VkShaderModule fragmentShaderModule;
        std::unordered_map<std::string, int> uniformLocations;

    public:
        ShaderProgram();
        ~ShaderProgram();

        bool loadShaders(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
        void use();
        void setMat4(const std::string& name, const glm::mat4& value);
        void setVec3(const std::string& name, const glm::vec3& value);
        void setFloat(const std::string& name, float value);
        void setInt(const std::string& name, int value);

        VkShaderModule getVertexShaderModule() const { return vertexShaderModule; }
        VkShaderModule getFragmentShaderModule() const { return fragmentShaderModule; }

    private:
        VkShaderModule createShaderModule(const std::vector<char>& code);
        std::vector<char> readFile(const std::string& filename);
    };

}