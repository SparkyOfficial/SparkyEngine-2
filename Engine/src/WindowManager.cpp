#include "../include/WindowManager.h"
#include "../include/Logger.h"
#include <string>

#ifdef HAS_GLFW
#include <GLFW/glfw3.h>
#include <iostream>

namespace Sparky {

    WindowManager::WindowManager() : window(nullptr), width(0), height(0), title(nullptr) {
    }

    WindowManager::~WindowManager() {
        cleanup();
    }

    bool WindowManager::initialize(int width, int height, const char* title) {
        this->width = width;
        this->height = height;
        this->title = title;

        // Initialize GLFW
        SPARKY_LOG_DEBUG("Initializing GLFW");
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            return false;
        }
        SPARKY_LOG_DEBUG("GLFW initialized successfully");

        // Configure GLFW for Vulkan
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        // Create window
        SPARKY_LOG_DEBUG("Creating GLFW window with size " + std::to_string(width) + "x" + std::to_string(height));
        window = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (!window) {
            std::cerr << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return false;
        }
        
        // Debug: Check if window was created successfully
        std::cout << "GLFW window created successfully" << std::endl;
        SPARKY_LOG_DEBUG("GLFW window created successfully");

        // Show and bring the window to front
        glfwShowWindow(window);
        glfwFocusWindow(window);

        // Set up framebuffer resize callback
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);

        return true;
    }

    void WindowManager::cleanup() {
        if (window) {
            glfwDestroyWindow(window);
        }
        glfwTerminate();
    }

    bool WindowManager::shouldClose() {
        if (!window) {
            SPARKY_LOG_DEBUG("Window is null, returning true for shouldClose");
            return true;
        }
        
        bool result = glfwWindowShouldClose(window);
        // Debug: Log the shouldClose result
        static bool firstCall = true;
        if (firstCall) {
            SPARKY_LOG_DEBUG("WindowManager::shouldClose() returned: " + std::to_string(result));
            firstCall = false;
        }
        return result;
    }

    void WindowManager::pollEvents() {
        glfwPollEvents();
    }
    
    // Static callback function for framebuffer resize
    void WindowManager::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
        auto app = reinterpret_cast<WindowManager*>(glfwGetWindowUserPointer(window));
        app->framebufferResized = true;
    }
}

#else

// Fallback implementation without GLFW
#include <iostream>

namespace Sparky {

    WindowManager::WindowManager() : window(nullptr), width(0), height(0), title(nullptr) {
    }

    WindowManager::~WindowManager() {
        cleanup();
    }

    bool WindowManager::initialize(int width, int height, const char* title) {
        this->width = width;
        this->height = height;
        this->title = title;
        
        std::cerr << "GLFW not available. Window management not supported." << std::endl;
        return false;
    }

    void WindowManager::cleanup() {
        // Nothing to clean up in fallback implementation
    }

    bool WindowManager::shouldClose() {
        return true; // Always close in fallback implementation
    }

    void WindowManager::pollEvents() {
        // Nothing to poll in fallback implementation
    }
    
    // Static callback function for framebuffer resize (dummy implementation)
    void WindowManager::framebufferResizeCallback(void* window, int width, int height) {
        // Dummy implementation
    }
}

#endif