#include "../include/ResourceManager.h"
#include "../include/Profiler.h"
#include "../include/Logger.h"
#include "../include/PhysicsWorld.h"
#include "../include/Config.h"
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>

// Simple test classes for the resource manager demo
class Texture {
public:
    Texture() { std::cout << "Texture created" << std::endl; }
    ~Texture() { std::cout << "Texture destroyed" << std::endl; }
};

class Shader {
public:
    Shader() { std::cout << "Shader created" << std::endl; }
    ~Shader() { std::cout << "Shader destroyed" << std::endl; }
};

// Comprehensive demo showcasing all Phase 3 dependency injection improvements
int main() {
    std::cout << "=== Sparky Engine 3 - Phase 3 Dependency Injection Demo ===" << std::endl;
    
    // 1. ResourceManager Dependency Injection
    std::cout << "\n1. ResourceManager Dependency Injection:" << std::endl;
    {
        // Using singleton (backward compatibility)
        Sparky::ResourceManager& singletonManager = Sparky::ResourceManager::getInstance();
        std::cout << "  Singleton ResourceManager created" << std::endl;
        
        // Creating ResourceManager with dependency injection
        auto graphicsManager = Sparky::ResourceManager::create("./assets/textures/");
        std::cout << "  Custom ResourceManager path: " << graphicsManager->getResourcePath() << std::endl;
    }
    
    // 2. Profiler Dependency Injection
    std::cout << "\n2. Profiler Dependency Injection:" << std::endl;
    {
        // Using singleton (backward compatibility)
        Sparky::Profiler& singletonProfiler = Sparky::Profiler::getInstance();
        std::cout << "  Singleton Profiler name: " << singletonProfiler.getProfilerName() << std::endl;
        
        // Creating Profiler with dependency injection
        auto gameProfiler = Sparky::Profiler::create("GameLogic");
        std::cout << "  Custom Profiler name: " << gameProfiler->getProfilerName() << std::endl;
        
        // Profile some work with the custom profiler
        {
            SPARKY_PROFILE_DI("GameUpdate", *gameProfiler);
            std::this_thread::sleep_for(std::chrono::milliseconds(5)); // Simulate work
        }
        
        gameProfiler->printReport();
    }
    
    // 3. PhysicsWorld Dependency Injection (from Phase 2)
    std::cout << "\n3. PhysicsWorld Dependency Injection:" << std::endl;
    {
        // Using singleton (backward compatibility)
        Sparky::PhysicsWorld& singletonWorld = Sparky::PhysicsWorld::getInstance();
        glm::vec3 earthGravity = singletonWorld.getGravity();
        std::cout << "  Singleton PhysicsWorld gravity: (" << earthGravity.x << ", " << earthGravity.y << ", " << earthGravity.z << ")" << std::endl;
        
        // Creating PhysicsWorld with dependency injection
        auto moonWorld = std::make_unique<Sparky::PhysicsWorld>(glm::vec3(0.0f, -1.6f, 0.0f)); // Moon gravity
        glm::vec3 moonGravity = moonWorld->getGravity();
        std::cout << "  Custom PhysicsWorld gravity: (" << moonGravity.x << ", " << moonGravity.y << ", " << moonGravity.z << ")" << std::endl;
    }
    
    // 4. Config Dependency Injection (from Phase 2)
    std::cout << "\n4. Config Dependency Injection:" << std::endl;
    {
        // Using singleton (backward compatibility)
        Sparky::Config& singletonConfig = Sparky::Config::getInstance();
        singletonConfig.setInt("window_width", 1920);
        std::cout << "  Singleton config window width: " << singletonConfig.getInt("window_width") << std::endl;
        
        // Creating Config with dependency injection
        auto graphicsConfig = Sparky::Config::create();
        graphicsConfig->setInt("resolution_width", 1920);
        graphicsConfig->setInt("resolution_height", 1080);
        std::cout << "  Custom config resolution: " << graphicsConfig->getInt("resolution_width") << "x" << graphicsConfig->getInt("resolution_height") << std::endl;
    }
    
    // 5. Logger Dependency Injection (from Phase 1)
    std::cout << "\n5. Logger Dependency Injection:" << std::endl;
    {
        // Create a custom logger with debug level
        auto customLogger = Sparky::Logger::create(Sparky::LogLevel::DEBUG);
        customLogger->info("Custom logger created with DEBUG level");
        customLogger->debug("This debug message will be shown");
        
        // Show backward compatibility with singleton
        Sparky::Logger::getInstance().warning("This uses the old singleton pattern");
    }
    
    // 6. Benefits of Dependency Injection
    std::cout << "\n6. Benefits of Dependency Injection:" << std::endl;
    std::cout << "  Better testability - each system can be tested independently" << std::endl;
    std::cout << "  Improved modularity - systems are less tightly coupled" << std::endl;
    std::cout << "  Enhanced flexibility - multiple instances with different configurations" << std::endl;
    std::cout << "  Backward compatibility - existing singleton code still works" << std::endl;
    std::cout << "  Better resource management - automatic cleanup with smart pointers" << std::endl;
    
    std::cout << "\n=== Phase 3 Demo Complete ===" << std::endl;
    std::cout << "All dependency injection improvements have been successfully implemented!" << std::endl;
    
    return 0;
}