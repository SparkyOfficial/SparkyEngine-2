#pragma once

#include <string>
#include <vector>

namespace Sparky {
    class ShaderCompiler {
    public:
        static std::vector<uint32_t> compileGLSLToSPIRV(const std::string& source, int shaderType);
        static bool compileShaderFile(const std::string& filepath, const std::string& outputPath, int shaderType);
        
        // Shader types
        static const int VERTEX_SHADER = 0;
        static const int FRAGMENT_SHADER = 1;
        static const int GEOMETRY_SHADER = 2;
        static const int COMPUTE_SHADER = 3;
    };
}