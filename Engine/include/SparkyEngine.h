#pragma once

// Main engine header that includes all essential components

#include "VulkanRenderer.h"
#include "WindowManager.h"
#include "GameEntity.h"
#include "InputManager.h"
#include "Camera.h"

// Core engine namespace
namespace Sparky {
    // Engine class to manage all subsystems
    class Engine {
    public:
        Engine();
        ~Engine();

        bool initialize(int windowWidth, int windowHeight, const char* windowTitle);
        void run();
        void shutdown();

        // Accessors
        VulkanRenderer& getRenderer() { return renderer; }
        WindowManager& getWindowManager() { return windowManager; }
        InputManager& getInputManager() { return inputManager; }
        Camera& getCamera() { return camera; }

    private:
        VulkanRenderer renderer;
        WindowManager windowManager;
        InputManager inputManager;
        Camera camera;

        bool isRunning;
    };
}