#include "../../Engine/include/SparkyEngine.h"
#include "../../Engine/include/InputManager.h"
#include "../../Engine/include/CollisionSystem.h"
#include "../../Engine/include/Logger.h"
#include "../../Engine/include/Config.h"
#include "../../Engine/include/Profiler.h"
#include "../../Engine/include/ExampleState.h"
#include "../../Engine/include/StateMachine.h"
#include "ExampleGame.h"
#include <iostream>
#include <vector>

int main() {
    SPARKY_PROFILE("Main");
    
    // Load engine configuration
    Sparky::Config::getInstance().loadFromFile("../Engine/assets/engine.cfg");
    
    // Set up logger
    Sparky::Logger::getInstance().setLogLevel(
        static_cast<Sparky::LogLevel>(Sparky::Config::getInstance().getInt("log_level", 1))
    );
    
    SPARKY_LOG_INFO("Starting Sparky Engine Test Game");
    
    // Create state machine
    Sparky::StateMachine stateMachine;
    auto gameState = std::make_unique<Sparky::ExampleState>("GameState");
    stateMachine.addState(std::move(gameState));
    stateMachine.setState("GameState");
    
    // Create and initialize the engine
    Sparky::Engine engine;
    
    int windowWidth = Sparky::Config::getInstance().getInt("window_width", 1280);
    int windowHeight = Sparky::Config::getInstance().getInt("window_height", 720);
    std::string windowTitle = Sparky::Config::getInstance().getString("window_title", "Sparky Engine");
    
    if (!engine.initialize(windowWidth, windowHeight, windowTitle.c_str())) {
        SPARKY_LOG_ERROR("Failed to initialize engine!");
        return -1;
    }
    
    // Create and initialize the game
    Sparky::ExampleGame game;
    if (!game.initialize(&engine)) {
        SPARKY_LOG_ERROR("Failed to initialize game!");
        return -1;
    }
    
    // Start the game
    game.startGame();
    
    SPARKY_LOG_INFO("Game initialized successfully");
    SPARKY_LOG_INFO("Controls:");
    SPARKY_LOG_INFO("  WASD - Move");
    SPARKY_LOG_INFO("  Space - Jump");
    SPARKY_LOG_INFO("  ESC - Exit");
    
    float lastTime = 0.0f;
    
    // Main game loop
    while (!engine.getWindowManager().shouldClose()) {
        SPARKY_PROFILE("GameLoop");
        
        // Calculate delta time
        float currentTime = static_cast<float>(glfwGetTime());
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        
        // Poll events
        engine.getWindowManager().pollEvents();
        
        // Get input manager
        Sparky::InputManager& inputManager = engine.getInputManager();
        
        // Update input
        inputManager.update();
        
        // Check for exit key (ESC)
        if (inputManager.isKeyJustPressed(256)) { // ESC key
            SPARKY_LOG_INFO("ESC key pressed, shutting down...");
            break;
        }
        
        // Update state machine
        stateMachine.update(deltaTime);
        
        // Update game
        game.update(deltaTime);
        
        // Render frame
        engine.getRenderer().render();
        game.render();
    }
    
    // End the game
    game.endGame();
    
    // Print profiling report
    Sparky::Profiler::getInstance().printReport();
    
    SPARKY_LOG_INFO("Game shutdown successfully");
    return 0;
}