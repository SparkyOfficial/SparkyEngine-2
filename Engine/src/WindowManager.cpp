#include "../include/WindowManager.h"

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
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            return false;
        }

        // Configure GLFW for Vulkan
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        // Create window
        window = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (!window) {
            std::cerr << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return false;
        }

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
        return glfwWindowShouldClose(window);
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