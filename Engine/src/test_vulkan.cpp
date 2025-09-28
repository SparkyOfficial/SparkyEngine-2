#include "../include/VulkanRenderer.h"
#include "../include/WindowManager.h"
#include "../include/Logger.h"
#include <iostream>
#include <chrono>
#include <thread>

int main() {
    Sparky::Logger::getInstance().setLogLevel(Sparky::LogLevel::DEBUG);
    
    try {
        // Create window
        Sparky::WindowManager windowManager;
        if (!windowManager.initialize(800, 600, "Vulkan Test")) {
            std::cerr << "Failed to create window" << std::endl;
            return -1;
        }
        
        // Create renderer
        Sparky::VulkanRenderer renderer;
        if (!renderer.initialize(windowManager.getWindowHandle())) {
            std::cerr << "Failed to initialize Vulkan renderer" << std::endl;
            return -1;
        }
        
        std::cout << "Vulkan renderer initialized successfully!" << std::endl;
        
        // Run for 5 seconds
        auto startTime = std::chrono::high_resolution_clock::now();
        while (!windowManager.shouldClose()) {
            windowManager.pollEvents();
            
            // Render frame
            renderer.render();
            
            // Check if 5 seconds have passed
            auto currentTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime);
            if (duration.count() >= 5) {
                break;
            }
            
            // Small delay to prevent excessive CPU usage
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }
        
        renderer.cleanup();
        windowManager.cleanup();
        
        std::cout << "Vulkan test completed successfully!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}