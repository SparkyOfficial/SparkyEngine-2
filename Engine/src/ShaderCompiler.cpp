#include "../include/ShaderCompiler.h"
#include "../include/FileUtils.h"
#include "../include/Logger.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>

// Include glslang headers for actual shader compilation
// Only include if the headers are available
#ifdef HAS_GLSLANG
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4100) // unreferenced formal parameter
#pragma warning(disable : 4189) // local variable is initialized but not referenced
#pragma warning(disable : 4127) // conditional expression is constant
#pragma warning(disable : 4324) // structure was padded due to alignment specifier
#endif

#include <glslang/Public/ShaderLang.h>
#include <glslang/SPIRV/GlslangToSpv.h>
// Try alternative path for ResourceLimits.h
#include <glslang/Public/ResourceLimits.h>

#ifdef _MSC_VER
#pragma warning(pop)
#endif
#endif // HAS_GLSLANG

namespace {
#ifdef HAS_GLSLANG
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
#endif
}

namespace Sparky {

    std::vector<uint32_t> ShaderCompiler::compileGLSLToSPIRV(const std::string& source, int shaderType) {
#ifdef HAS_GLSLANG
        SPARKY_LOG_DEBUG("Compiling GLSL shader to SPIR-V");
        SPARKY_LOG_DEBUG("Shader source length: " + std::to_string(source.length()));
        SPARKY_LOG_DEBUG("First 200 chars of shader: " + source.substr(0, std::min(200, (int)source.length())));
        
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
        // Set more conservative limits
        resources.maxVertexAttribs = 16;
        resources.maxVertexUniformComponents = 1024;
        resources.maxVaryingFloats = 16;
        resources.maxVertexTextureImageUnits = 16;
        resources.maxCombinedTextureImageUnits = 16;
        resources.maxTextureImageUnits = 16;
        resources.maxFragmentUniformComponents = 1024;
        resources.maxDrawBuffers = 8;
        EShMessages messages = static_cast<EShMessages>(EShMsgSpvRules | EShMsgVulkanRules);
        
        // Preprocess
        const int defaultVersion = 450;
        std::string preprocessedGLSL;
        glslang::TShader::ForbidIncluder includer;  // Create a simple includer
        if (!shader.preprocess(&resources, defaultVersion, ECoreProfile, false, false, messages, &preprocessedGLSL, includer)) {
            SPARKY_LOG_ERROR("GLSL preprocessing failed: " + std::string(shader.getInfoLog()));
            throw std::runtime_error("GLSL preprocessing failed: " + std::string(shader.getInfoLog()));
        }
        
        const char* preprocessedCStr = preprocessedGLSL.c_str();
        shader.setStrings(&preprocessedCStr, 1);
        
        // Parse with more lenient settings
        if (!shader.parse(&resources, 450, false, messages)) {
            SPARKY_LOG_ERROR("GLSL parsing failed: " + std::string(shader.getInfoLog()));
            throw std::runtime_error("GLSL parsing failed: " + std::string(shader.getInfoLog()));
        }
        
        // Add to program
        program.addShader(&shader);
        
        // Link with more lenient settings
        if (!program.link(messages)) {
            SPARKY_LOG_ERROR("GLSL linking failed: " + std::string(program.getInfoLog()));
            throw std::runtime_error("GLSL linking failed: " + std::string(program.getInfoLog()));
        }
        
        // Map IO with more lenient settings
        if (!program.mapIO()) {
            SPARKY_LOG_ERROR("GLSL mapIO failed: " + std::string(program.getInfoLog()));
            throw std::runtime_error("GLSL mapIO failed: " + std::string(program.getInfoLog()));
        }
        
        // Generate SPIR-V
        std::vector<uint32_t> spirv;
        glslang::SpvOptions spvOptions;
        spvOptions.generateDebugInfo = false;
        spvOptions.stripDebugInfo = true;  // Strip debug info to reduce complexity
        spvOptions.disableOptimizer = false;
        spvOptions.optimizeSize = true;
        glslang::GlslangToSpv(*program.getIntermediate(stage), spirv, &spvOptions);
        
        SPARKY_LOG_DEBUG("Shader compiled to SPIR-V with " + std::to_string(spirv.size()) + " words");
        return spirv;
#else
        // Return empty vector if glslang is not available
        SPARKY_LOG_WARNING("glslang not available, returning empty SPIR-V");
        return std::vector<uint32_t>();
#endif
    }

    std::vector<uint32_t> ShaderCompiler::loadSPIRVFromFile(const std::string& filepath) {
        SPARKY_LOG_INFO("Loading SPIR-V from file: " + filepath);
        
        std::ifstream file(filepath, std::ios::ate | std::ios::binary);
        
        if (!file.is_open()) {
            SPARKY_LOG_ERROR("Failed to open SPIR-V file: " + filepath);
            throw std::runtime_error("Failed to open SPIR-V file: " + filepath);
        }
        
        size_t fileSize = (size_t)file.tellg();
        std::vector<char> buffer(fileSize);
        
        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();
        
        // Convert char vector to uint32_t vector
        std::vector<uint32_t> spirv(buffer.size() / sizeof(uint32_t));
        memcpy(spirv.data(), buffer.data(), buffer.size());
        
        SPARKY_LOG_INFO("SPIR-V loaded successfully, size: " + std::to_string(spirv.size()) + " words");
        return spirv;
    }

    std::vector<uint32_t> ShaderCompiler::compileGLSLToSPIRVWithFallback(const std::string& source, const std::string& spvFallbackPath, int shaderType) {
        try {
            // Try to compile with glslang first
            return compileGLSLToSPIRV(source, shaderType);
        } catch (const std::exception& e) {
            SPARKY_LOG_WARNING("GLSL compilation failed, trying SPIR-V fallback: " + std::string(e.what()));
            
            // If compilation fails, try to load pre-compiled SPIR-V
            try {
                return loadSPIRVFromFile(spvFallbackPath);
            } catch (const std::exception& fallbackError) {
                SPARKY_LOG_ERROR("SPIR-V fallback also failed: " + std::string(fallbackError.what()));
                // Re-throw the original error
                throw;
            }
        }
    }

    bool ShaderCompiler::compileShaderFile(const std::string& filepath, const std::string& outputPath, int shaderType) {
        SPARKY_LOG_INFO("Compiling shader file: " + filepath);
        
        try {
            // Read the GLSL source
            std::vector<char> source = FileUtils::readFile(filepath);
            std::string sourceStr(source.begin(), source.end());
            
#ifdef HAS_GLSLANG
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
#else
            // If glslang is not available, just copy the file
            std::ofstream outFile(outputPath, std::ios::binary);
            if (!outFile.is_open()) {
                SPARKY_LOG_ERROR("Failed to open output file: " + outputPath);
                return false;
            }
            
            outFile.write(source.data(), source.size());
            outFile.close();
            
            SPARKY_LOG_INFO("Shader copied successfully (glslang not available): " + outputPath);
            return true;
#endif
        } catch (const std::exception& e) {
            SPARKY_LOG_ERROR("Failed to compile shader: " + std::string(e.what()));
            return false;
        }
    }
}