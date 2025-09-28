#include "../include/ShaderProgram.h"
#include "../include/FileUtils.h"
#include "../include/Logger.h"
#include <GL/glew.h>
#include <fstream>
#include <sstream>

namespace Sparky {

    ShaderProgram::ShaderProgram() : programId(0) {
    }

    ShaderProgram::~ShaderProgram() {
        if (programId != 0) {
            glDeleteProgram(programId);
        }
    }

    bool ShaderProgram::loadShaders(const std::string& vertexFile, const std::string& fragmentFile) {
        SPARKY_LOG_DEBUG("Loading shaders: " + vertexFile + ", " + fragmentFile);
        
        try {
            // Read vertex shader source
            std::vector<char> vertexSourceVec = FileUtils::readFile(vertexFile);
            std::string vertexSource(vertexSourceVec.begin(), vertexSourceVec.end());
            
            // Read fragment shader source
            std::vector<char> fragmentSourceVec = FileUtils::readFile(fragmentFile);
            std::string fragmentSource(fragmentSourceVec.begin(), fragmentSourceVec.end());
            
            return loadShadersFromSource(vertexSource, fragmentSource);
        } catch (const std::exception& e) {
            SPARKY_LOG_ERROR("Failed to load shaders: " + std::string(e.what()));
            return false;
        }
    }

    bool ShaderProgram::loadShadersFromSource(const std::string& vertexSource, const std::string& fragmentSource) {
        SPARKY_LOG_DEBUG("Compiling shaders from source");
        
        // Compile vertex shader
        unsigned int vertexShader;
        if (!compileShader(vertexShader, GL_VERTEX_SHADER, vertexSource)) {
            return false;
        }
        
        // Compile fragment shader
        unsigned int fragmentShader;
        if (!compileShader(fragmentShader, GL_FRAGMENT_SHADER, fragmentSource)) {
            glDeleteShader(vertexShader);
            return false;
        }
        
        // Create shader program
        programId = glCreateProgram();
        
        // Attach shaders
        glAttachShader(programId, vertexShader);
        glAttachShader(programId, fragmentShader);
        
        // Link program
        glLinkProgram(programId);
        
        // Check for linking errors
        int success;
        char infoLog[512];
        glGetProgramiv(programId, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(programId, 512, NULL, infoLog);
            SPARKY_LOG_ERROR("Shader program linking failed: " + std::string(infoLog));
            glDeleteProgram(programId);
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
            programId = 0;
            return false;
        }
        
        // Delete shader objects as they're linked into our program now
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        
        SPARKY_LOG_DEBUG("Shaders compiled and linked successfully");
        return true;
    }

    void ShaderProgram::use() const {
        if (programId != 0) {
            glUseProgram(programId);
        }
    }

    bool ShaderProgram::compileShader(unsigned int& shader, int type, const std::string& source) {
        shader = glCreateShader(type);
        const char* sourceCStr = source.c_str();
        glShaderSource(shader, 1, &sourceCStr, NULL);
        glCompileShader(shader);
        
        // Check for compilation errors
        int success;
        char infoLog[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::string shaderType = (type == GL_VERTEX_SHADER) ? "vertex" : "fragment";
            SPARKY_LOG_ERROR("Shader compilation failed (" + shaderType + "): " + std::string(infoLog));
            glDeleteShader(shader);
            return false;
        }
        
        return true;
    }

    int ShaderProgram::getUniformLocation(const std::string& name) const {
        // Check if we've already cached this uniform location
        auto it = uniformLocations.find(name);
        if (it != uniformLocations.end()) {
            return it->second;
        }
        
        // Get and cache the uniform location
        int location = glGetUniformLocation(programId, name.c_str());
        uniformLocations[name] = location;
        return location;
    }

    void ShaderProgram::setBool(const std::string& name, bool value) const {
        int location = getUniformLocation(name);
        if (location != -1) {
            glUniform1i(location, (int)value);
        }
    }

    void ShaderProgram::setInt(const std::string& name, int value) const {
        int location = getUniformLocation(name);
        if (location != -1) {
            glUniform1i(location, value);
        }
    }

    void ShaderProgram::setFloat(const std::string& name, float value) const {
        int location = getUniformLocation(name);
        if (location != -1) {
            glUniform1f(location, value);
        }
    }

    void ShaderProgram::setVec2(const std::string& name, const glm::vec2& value) const {
        int location = getUniformLocation(name);
        if (location != -1) {
            glUniform2fv(location, 1, glm::value_ptr(value));
        }
    }

    void ShaderProgram::setVec3(const std::string& name, const glm::vec3& value) const {
        int location = getUniformLocation(name);
        if (location != -1) {
            glUniform3fv(location, 1, glm::value_ptr(value));
        }
    }

    void ShaderProgram::setVec4(const std::string& name, const glm::vec4& value) const {
        int location = getUniformLocation(name);
        if (location != -1) {
            glUniform4fv(location, 1, glm::value_ptr(value));
        }
    }

    void ShaderProgram::setMat2(const std::string& name, const glm::mat2& mat) const {
        int location = getUniformLocation(name);
        if (location != -1) {
            glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(mat));
        }
    }

    void ShaderProgram::setMat3(const std::string& name, const glm::mat3& mat) const {
        int location = getUniformLocation(name);
        if (location != -1) {
            glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(mat));
        }
    }

    void ShaderProgram::setMat4(const std::string& name, const glm::mat4& mat) const {
        int location = getUniformLocation(name);
        if (location != -1) {
            glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
        }
    }
}