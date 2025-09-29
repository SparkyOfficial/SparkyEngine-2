#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

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

        // Mouse position and movement
        glm::vec2 getMousePosition() const { return glm::vec2(mouseX, mouseY); }
        glm::vec2 getMouseDelta() const { return glm::vec2(mouseX - prevMouseX, mouseY - prevMouseY); }
        void setMousePosition(float x, float y);

        // Cursor mode
        void setCursorMode(int mode); // GLFW_CURSOR_DISABLED, GLFW_CURSOR_HIDDEN, GLFW_CURSOR_NORMAL
        int getCursorMode() const { return cursorMode; }

        // Callbacks
        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
        static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

    private:
        static const int KEY_COUNT = 512;
        static const int MOUSE_BUTTON_COUNT = 8;

        bool keys[KEY_COUNT];
        bool prevKeys[KEY_COUNT];
        bool mouseButtons[MOUSE_BUTTON_COUNT];
        bool prevMouseButtons[MOUSE_BUTTON_COUNT];
        float mouseX, mouseY;
        float prevMouseX, prevMouseY;
        float scrollX, scrollY;
        float prevScrollX, prevScrollY;
        GLFWwindow* window;
        int cursorMode;
        
        static InputManager* instance;
    };
}