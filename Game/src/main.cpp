#include "../../Engine/include/SparkyEngine.h"
#include "../../Engine/include/InputManager.h"
#include "../../Engine/include/CollisionSystem.h"
#include "../../Engine/include/Logger.h"
#include "../../Engine/include/Config.h"
#include "../../Engine/include/Profiler.h"
#include "../../Engine/include/ExampleState.h"
#include "../../Engine/include/StateMachine.h"
#include "ExampleGame.h"
#include "Player.h"
#include "Gun.h"
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
    
    // Create player
    auto player = std::make_unique<Sparky::Player>();
    player->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    player->setCamera(&engine.getCamera());
    
    // Create gun
    auto gun = std::make_unique<Sparky::Gun>();
    gun->setCamera(&engine.getCamera());
    
    // Register player and gun with the render system
    engine.getRenderSystem().registerGameObject(player.get());
    engine.getRenderSystem().registerGameObject(gun.get());
    
    // Start the game
    game.startGame();
    
    SPARKY_LOG_INFO("Game initialized successfully");
    SPARKY_LOG_INFO("Controls:");
    SPARKY_LOG_INFO("  WASD - Move");
    SPARKY_LOG_INFO("  Mouse - Look around");
    SPARKY_LOG_INFO("  Space - Jump");
    SPARKY_LOG_INFO("  Left Mouse Button - Shoot");
    SPARKY_LOG_INFO("  R - Reload");
    SPARKY_LOG_INFO("  ESC - Toggle mouse lock");
    
    float lastTime = 0.0f;
    
    // Main game loop
    while (!engine.getWindowManager().shouldClose()) {
        SPARKY_PROFILE("GameLoop");
        
        // Calculate delta time
        float currentTime = static_cast<float>(glfwGetTime());
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        
        // Debug: Log frame information
        static int frameCount = 0;
        frameCount++;
        if (frameCount % 60 == 0) { // Log every 60 frames
            SPARKY_LOG_DEBUG("Main loop frame " + std::to_string(frameCount) + ", Delta time: " + std::to_string(deltaTime));
        }
        
        // Poll events
        engine.getWindowManager().pollEvents();
        
        // Get input manager
        Sparky::InputManager& inputManager = engine.getInputManager();
        
        // Update input
        inputManager.update();
        
        // Update player
        player->update(deltaTime);
        
        // Update gun
        gun->update(deltaTime);
        
        // Debug: Log state machine and game update
        if (frameCount % 60 == 0) {
            SPARKY_LOG_DEBUG("Updating state machine and game");
        }
        
        // Update state machine
        stateMachine.update(deltaTime);
        
        // Update game
        game.update(deltaTime);
        
        // Debug: Log render system info
        if (frameCount % 60 == 0) {
            Sparky::RenderSystem& renderSystem = engine.getRenderSystem();
            SPARKY_LOG_DEBUG("RenderSystem has " + std::to_string(renderSystem.getGameObjects().size()) + " game objects");
        }
        
        // Debug: Log render calls
        if (frameCount % 60 == 0) {
            SPARKY_LOG_DEBUG("Calling engine renderer");
        }
        
        // Render frame
        engine.getRenderer().render();
        
        if (frameCount % 60 == 0) {
            SPARKY_LOG_DEBUG("Calling game render");
        }
        
        game.render();
    }

    // End the game
    game.endGame();
    
    // Print profiling report
    Sparky::Profiler::getInstance().printReport();
    
    SPARKY_LOG_INFO("Game shutdown successfully");
    return 0;
}