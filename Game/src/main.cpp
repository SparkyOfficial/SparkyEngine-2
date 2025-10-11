#include "../../Engine/include/SparkyEngine.h"
#include "../../Engine/include/InputManager.h"
#include "../../Engine/include/CollisionSystem.h"
#include "../../Engine/include/Logger.h"
#include "../../Engine/include/Config.h"
#include "../../Engine/include/Profiler.h"
#include "../../Engine/include/ExampleState.h"
#include "../../Engine/include/StateMachine.h"
#include "../../Engine/include/GUIManager.h"
#include "../../Engine/include/FileUtils.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ExampleGame.h"
#include "Player.h"
#include "Gun.h"
#include <iostream>
#include <vector>

#ifdef HAS_GLFW
#include <GLFW/glfw3.h>
#endif

int main() {
    SPARKY_PROFILE("Main");
    
    // Load engine configuration
    Sparky::Config::getInstance().loadFromFile("engine.cfg");
    
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
    
    SPARKY_LOG_INFO("Initializing engine with window size: " + std::to_string(windowWidth) + "x" + std::to_string(windowHeight));
    
    if (!engine.initialize(windowWidth, windowHeight, windowTitle.c_str())) {
        SPARKY_LOG_ERROR("Failed to initialize engine!");
        return -1;
    }
    
    SPARKY_LOG_INFO("Engine initialized successfully");
    
    // Create and initialize the game
    Sparky::ExampleGame game;
    if (!game.initialize(&engine)) {
        SPARKY_LOG_ERROR("Failed to initialize game!");
        return -1;
    }
    
    SPARKY_LOG_INFO("Game initialized successfully");
    
    // Create GUI elements
    Sparky::GUIManager& guiManager = Sparky::GUIManager::getInstance();
    
    // Create HUD
    guiManager.createHUD();
    
    // Create menus
    guiManager.createMainMenu();
    guiManager.createPauseMenu();
    
    // Show main menu initially
    guiManager.showMenu("main");
    
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
    
    SPARKY_LOG_INFO("Game fully initialized, about to start main loop");
    SPARKY_LOG_INFO("Controls:");
    SPARKY_LOG_INFO("  WASD - Move");
    SPARKY_LOG_INFO("  Mouse - Look around");
    SPARKY_LOG_INFO("  Space - Jump");
    SPARKY_LOG_INFO("  Left Mouse Button - Shoot");
    SPARKY_LOG_INFO("  R - Reload");
    SPARKY_LOG_INFO("  ESC - Toggle mouse lock");
    SPARKY_LOG_INFO("  F1 - Show main menu");
    SPARKY_LOG_INFO("  F2 - Show pause menu");
    SPARKY_LOG_INFO("  F3 - Hide menus");
    
    float lastTime = 0.0f;
    bool gameStarted = true; // Start the game immediately
    
    SPARKY_LOG_INFO("About to enter main game loop");
    
    // Main game loop
    int frameCounter = 0;
    bool shouldClose = engine.getWindowManager().shouldClose();
    SPARKY_LOG_INFO("Initial shouldClose value: " + std::to_string(shouldClose));
    
    if (shouldClose) {
        SPARKY_LOG_ERROR("Window manager reported shouldClose immediately");
        // Don't exit immediately, let's see what happens
    }
    
    SPARKY_LOG_INFO("Entering main game loop");
    
    // Add a counter to see if we're actually entering the loop
    int loopEntryCount = 0;
    
    while (!shouldClose) {
        loopEntryCount++;
        if (loopEntryCount == 1) {
            SPARKY_LOG_INFO("Successfully entered main game loop");
        }
        
        frameCounter++;
        if (frameCounter % 60 == 0) {
            SPARKY_LOG_DEBUG("Game loop running, frame: " + std::to_string(frameCounter));
        }
        
        // Debug: Check if window should close immediately
        if (frameCounter == 1) {
            SPARKY_LOG_DEBUG("Window should close check: " + std::to_string(engine.getWindowManager().shouldClose()));
        }
        
        SPARKY_PROFILE("GameLoop");
        
        // Calculate delta time
#ifdef HAS_GLFW
        float currentTime = static_cast<float>(glfwGetTime());
#else
        // Fallback implementation for delta time calculation
        static float fallbackTime = 0.0f;
        fallbackTime += 0.016f; // Assume 60 FPS
        float currentTime = fallbackTime;
#endif
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        
        // Debug: Log frame information
        static int debugFrameCount = 0;
        debugFrameCount++;
        if (debugFrameCount % 60 == 0) { // Log every 60 frames
            SPARKY_LOG_DEBUG("Main loop frame " + std::to_string(debugFrameCount) + ", Delta time: " + std::to_string(deltaTime));
        }
        
        // Poll events
        engine.getWindowManager().pollEvents();
        
        // Get input manager
        Sparky::InputManager& inputManager = engine.getInputManager();
        
        // Update input
        inputManager.update();
        
        // Handle menu controls
#ifdef HAS_GLFW
        if (inputManager.isKeyJustPressed(GLFW_KEY_F1)) {
            guiManager.showMenu("main");
        }
        if (inputManager.isKeyJustPressed(GLFW_KEY_F2)) {
            guiManager.showMenu("pause");
        }
        if (inputManager.isKeyJustPressed(GLFW_KEY_F3)) {
            guiManager.hideAllMenus();
            gameStarted = true; // Start the game when menus are hidden
        }
#else
        // Fallback implementation for menu controls
        // For now, we'll just start the game immediately
        gameStarted = true;
#endif
        
        // Update game elements only if the game has started
        if (gameStarted) {
            // Update player
            player->update(deltaTime);
            
            // Update gun
            gun->update(deltaTime);
            
            // Update health and ammo displays
            guiManager.updateHealthDisplay(100); // Placeholder value
            guiManager.updateAmmoDisplay(gun->getAmmo(), gun->getTotalAmmo());
        }
        
        // Update GUI
        guiManager.update(deltaTime);
        
        // Debug: Log state machine and game update
        if (debugFrameCount % 60 == 0) {
            SPARKY_LOG_DEBUG("Updating state machine and game");
        }
        
        // Update state machine
        stateMachine.update(deltaTime);
        
        // Update game
        game.update(deltaTime);
        
        // Debug: Log render system info
        if (debugFrameCount % 60 == 0) {
            Sparky::RenderSystem& renderSystem = engine.getRenderSystem();
            SPARKY_LOG_DEBUG("RenderSystem has " + std::to_string(renderSystem.getGameObjects().size()) + " game objects");
        }
        
        // Debug: Log render calls
        if (debugFrameCount % 60 == 0) {
            SPARKY_LOG_DEBUG("Calling engine renderer");
        }
        
        // Render frame
        engine.getRenderer().render();
        
        // Render GUI
        guiManager.render();
        
        if (debugFrameCount % 60 == 0) {
            SPARKY_LOG_DEBUG("Calling game render");
        }
        
        game.render();
        
        // Update shouldClose flag
        shouldClose = engine.getWindowManager().shouldClose();
        
        // Additional debug: Check for ESC key to manually exit
#ifdef HAS_GLFW
        if (inputManager.isKeyJustPressed(GLFW_KEY_ESCAPE)) {
            SPARKY_LOG_INFO("ESC key pressed, setting shouldClose to true");
            shouldClose = true; // Set shouldClose to true to exit the loop
        }
#endif
        
        // Additional debug: Log every 100 frames to see if we're still running
        if (frameCounter % 100 == 0) {
            SPARKY_LOG_INFO("Still running, frame: " + std::to_string(frameCounter));
        }
        
        // Remove the artificial frame limit to allow the game to run indefinitely
        // This was only added for debugging purposes
        // The game will now only exit when the window is closed or ESC is pressed
        /*
        // If we've been in the loop for more than 1000 iterations, exit to prevent infinite loop
        if (frameCounter > 1000) {
            SPARKY_LOG_INFO("Exiting after 1000 frames to prevent infinite loop");
            break;
        }
        */
    }

    SPARKY_LOG_INFO("Exiting main game loop after " + std::to_string(frameCounter) + " frames");
    
    // End the game
    game.endGame();
    
    // Print profiling report
    Sparky::Profiler::getInstance().printReport();
    
    SPARKY_LOG_INFO("Game shutdown successfully");
    return 0;
}