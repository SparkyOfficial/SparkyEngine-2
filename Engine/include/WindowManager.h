#pragma once

struct GLFWwindow;

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

    private:
        GLFWwindow* window;
        int width;
        int height;
        const char* title;
    };
}