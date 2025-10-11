#include "../../Engine/include/SparkyEngine.h"
#include "../../Engine/include/Logger.h"
#include "EnhancedPhysicsDemo.h"
#include <iostream>

int main() {
    // Set up logger
    Sparky::Logger::getInstance().setLogLevel(Sparky::LogLevel::DEBUG);
    
    std::cout << "Initializing Enhanced Physics Test..." << std::endl;
    
    // Create and initialize the engine
    Sparky::SparkyEngine engine;
    
    if (!engine.initialize(1280, 720, "Sparky Engine - Enhanced Physics Test")) {
        std::cerr << "Failed to initialize engine!" << std::endl;
        return -1;
    }
    
    std::cout << "Engine initialized successfully" << std::endl;
    
    // Create and initialize the physics demo
    Sparky::EnhancedPhysicsDemo physicsDemo;
    if (!physicsDemo.initialize(&engine)) {
        std::cerr << "Failed to initialize physics demo!" << std::endl;
        return -1;
    }
    
    std::cout << "Physics demo initialized successfully" << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "  Arrow Keys - Move player" << std::endl;
    std::cout << "  Space - Jump" << std::endl;
    std::cout << "  R - Reset scene" << std::endl;
    std::cout << "  ESC - Exit" << std::endl;
    
    // Main game loop
    while (!engine.shouldClose()) {
        float deltaTime = engine.getDeltaTime();
        
        // Update physics demo
        physicsDemo.update(deltaTime);
        
        // Update engine
        engine.update();
        
        // Render frame
        engine.render();
    }
    
    std::cout << "Exiting physics test..." << std::endl;
    
    return 0;
}