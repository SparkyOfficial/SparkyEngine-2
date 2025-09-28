#include "../include/ShaderCompiler.h"
#include "../include/FileUtils.h"
#include "../include/Logger.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>

// Include glslang headers for actual shader compilation
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4100) // unreferenced formal parameter
#pragma warning(disable : 4189) // local variable is initialized but not referenced
#pragma warning(disable : 4127) // conditional expression is constant
#pragma warning(disable : 4324) // structure was padded due to alignment specifier
#endif

#include <glslang/Public/ShaderLang.h>
#include <glslang/SPIRV/GlslangToSpv.h>
#include <glslang/StandAlone/ResourceLimits.h>

#ifdef _MSC_VER
#pragma warning(pop)
#endif

namespace {
    // Helper function to map shader type to EShLanguage
    EShLanguage findLanguage(const std::string& filename) {
        if (filename.find(".vert") != std::string::npos) {
            return EShLangVertex;
        } else if (filename.find(".frag") != std::string::npos) {
            return EShLangFragment;
        } else if (filename.find(".geom") != std::string::npos) {
            return EShLangGeometry;
        } else if (filename.find(".comp") != std::string::npos) {
            return EShLangCompute;
        } else if (filename.find(".tesc") != std::string::npos) {
            return EShLangTessControl;
        } else if (filename.find(".tese") != std::string::npos) {
            return EShLangTessEvaluation;
        }
        return EShLangVertex;
    }

    // Initialize glslang if not already initialized
    void initializeGlslang() {
        static bool initialized = false;
        if (!initialized) {
            glslang::InitializeProcess();
            initialized = true;
        }
    }
}

namespace Sparky {

    std::vector<uint32_t> ShaderCompiler::compileGLSLToSPIRV(const std::string& source, int shaderType) {
        SPARKY_LOG_DEBUG("Compiling GLSL shader to SPIR-V");
        
        initializeGlslang();
        
        EShLanguage stage = static_cast<EShLanguage>(shaderType);
        glslang::TShader shader(stage);
        glslang::TProgram program;
        const char* shaderStrings[1];
        shaderStrings[0] = source.c_str();
        int shaderLengths[1];
        shaderLengths[0] = static_cast<int>(source.length());
        
        shader.setStringsWithLengths(shaderStrings, shaderLengths, 1);
        shader.setEntryPoint("main");
        shader.setSourceEntryPoint("main");
        
        TBuiltInResource resources = *GetDefaultResources();
        EShMessages messages = static_cast<EShMessages>(EShMsgSpvRules | EShMsgVulkanRules);
        
        // Preprocess
        const int defaultVersion = 100;
        std::string preprocessedGLSL;
        if (!shader.preprocess(&resources, defaultVersion, ENoProfile, false, false, messages, &preprocessedGLSL, nullptr)) {
            SPARKY_LOG_ERROR("GLSL preprocessing failed: " + std::string(shader.getInfoLog()));
            throw std::runtime_error("GLSL preprocessing failed");
        }
        
        const char* preprocessedCStr = preprocessedGLSL.c_str();
        shader.setStrings(&preprocessedCStr, 1);
        
        // Parse
        if (!shader.parse(&resources, 100, false, messages)) {
            SPARKY_LOG_ERROR("GLSL parsing failed: " + std::string(shader.getInfoLog()));
            throw std::runtime_error("GLSL parsing failed");
        }
        
        // Add to program
        program.addShader(&shader);
        
        // Link
        if (!program.link(messages)) {
            SPARKY_LOG_ERROR("GLSL linking failed: " + std::string(program.getInfoLog()));
            throw std::runtime_error("GLSL linking failed");
        }
        
        // Map IO
        if (!program.mapIO()) {
            SPARKY_LOG_ERROR("GLSL mapIO failed: " + std::string(program.getInfoLog()));
            throw std::runtime_error("GLSL mapIO failed");
        }
        
        // Generate SPIR-V
        std::vector<uint32_t> spirv;
        glslang::SpvOptions spvOptions;
        spvOptions.generateDebugInfo = false;
        spvOptions.stripDebugInfo = false;
        spvOptions.disableOptimizer = false;
        spvOptions.optimizeSize = true;
        glslang::GlslangToSpv(*program.getIntermediate(stage), spirv, &spvOptions);
        
        SPARKY_LOG_DEBUG("Shader compiled to SPIR-V with " + std::to_string(spirv.size()) + " words");
        return spirv;
    }

    bool ShaderCompiler::compileShaderFile(const std::string& filepath, const std::string& outputPath, int shaderType) {
        SPARKY_LOG_INFO("Compiling shader file: " + filepath);
        
        try {
            // Read the GLSL source
            std::vector<char> source = FileUtils::readFile(filepath);
            std::string sourceStr(source.begin(), source.end());
            
            // Determine shader type from file extension if not provided
            EShLanguage stage = EShLangVertex;
            if (shaderType == -1) {
                stage = findLanguage(filepath);
            } else {
                stage = static_cast<EShLanguage>(shaderType);
            }
            
            // Compile to SPIR-V
            std::vector<uint32_t> spirv = compileGLSLToSPIRV(sourceStr, static_cast<int>(stage));
            
            // Write SPIR-V to output file
            std::ofstream outFile(outputPath, std::ios::binary);
            if (!outFile.is_open()) {
                SPARKY_LOG_ERROR("Failed to open output file: " + outputPath);
                return false;
            }
            
            outFile.write(reinterpret_cast<const char*>(spirv.data()), spirv.size() * sizeof(uint32_t));
            outFile.close();
            
            SPARKY_LOG_INFO("Shader compiled successfully: " + outputPath);
            return true;
        } catch (const std::exception& e) {
            SPARKY_LOG_ERROR("Failed to compile shader: " + std::string(e.what()));
            return false;
        }
    }
}