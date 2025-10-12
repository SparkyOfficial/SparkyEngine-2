#include "../../Engine/include/SparkyEngine.h"
#include "../../Engine/include/Logger.h"
#include "EnhancedPhysicsDemo.h"
#include <iostream>

int main() {
    // Set up logger
    Sparky::Logger::getInstance().setLogLevel(Sparky::LogLevel::DEBUG);
    
    std::cout << "Initializing Enhanced Physics Test..." << std::endl;
    
    // Create and initialize the engine
    Sparky::Engine engine;
    
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
    // Note: Engine class doesn't have shouldClose method
    // In a full implementation, we would check the window manager
    bool shouldClose = false;
    int frameCount = 0;
    
    while (!shouldClose) {
        // Note: Engine class doesn't have getDeltaTime method
        float deltaTime = 0.016f; // Assume 60 FPS
        
        // Update physics demo
        physicsDemo.update(deltaTime);
        
        // Note: Engine class doesn't have update and render methods
        // In a full implementation, we would update and render the engine differently
        
        // Simulate engine update and render
        engine.getWindowManager().pollEvents();
        engine.getRenderer().render();
        
        // Simulate shouldClose after some frames for testing
        frameCount++;
        if (frameCount > 1000) { // Run for about 16 seconds at 60 FPS
            shouldClose = true;
        }
    }
    
    std::cout << "Exiting physics test..." << std::endl;
    
    return 0;
}