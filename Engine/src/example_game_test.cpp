#include "../include/SparkyEngine.h"
#include "../include/Logger.h"
#include "../include/Config.h"
#include "ExampleGame.h"
#include <iostream>

int main() {
    // Set up logger
    Sparky::Logger::getInstance().setLogLevel(Sparky::LogLevel::DEBUG);
    
    // Load engine configuration
    Sparky::Config::getInstance().loadFromFile("engine.cfg");
    
    std::cout << "Testing ExampleGame implementation" << std::endl;
    
    // Create and initialize the engine
    Sparky::Engine engine;
    
    int windowWidth = Sparky::Config::getInstance().getInt("window_width", 1280);
    int windowHeight = Sparky::Config::getInstance().getInt("window_height", 720);
    std::string windowTitle = Sparky::Config::getInstance().getString("window_title", "Sparky Engine");
    
    std::cout << "Initializing engine with window size: " << windowWidth << "x" << windowHeight << std::endl;
    
    if (!engine.initialize(windowWidth, windowHeight, windowTitle.c_str())) {
        std::cout << "Failed to initialize engine!" << std::endl;
        return -1;
    }
    
    std::cout << "Engine initialized successfully" << std::endl;
    
    // Create and initialize the game
    Sparky::ExampleGame game;
    if (!game.initialize(&engine)) {
        std::cout << "Failed to initialize game!" << std::endl;
        return -1;
    }
    
    std::cout << "Game initialized successfully" << std::endl;
    
    // Start the game
    game.startGame();
    
    std::cout << "Game started successfully" << std::endl;
    
    // Run the engine
    engine.run();
    
    std::cout << "Engine finished running" << std::endl;
    
    return 0;
}