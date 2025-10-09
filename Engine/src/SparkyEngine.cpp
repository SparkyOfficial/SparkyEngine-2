#include "../include/SparkyEngine.h"
#include "../include/Logger.h"

#ifdef HAS_GLFW
#include <GLFW/glfw3.h>
#endif

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
#ifdef HAS_GLFW
        inputManager.initialize(static_cast<GLFWwindow*>(windowManager.getWindowHandle()));
#else
        inputManager.initialize(nullptr);
#endif

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
        
        SPARKY_LOG_INFO("About to enter game loop - if you see this message but not the 'Actually entering game loop' message, the program is crashing or exiting before entering the loop. Adding more debug info:");
        int frameCount = 0;
        
        SPARKY_LOG_INFO("isRunning: " + std::to_string(isRunning));
        SPARKY_LOG_INFO("windowManager.shouldClose(): " + std::to_string(windowManager.shouldClose()));
        
        // Add a check to see if we're entering the loop
        if (!isRunning || windowManager.shouldClose()) {
            SPARKY_LOG_INFO("Not entering game loop - isRunning: " + std::to_string(isRunning) + 
                           ", shouldClose: " + std::to_string(windowManager.shouldClose()));
            return;
        }
        
        SPARKY_LOG_INFO("Actually entering game loop - this should be visible");
        SPARKY_LOG_INFO("If you don't see this message, the program is crashing or exiting before entering the loop");
        
        while (isRunning && !windowManager.shouldClose()) {
            frameCount++;
            SPARKY_LOG_DEBUG("Game loop frame: " + std::to_string(frameCount));
            
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
        
        SPARKY_LOG_INFO("Game loop ended after " + std::to_string(frameCount) + " frames");
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