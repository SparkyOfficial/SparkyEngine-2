#include <vulkan/vulkan.h>
#include <iostream>
#include <vector>
#include <cstring>

int main() {
    // Initialize Vulkan instance
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Vulkan Simple Test";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    VkInstance instance;
    VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
    
    if (result != VK_SUCCESS) {
        std::cerr << "Failed to create Vulkan instance! Error code: " << result << std::endl;
        return -1;
    }

    std::cout << "Vulkan instance created successfully!" << std::endl;

    // Enumerate physical devices
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        std::cerr << "No Vulkan-compatible GPUs found!" << std::endl;
        vkDestroyInstance(instance, nullptr);
        return -1;
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    std::cout << "Found " << deviceCount << " Vulkan-compatible GPU(s):" << std::endl;

    for (const auto& device : devices) {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(device, &properties);
        std::cout << "  - " << properties.deviceName << std::endl;
    }

    // Cleanup
    vkDestroyInstance(instance, nullptr);
    
    std::cout << "Vulkan simple test completed successfully!" << std::endl;
    return 0;
}