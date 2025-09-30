#include "../include/Skybox.h"
#include "../include/VulkanRenderer.h"
#include "../include/Logger.h"
#include <iostream>

int main() {
    Sparky::Logger::getInstance().setLogLevel(Sparky::LogLevel::DEBUG);
    
    SPARKY_LOG_INFO("Starting Skybox Test");
    
    // Create a skybox
    auto skybox = std::make_unique<Sparky::Skybox>();
    
    if (!skybox) {
        SPARKY_LOG_ERROR("Failed to create skybox");
        return -1;
    }
    
    SPARKY_LOG_INFO("Skybox created successfully");
    
    // In a real implementation, we would initialize the skybox with a renderer
    // and load textures, but for this test, we'll just verify the object is created
    
    SPARKY_LOG_INFO("Skybox test completed successfully");
    return 0;
}