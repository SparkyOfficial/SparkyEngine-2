#pragma once

#include <vulkan/vulkan.h>
#include <string>
#include <vector>

namespace Sparky {
    class ShaderUtils {
    public:
        static std::vector<char> readFile(const std::string& filename);
        static VkShaderModule createShaderModule(VkDevice device, const std::vector<char>& code);
    };
}