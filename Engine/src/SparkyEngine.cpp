#include "../include/SparkyEngine.h"
#include "../include/Logger.h"
#include <GLFW/glfw3.h>

namespace Sparky {

    Engine::Engine() : isRunning(false) {
    }

    Engine::~Engine() {
        shutdown();
    }

    bool Engine::initialize(int windowWidth, int windowHeight, const char* windowTitle) {
        SPARKY_LOG_INFO("Initializing Sparky Engine...");
        
        // Initialize window manager
        SPARKY_LOG_INFO("Initializing window manager...");
        if (!windowManager.initialize(windowWidth, windowHeight, windowTitle)) {
            SPARKY_LOG_ERROR("Failed to initialize window manager");
            return false;
        }

        // Initialize input manager
        SPARKY_LOG_INFO("Initializing input manager...");
        inputManager.initialize(static_cast<GLFWwindow*>(windowManager.getWindowHandle()));

        // Initialize renderer
        SPARKY_LOG_INFO("Initializing renderer...");
        renderer.setEngine(this);  // Set the engine reference
        if (!renderer.initialize(windowManager.getWindowHandle())) {
            SPARKY_LOG_ERROR("Failed to initialize renderer");
            return false;
        }

        // Initialize render system
        SPARKY_LOG_INFO("Initializing render system...");
        renderSystem.initialize(&renderer);

        isRunning = true;
        SPARKY_LOG_INFO("Sparky Engine initialized successfully");
        return true;
    }

    void Engine::run() {
        SPARKY_LOG_INFO("Starting game loop...");
        float lastTime = 0.0f;
        
        while (isRunning && !windowManager.shouldClose()) {
            // Calculate delta time
            float currentTime = static_cast<float>(glfwGetTime());
            float deltaTime = currentTime - lastTime;
            lastTime = currentTime;

            // Poll events
            windowManager.pollEvents();
            
            // Update input
            inputManager.update();
            
            // Check for exit key (ESC)
            if (inputManager.isKeyJustPressed(256)) { // ESC key
                SPARKY_LOG_INFO("ESC key pressed, shutting down...");
                isRunning = false;
            }
            
            // Render frame
            renderer.render();
        }
        
        SPARKY_LOG_INFO("Game loop ended");
    }

    void Engine::shutdown() {
        if (isRunning) {
            SPARKY_LOG_INFO("Shutting down Sparky Engine...");
            renderSystem.cleanup();
            renderer.cleanup();
            windowManager.cleanup();
            isRunning = false;
            SPARKY_LOG_INFO("Sparky Engine shut down");
        }
    }
}