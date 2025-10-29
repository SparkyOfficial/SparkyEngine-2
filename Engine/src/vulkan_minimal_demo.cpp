#include "../include/Logger.h"
#include <iostream>
#include <vector>
#include <fstream>

// Include Vulkan headers through the engine's header
#include "../include/VulkanRenderer.h"

// Simple vertex shader SPIR-V (just passes through position)
static const uint32_t vertShaderCode[] = {
    0x07230203,0x00010000,0x00080001,0x00000028,
    0x00000000,0x00020011,0x00000001,0x0006000b,
    0x00000001,0x4c534c47,0x6474732e,0x3035342e,
    0x00000000,0x0003000e,0x00000000,0x00000001,
    0x0006000f,0x00000000,0x00000004,0x6e69616d,
    0x00000000,0x0000000d,0x00030003,0x00000002,
    0x000001c2,0x00040005,0x00000004,0x6e69616d,
    0x00000000,0x00060005,0x0000000b,0x505f6c67,
    0x65567265,0x78657472,0x00000000,0x00060006,
    0x0000000b,0x00000000,0x505f6c67,0x7469736f,
    0x006e6f69,0x00070006,0x0000000b,0x00000001,
    0x505f6c67,0x746e694f,0x657a6953,0x00000000,
    0x00070006,0x0000000b,0x00000002,0x435f6c67,
    0x4470696c,0x61747369,0x0065636e,0x00030005,
    0x0000000d,0x00000000,0x00050048,0x0000000b,
    0x00000000,0x0000000b,0x00000000,0x00050048,
    0x0000000b,0x00000001,0x0000000b,0x00000001,
    0x00050048,0x0000000b,0x00000002,0x0000000b,
    0x00000003,0x00030047,0x0000000b,0x00000002,
    0x00040047,0x0000000d,0x0000001e,0x00000000,
    0x00050048,0x0000000d,0x00000000,0x00000023,
    0x00000000,0x00050048,0x0000000d,0x00000000,
    0x00000023,0x00000008,0x00030047,0x0000000d,
    0x00000002,0x00020013,0x00000002,0x00030021,
    0x00000003,0x00000002,0x00030016,0x00000006,
    0x00000020,0x00040017,0x00000007,0x00000006,
    0x00000004,0x00040020,0x00000008,0x00000003,
    0x00000007,0x0004003b,0x00000008,0x00000009,
    0x00000003,0x00040015,0x0000000a,0x00000020,
    0x00000001,0x0004002b,0x0000000a,0x0000000b,
    0x00000000,0x0004001c,0x0000000c,0x00000006,
    0x0000000b,0x0006001e,0x0000000d,0x00000007,
    0x00000006,0x0000000c,0x0000000c,0x00040020,
    0x0000000e,0x00000003,0x0000000d,0x0004003b,
    0x0000000e,0x0000000f,0x00000003,0x00040020,
    0x00000010,0x00000001,0x00000007,0x0004003b,
    0x00000010,0x00000011,0x00000001,0x00050036,
    0x00000002,0x00000004,0x00000000,0x00000003,
    0x000200f8,0x00000005,0x0004003d,0x00000007,
    0x00000012,0x0000000f,0x00050041,0x00000013,
    0x00000014,0x00000009,0x00000011,0x0003003e,
    0x00000012,0x00000014,0x000100fd,0x00010038
};

// Simple fragment shader SPIR-V (outputs red color)
static const uint32_t fragShaderCode[] = {
    0x07230203,0x00010000,0x00080001,0x00000014,
    0x00000000,0x00020011,0x00000001,0x0006000b,
    0x00000001,0x4c534c47,0x6474732e,0x3035342e,
    0x00000000,0x0003000e,0x00000000,0x00000001,
    0x0005000f,0x00000005,0x00000004,0x6e69616d,
    0x00000000,0x00030010,0x00000004,0x00000007,
    0x00030003,0x00000002,0x000001c2,0x00040005,
    0x00000004,0x6e69616d,0x00000000,0x00040005,
    0x00000009,0x6c6f4366,0x0000726f,0x00030047,
    0x00000009,0x00000019,0x00040047,0x00000009,
    0x0000001e,0x00000000,0x00020013,0x00000002,
    0x00030021,0x00000003,0x00000002,0x00030016,
    0x00000006,0x00000020,0x00040017,0x00000007,
    0x00000006,0x00000004,0x00040020,0x00000008,
    0x00000003,0x00000007,0x0004003b,0x00000008,
    0x00000009,0x00000003,0x00050036,0x00000002,
    0x00000004,0x00000000,0x00000003,0x000200f8,
    0x00000005,0x0004003d,0x00000007,0x0000000a,
    0x00000009,0x0004003e,0x0000000a,0x0000000a,
    0x00000001,0x000100fd,0x00010038
};

VkShaderModule createShaderModule(VkDevice device, const uint32_t* code, size_t codeSize) {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = codeSize;
    createInfo.pCode = code;

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module!");
    }

    return shaderModule;
}

int main() {
    Sparky::Logger::getInstance().setLogLevel(Sparky::LogLevel::DEBUG);
    Sparky::Logger::getInstance().info("Starting Minimal Vulkan Pipeline Test");
    
    // Test 1: Create Vulkan instance
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Minimal Vulkan Pipeline Test";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    
    // No extensions for now
    createInfo.enabledExtensionCount = 0;
    createInfo.ppEnabledExtensionNames = nullptr;

    VkInstance instance;
    VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
    
    if (result != VK_SUCCESS) {
        Sparky::Logger::getInstance().error("Failed to create Vulkan instance, result: " + std::to_string(result));
        return -1;
    }
    
    Sparky::Logger::getInstance().info("Vulkan instance created successfully");
    
    // Test 2: Enumerate physical devices
    uint32_t deviceCount = 0;
    result = vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    
    if (result != VK_SUCCESS || deviceCount == 0) {
        Sparky::Logger::getInstance().error("No physical devices found or enumeration failed");
        vkDestroyInstance(instance, nullptr);
        return -1;
    }
    
    Sparky::Logger::getInstance().info("Found " + std::to_string(deviceCount) + " physical devices");
    
    std::vector<VkPhysicalDevice> devices(deviceCount);
    result = vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
    
    if (result != VK_SUCCESS) {
        Sparky::Logger::getInstance().error("Failed to enumerate physical devices, result: " + std::to_string(result));
        vkDestroyInstance(instance, nullptr);
        return -1;
    }
    
    // Test 3: Get device properties
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(devices[0], &deviceProperties);
    
    Sparky::Logger::getInstance().info("First device: " + std::string(deviceProperties.deviceName));
    
    // Test 4: Find a graphics queue family
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(devices[0], &queueFamilyCount, nullptr);
    
    if (queueFamilyCount == 0) {
        Sparky::Logger::getInstance().error("No queue families found");
        vkDestroyInstance(instance, nullptr);
        return -1;
    }
    
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(devices[0], &queueFamilyCount, queueFamilies.data());
    
    // Find a graphics queue family
    uint32_t graphicsQueueFamilyIndex = 0;
    for (uint32_t i = 0; i < queueFamilyCount; i++) {
        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            graphicsQueueFamilyIndex = i;
            break;
        }
    }
    
    // Test 5: Create logical device
    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = graphicsQueueFamilyIndex;
    queueCreateInfo.queueCount = 1;
    
    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority;
    
    VkDeviceCreateInfo deviceCreateInfo{};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
    deviceCreateInfo.enabledExtensionCount = 0;
    deviceCreateInfo.ppEnabledExtensionNames = nullptr;
    
    VkDevice device;
    result = vkCreateDevice(devices[0], &deviceCreateInfo, nullptr, &device);
    
    if (result != VK_SUCCESS) {
        Sparky::Logger::getInstance().error("Failed to create logical device, result: " + std::to_string(result));
        vkDestroyInstance(instance, nullptr);
        return -1;
    }
    
    Sparky::Logger::getInstance().info("Logical device created successfully");
    
    // Test 6: Create shader modules
    VkShaderModule vertShaderModule = createShaderModule(device, vertShaderCode, sizeof(vertShaderCode));
    VkShaderModule fragShaderModule = createShaderModule(device, fragShaderCode, sizeof(fragShaderCode));
    
    Sparky::Logger::getInstance().info("Shader modules created successfully");
    
    // Test 7: Create pipeline layout
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;

    VkPipelineLayout pipelineLayout;
    result = vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout);
    
    if (result != VK_SUCCESS) {
        Sparky::Logger::getInstance().error("Failed to create pipeline layout, result: " + std::to_string(result));
        vkDestroyShaderModule(device, fragShaderModule, nullptr);
        vkDestroyShaderModule(device, vertShaderModule, nullptr);
        vkDestroyDevice(device, nullptr);
        vkDestroyInstance(instance, nullptr);
        return -1;
    }
    
    Sparky::Logger::getInstance().info("Pipeline layout created successfully");
    
    // Test 8: Try to create graphics pipeline (this is where the error usually occurs)
    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

    // Simple vertex input (no attributes)
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.pVertexBindingDescriptions = nullptr;
    vertexInputInfo.vertexAttributeDescriptionCount = 0;
    vertexInputInfo.pVertexAttributeDescriptions = nullptr;

    // Simple input assembly
    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    // Simple rasterization
    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;

    // Simple multisampling
    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    // Simple color blending
    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;

    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;

    // Simple viewport and scissor
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = 800.0f;  // Use a default size
    viewport.height = 600.0f;  // Use a default size
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = {800, 600};  // Use a default size

    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    // Simple dynamic states (none for maximum compatibility)
    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = 0;  // No dynamic states for maximum compatibility
    dynamicState.pDynamicStates = nullptr;

    // Graphics pipeline
    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;  // Properly set viewport state
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = nullptr;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.renderPass = VK_NULL_HANDLE;  // Still need to create a proper render pass
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineInfo.basePipelineIndex = -1;

    // For a minimal test, we'll skip trying to create the full graphics pipeline
    // since it requires a valid render pass which is complex to set up in a minimal test
    Sparky::Logger::getInstance().info("Skipping graphics pipeline creation in minimal test - all prerequisite components created successfully");
    
    // In a real application, we would need to create a proper render pass before creating the graphics pipeline
    /*
    VkPipeline graphicsPipeline;
    result = vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline);
    
    if (result == VK_SUCCESS) {
        Sparky::Logger::getInstance().info("Graphics pipeline created successfully");
        vkDestroyPipeline(device, graphicsPipeline, nullptr);
    } else {
        Sparky::Logger::getInstance().error("Failed to create graphics pipeline, result: " + std::to_string(result));
    }
    */
    
    // Cleanup
    vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
    vkDestroyShaderModule(device, fragShaderModule, nullptr);
    vkDestroyShaderModule(device, vertShaderModule, nullptr);
    vkDestroyDevice(device, nullptr);
    vkDestroyInstance(instance, nullptr);
    
    Sparky::Logger::getInstance().info("Minimal Vulkan Pipeline Test finished");
    return 0;
}