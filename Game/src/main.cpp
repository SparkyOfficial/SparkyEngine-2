#include "../../Engine/include/SparkyEngine.h"
#include "../../Engine/include/InputManager.h"
#include "../../Engine/include/CollisionSystem.h"
#include "../../Engine/include/Logger.h"
#include "../../Engine/include/Config.h"
#include "../../Engine/include/Profiler.h"
#include "../../Engine/include/ExampleState.h"
#include "../../Engine/include/StateMachine.h"
#include "Player.h"
#include "Level.h"
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
    
    // Create game objects
    Sparky::Player player;
    Sparky::Level level("TestLevel");
    
    // Set up player camera
    player.setCamera(&engine.getCamera());
    
    // Load the level from file
    level.loadLevelFromFile("../Game/assets/level1.json");
    
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
        
        // Handle player input
        if (inputManager.isKeyPressed(87)) { // W key
            player.moveForward(deltaTime);
        }
        if (inputManager.isKeyPressed(83)) { // S key
            player.moveBackward(deltaTime);
        }
        if (inputManager.isKeyPressed(65)) { // A key
            player.moveLeft(deltaTime);
        }
        if (inputManager.isKeyPressed(68)) { // D key
            player.moveRight(deltaTime);
        }
        if (inputManager.isKeyJustPressed(32)) { // Space key
            player.jump();
        }
        
        // Check for exit key (ESC)
        if (inputManager.isKeyJustPressed(256)) { // ESC key
            SPARKY_LOG_INFO("ESC key pressed, shutting down...");
            break;
        }
        
        // Update state machine
        stateMachine.update(deltaTime);
        
        // Update game objects
        player.update(deltaTime);
        level.update(deltaTime);
        
        // Simple ground collision detection
        // In a real implementation, this would be more sophisticated
        glm::vec3 playerPos = player.getPosition();
        if (playerPos.y < 0.0f) {
            playerPos.y = 0.0f;
            player.setPosition(playerPos);
            player.setOnGround(true);
        } else {
            player.setOnGround(false);
        }
        
        // Render frame
        engine.getRenderer().render();
    }
    
    // Clean up
    level.unloadLevel();
    
    // Print profiling report
    Sparky::Profiler::getInstance().printReport();
    
    SPARKY_LOG_INFO("Game shutdown successfully");
    return 0;
}