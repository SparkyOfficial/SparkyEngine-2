#include "../include/WindowManager.h"
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
}