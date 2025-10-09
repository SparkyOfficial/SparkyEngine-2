#include "../include/SparkyEngine.h"
#include "../include/Logger.h"
#include "../include/Config.h"
#include <iostream>

int main() {
    std::cout << "Starting Simple Engine Test" << std::endl;
    
    // Set up logger
    Sparky::Logger::getInstance().setLogLevel(Sparky::LogLevel::DEBUG);
    std::cout << "Logger set up" << std::endl;
    
    // Load engine configuration
    Sparky::Config::getInstance().loadFromFile("engine.cfg");
    std::cout << "Config loaded" << std::endl;
    
    std::cout << "Simple Engine Test" << std::endl;
    
    // Create and initialize the engine
    Sparky::Engine engine;
    std::cout << "Engine created" << std::endl;
    
    int windowWidth = Sparky::Config::getInstance().getInt("window_width", 1280);
    int windowHeight = Sparky::Config::getInstance().getInt("window_height", 720);
    std::string windowTitle = Sparky::Config::getInstance().getString("window_title", "Sparky Engine");
    
    std::cout << "Initializing engine with window size: " << windowWidth << "x" << windowHeight << std::endl;
    
    if (!engine.initialize(windowWidth, windowHeight, windowTitle.c_str())) {
        std::cout << "Failed to initialize engine!" << std::endl;
        return -1;
    }
    
    std::cout << "Engine initialized successfully" << std::endl;
    
    // Run the engine
    std::cout << "About to call engine.run()" << std::endl;
    engine.run();
    std::cout << "Returned from engine.run()" << std::endl;
    
    std::cout << "Engine finished running" << std::endl;
    
    return 0;
}