#pragma once
#include <GLFW/glfw3.h>

namespace Sparky {
    class InputManager {
    public:
        InputManager();
        ~InputManager();

        void initialize(GLFWwindow* window);
        void update();
        bool isKeyPressed(int key);
        bool isKeyJustPressed(int key);
        bool isMouseButtonPressed(int button);
        bool isMouseButtonJustPressed(int button);

        // Mouse position
        float getMouseX() const { return mouseX; }
        float getMouseY() const { return mouseY; }
        void setMousePosition(float x, float y);

        // Callbacks
        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
        static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

    private:
        static const int KEY_COUNT = 512;
        static const int MOUSE_BUTTON_COUNT = 8;

        bool keys[KEY_COUNT];
        bool prevKeys[KEY_COUNT];
        bool mouseButtons[MOUSE_BUTTON_COUNT];
        bool prevMouseButtons[MOUSE_BUTTON_COUNT];
        float mouseX, mouseY;
        float prevMouseX, prevMouseY;
        GLFWwindow* window;
        
        static InputManager* instance;
    };
}