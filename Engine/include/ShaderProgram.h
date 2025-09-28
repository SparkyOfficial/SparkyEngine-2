#pragma once

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Sparky {
    class ShaderProgram {
    public:
        ShaderProgram();
        ~ShaderProgram();

        bool loadShaders(const std::string& vertexFile, const std::string& fragmentFile);
        bool loadShadersFromSource(const std::string& vertexSource, const std::string& fragmentSource);
        void use() const;

        // Uniform setters
        void setBool(const std::string& name, bool value) const;
        void setInt(const std::string& name, int value) const;
        void setFloat(const std::string& name, float value) const;
        void setVec2(const std::string& name, const glm::vec2& value) const;
        void setVec3(const std::string& name, const glm::vec3& value) const;
        void setVec4(const std::string& name, const glm::vec4& value) const;
        void setMat2(const std::string& name, const glm::mat2& mat) const;
        void setMat3(const std::string& name, const glm::mat3& mat) const;
        void setMat4(const std::string& name, const glm::mat4& mat) const;

        unsigned int getProgramId() const { return programId; }

    private:
        unsigned int programId;
        mutable std::unordered_map<std::string, int> uniformLocations;

        bool compileShader(unsigned int& shader, int type, const std::string& source);
        int getUniformLocation(const std::string& name) const;
    };
}