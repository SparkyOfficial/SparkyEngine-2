#include "../../Engine/include/SparkyEngine.h"
#include "../../Engine/include/Logger.h"
#include "../../Engine/include/AudioEngine.h"
#include "AudioDemo.h"
#include <iostream>

int main() {
    // Set up logger
    Sparky::Logger::getInstance().setLogLevel(Sparky::LogLevel::DEBUG);
    
    std::cout << "Initializing Enhanced Audio Test..." << std::endl;
    
    // Create and initialize the engine
    Sparky::SparkyEngine engine;
    
    if (!engine.initialize(1280, 720, "Sparky Engine - Enhanced Audio Test")) {
        std::cerr << "Failed to initialize engine!" << std::endl;
        return -1;
    }
    
    std::cout << "Engine initialized successfully" << std::endl;
    
    // Initialize audio engine
    if (!Sparky::AudioEngine::getInstance().initialize()) {
        std::cerr << "Failed to initialize audio engine!" << std::endl;
        return -1;
    }
    
    std::cout << "Audio engine initialized successfully" << std::endl;
    
    // Create and initialize the audio demo
    Sparky::AudioDemo audioDemo;
    if (!audioDemo.initialize(&engine)) {
        std::cerr << "Failed to initialize audio demo!" << std::endl;
        return -1;
    }
    
    std::cout << "Audio demo initialized successfully" << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "  Arrow Keys - Move player" << std::endl;
    std::cout << "  Space - Play footstep sound" << std::endl;
    std::cout << "  Enter - Play beep sound" << std::endl;
    std::cout << "  R - Reset scene" << std::endl;
    std::cout << "  ESC - Exit" << std::endl;
    
    // Main game loop
    while (!engine.shouldClose()) {
        float deltaTime = engine.getDeltaTime();
        
        // Update audio demo
        audioDemo.update(deltaTime);
        
        // Update engine
        engine.update();
        
        // Render frame
        engine.render();
    }
    
    // Clean up audio engine
    Sparky::AudioEngine::getInstance().cleanup();
    
    std::cout << "Exiting audio test..." << std::endl;
    
    return 0;
}