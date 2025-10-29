#ifdef HAS_VULKAN
#include <vulkan/vulkan.h>
#endif

#ifdef HAS_GLFW
#include <GLFW/glfw3.h>
#endif

#include <iostream>
#include <vector>
#include <cstring>

int main() {
    std::cout << "Starting Vulkan extension test..." << std::endl;
    
#ifdef HAS_VULKAN
    std::cout << "Vulkan is available" << std::endl;
    
    // Check if Vulkan is available
    uint32_t instanceVersion;
    VkResult result = vkEnumerateInstanceVersion(&instanceVersion);
    if (result == VK_SUCCESS) {
        std::cout << "Vulkan instance version: " << VK_VERSION_MAJOR(instanceVersion) << "."
                  << VK_VERSION_MINOR(instanceVersion) << "." << VK_VERSION_PATCH(instanceVersion) << std::endl;
    } else {
        std::cout << "Failed to get Vulkan instance version: " << result << std::endl;
        return 1;
    }

    // List available instance extensions
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

    std::cout << "Available instance extensions:" << std::endl;
    for (const auto& extension : extensions) {
        std::cout << "  " << extension.extensionName << " (version " << extension.specVersion << ")" << std::endl;
    }

    // Try to create instance with different API versions
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Vulkan Extension Test";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Test Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    
    // Try with different API versions
    std::vector<uint32_t> apiVersions = {
        VK_API_VERSION_1_0,
        VK_API_VERSION_1_1,
        VK_API_VERSION_1_2,
        VK_API_VERSION_1_3
    };
    
    for (uint32_t apiVersion : apiVersions) {
        appInfo.apiVersion = apiVersion;
        
        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;
        
        // Add required extensions
        const std::vector<const char*> requiredExtensions = {
            VK_KHR_SURFACE_EXTENSION_NAME,
#ifdef _WIN32
            VK_KHR_WIN32_SURFACE_EXTENSION_NAME
#endif
        };
        
        createInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
        createInfo.ppEnabledExtensionNames = requiredExtensions.data();
        
        VkInstance instance;
        VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
        
        std::cout << "API Version " << VK_VERSION_MAJOR(apiVersion) << "." 
                  << VK_VERSION_MINOR(apiVersion) << "." << VK_VERSION_PATCH(apiVersion) 
                  << ": " << (result == VK_SUCCESS ? "SUCCESS" : "FAILED") << " (" << result << ")" << std::endl;
        
        if (result == VK_SUCCESS) {
            vkDestroyInstance(instance, nullptr);
        }
    }
#else
    std::cout << "Vulkan not available" << std::endl;
#endif
    
    std::cout << "Vulkan extension test completed" << std::endl;
    return 0;
}