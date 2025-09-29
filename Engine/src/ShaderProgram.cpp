#include "../include/ShaderProgram.h"
#include "../include/ShaderCompiler.h"
#include "../include/FileUtils.h"
#include "../include/Logger.h"
#include <vector>
#include <stdexcept>

namespace Sparky {

    ShaderProgram::ShaderProgram() : vertexShaderModule(VK_NULL_HANDLE), fragmentShaderModule(VK_NULL_HANDLE) {
        SPARKY_LOG_DEBUG("ShaderProgram created");
    }

    ShaderProgram::~ShaderProgram() {
        SPARKY_LOG_DEBUG("ShaderProgram destroyed");
    }

    bool ShaderProgram::loadShaders(const std::string& vertexPath, const std::string& fragmentPath) {
        try {
            // Read vertex shader source
            std::vector<char> vertSource = readFile(vertexPath);
            std::string vertSourceStr(vertSource.begin(), vertSource.end());
            std::vector<uint32_t> vertShaderCode = ShaderCompiler::compileGLSLToSPIRV(vertSourceStr, 0); // 0 = vertex shader
            vertexShaderModule = createShaderModule(vertShaderCode);
            
            // Read fragment shader source
            std::vector<char> fragSource = readFile(fragmentPath);
            std::string fragSourceStr(fragSource.begin(), fragSource.end());
            std::vector<uint32_t> fragShaderCode = ShaderCompiler::compileGLSLToSPIRV(fragSourceStr, 1); // 1 = fragment shader
            fragmentShaderModule = createShaderModule(fragShaderCode);
            
            return true;
        } catch (const std::exception& e) {
            SPARKY_LOG_ERROR("Failed to load shaders: " + std::string(e.what()));
            return false;
        }
    }

    void ShaderProgram::use() {
        // Bind the shader program for use in rendering
        SPARKY_LOG_DEBUG("Using shader program");
    }

    void ShaderProgram::setMat4(const std::string& name, const glm::mat4& value) {
        // Set the uniform matrix in the shader
        SPARKY_LOG_DEBUG("Setting mat4 uniform: " + name);
    }

    void ShaderProgram::setVec3(const std::string& name, const glm::vec3& value) {
        // Set the uniform vector in the shader
        SPARKY_LOG_DEBUG("Setting vec3 uniform: " + name);
    }

    void ShaderProgram::setFloat(const std::string& name, float value) {
        // Set the uniform float in the shader
        SPARKY_LOG_DEBUG("Setting float uniform: " + name + " = " + std::to_string(value));
    }

    void ShaderProgram::setInt(const std::string& name, int value) {
        // Set the uniform int in the shader
        SPARKY_LOG_DEBUG("Setting int uniform: " + name + " = " + std::to_string(value));
    }

    VkShaderModule ShaderProgram::createShaderModule(const std::vector<uint32_t>& code) {
        // Create a Vulkan shader module from SPIR-V bytecode
        SPARKY_LOG_DEBUG("Creating shader module");
        return VK_NULL_HANDLE;
    }

    std::vector<char> ShaderProgram::readFile(const std::string& filename) {
        // Read the shader file from disk
        SPARKY_LOG_DEBUG("Reading shader file: " + filename);
        return FileUtils::readFile(filename);
    }

}