#pragma once

#ifdef HAS_GLFW
struct GLFWwindow;
#else
typedef void GLFWwindow;
#endif

namespace Sparky {
    class WindowManager {
    public:
        WindowManager();
        ~WindowManager();

        bool initialize(int width, int height, const char* title);
        void cleanup();
        bool shouldClose();
        void pollEvents();
        void* getWindowHandle() const { return static_cast<void*>(window); }

        // Getters
        int getWidth() const { return width; }
        int getHeight() const { return height; }
        
        // Framebuffer resize flag
        bool framebufferResized = false;
        
        // Static callback function for framebuffer resize
        static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

    private:
        GLFWwindow* window;
        int width;
        int height;
        const char* title;
    };
}