#include "../include/Logger.h"
#include "../include/PhysicsWorld.h"
#include "../include/Config.h"
#include <iostream>
#include <memory>

// Comprehensive demo showcasing all Phase 2 dependency injection improvements
int main() {
    std::cout << "=== Sparky Engine 3 - Phase 2 Dependency Injection Demo ===" << std::endl;
    
    // 1. Logger Dependency Injection (from Phase 1)
    std::cout << "\n1. Logger Dependency Injection:" << std::endl;
    {
        // Create a custom logger with debug level
        auto customLogger = Sparky::Logger::create(Sparky::LogLevel::DEBUG);
        customLogger->info("Custom logger created with DEBUG level");
        customLogger->debug("This debug message will be shown");
        
        // Show backward compatibility with singleton
        Sparky::Logger::getInstance().warning("This uses the old singleton pattern");
    }
    
    // 2. PhysicsWorld Dependency Injection (Phase 2)
    std::cout << "\n2. PhysicsWorld Dependency Injection:" << std::endl;
    {
        // Using singleton (backward compatibility)
        Sparky::PhysicsWorld& singletonWorld = Sparky::PhysicsWorld::getInstance();
        glm::vec3 earthGravity = singletonWorld.getGravity();
        std::cout << "  Singleton PhysicsWorld gravity: (" << earthGravity.x << ", " << earthGravity.y << ", " << earthGravity.z << ")" << std::endl;
        
        // Creating PhysicsWorld with dependency injection
        auto moonWorld = std::make_unique<Sparky::PhysicsWorld>(glm::vec3(0.0f, -1.6f, 0.0f)); // Moon gravity
        glm::vec3 moonGravity = moonWorld->getGravity();
        std::cout << "  Custom PhysicsWorld gravity: (" << moonGravity.x << ", " << moonGravity.y << ", " << moonGravity.z << ")" << std::endl;
        
        // Using factory method
        auto factoryWorld = Sparky::PhysicsWorld::create(glm::vec3(0.0f, -20.0f, 0.0f)); // Custom gravity
        glm::vec3 customGravity = factoryWorld->getGravity();
        std::cout << "  Factory PhysicsWorld gravity: (" << customGravity.x << ", " << customGravity.y << ", " << customGravity.z << ")" << std::endl;
    }
    
    // 3. Config Dependency Injection (Phase 2)
    std::cout << "\n3. Config Dependency Injection:" << std::endl;
    {
        // Using singleton (backward compatibility)
        Sparky::Config& singletonConfig = Sparky::Config::getInstance();
        singletonConfig.setInt("window_width", 1920);
        singletonConfig.setInt("window_height", 1080);
        singletonConfig.setBool("fullscreen", false);
        
        std::cout << "  Singleton config values:" << std::endl;
        std::cout << "    Window width: " << singletonConfig.getInt("window_width") << std::endl;
        std::cout << "    Window height: " << singletonConfig.getInt("window_height") << std::endl;
        std::cout << "    Fullscreen: " << (singletonConfig.getBool("fullscreen") ? "true" : "false") << std::endl;
        
        // Creating Config with dependency injection
        auto graphicsConfig = Sparky::Config::create();
        graphicsConfig->setInt("resolution_width", 1920);
        graphicsConfig->setInt("resolution_height", 1080);
        graphicsConfig->setString("renderer", "Vulkan");
        graphicsConfig->setBool("vsync", true);
        
        std::cout << "  Custom config values:" << std::endl;
        std::cout << "    Resolution: " << graphicsConfig->getInt("resolution_width") << "x" << graphicsConfig->getInt("resolution_height") << std::endl;
        std::cout << "    Renderer: " << graphicsConfig->getString("renderer") << std::endl;
        std::cout << "    VSync: " << (graphicsConfig->getBool("vsync") ? "enabled" : "disabled") << std::endl;
    }
    
    // 4. Benefits of Dependency Injection
    std::cout << "\n4. Benefits of Dependency Injection:" << std::endl;
    std::cout << "  Better testability - each system can be tested independently" << std::endl;
    std::cout << "  Improved modularity - systems are less tightly coupled" << std::endl;
    std::cout << "  Enhanced flexibility - multiple instances with different configurations" << std::endl;
    std::cout << "  Backward compatibility - existing singleton code still works" << std::endl;
    std::cout << "  Better resource management - automatic cleanup with smart pointers" << std::endl;
    
    std::cout << "\n=== Phase 2 Demo Complete ===" << std::endl;
    std::cout << "All dependency injection improvements have been successfully implemented!" << std::endl;
    
    return 0;
}