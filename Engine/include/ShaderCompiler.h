#pragma once

#include <vector>
#include <string>

namespace Sparky {
    class ShaderCompiler {
    public:
        /**
         * @brief Compiles GLSL source code to SPIR-V bytecode
         * @param source The GLSL source code as a string
         * @param shaderType The type of shader (0=vertex, 1=fragment, etc.)
         * @return Vector of uint32_t containing the SPIR-V bytecode
         */
        static std::vector<uint32_t> compileGLSLToSPIRV(const std::string& source, int shaderType);
        
        /**
         * @brief Compiles a shader file to SPIR-V and saves it to an output file
         * @param filepath Path to the GLSL shader file
         * @param outputPath Path where the compiled SPIR-V file should be saved
         * @param shaderType The type of shader (0=vertex, 1=fragment, etc.)
         * @return True if compilation was successful, false otherwise
         */
        static bool compileShaderFile(const std::string& filepath, const std::string& outputPath, int shaderType = -1);
    };
}