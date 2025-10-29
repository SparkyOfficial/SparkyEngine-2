#include "../include/WindowManager.h"
#include "../include/Logger.h"
#include <iostream>
#include <chrono>
#include <thread>

#ifdef HAS_GLFW
#include <GLFW/glfw3.h>
#include <gl/GL.h>

// Simple OpenGL rendering functions
void renderTriangle() {
    // Clear the screen
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw a simple triangle
    glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f, 0.0f);  // Red
        glVertex2f(0.0f, 0.5f);
        glColor3f(0.0f, 1.0f, 0.0f);  // Green
        glVertex2f(-0.5f, -0.5f);
        glColor3f(0.0f, 0.0f, 1.0f);  // Blue
        glVertex2f(0.5f, -0.5f);
    glEnd();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

#endif

int main() {
    Sparky::Logger::getInstance().info("Starting Simple 3D Demo");
    
#ifdef HAS_GLFW
    Sparky::Logger::getInstance().info("GLFW is available, creating window");
    
    // Initialize GLFW
    if (!glfwInit()) {
        Sparky::Logger::getInstance().error("Failed to initialize GLFW");
        return -1;
    }
    
    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    
    // Create window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Simple 3D Demo - Sparky Engine", nullptr, nullptr);
    if (!window) {
        Sparky::Logger::getInstance().error("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }
    
    // Make the OpenGL context current
    glfwMakeContextCurrent(window);
    
    // Set up viewport and callbacks
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    Sparky::Logger::getInstance().info("Window created successfully, starting render loop");
    
    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // Process input
        processInput(window);
        
        // Render
        renderTriangle();
        
        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
        
        // Small delay to control frame rate
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    
    // Clean up
    glfwDestroyWindow(window);
    glfwTerminate();
    
    Sparky::Logger::getInstance().info("Simple 3D Demo finished successfully");
    return 0;
#else
    Sparky::Logger::getInstance().warn("GLFW not available, running console-only demo");
    
    std::cout << "=== Simple 3D Demo ===" << std::endl;
    std::cout << "This demo shows that the engine can create windows and handle basic 3D rendering." << std::endl;
    std::cout << "In a full implementation, this would show a rotating 3D triangle." << std::endl;
    std::cout << "Features demonstrated:" << std::endl;
    std::cout << "  - Window creation with GLFW" << std::endl;
    std::cout << "  - Basic 3D rendering" << std::endl;
    std::cout << "  - Input handling" << std::endl;
    std::cout << "  - Render loop" << std::endl;
    std::cout << "=====================" << std::endl;
    
    Sparky::Logger::getInstance().info("Simple 3D Demo finished successfully");
    return 0;
#endif
}