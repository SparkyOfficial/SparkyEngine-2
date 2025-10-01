#pragma once

#ifdef HAS_GLFW
#include <GLFW/glfw3.h>
#else
typedef void GLFWwindow;
const int GLFW_KEY_SPACE = 32;
const int GLFW_KEY_APOSTROPHE = 39;
const int GLFW_KEY_COMMA = 44;
const int GLFW_KEY_MINUS = 45;
const int GLFW_KEY_PERIOD = 46;
const int GLFW_KEY_SLASH = 47;
const int GLFW_KEY_0 = 48;
const int GLFW_KEY_1 = 49;
const int GLFW_KEY_2 = 50;
const int GLFW_KEY_3 = 51;
const int GLFW_KEY_4 = 52;
const int GLFW_KEY_5 = 53;
const int GLFW_KEY_6 = 54;
const int GLFW_KEY_7 = 55;
const int GLFW_KEY_8 = 56;
const int GLFW_KEY_9 = 57;
const int GLFW_KEY_SEMICOLON = 59;
const int GLFW_KEY_EQUAL = 61;
const int GLFW_KEY_A = 65;
const int GLFW_KEY_B = 66;
const int GLFW_KEY_C = 67;
const int GLFW_KEY_D = 68;
const int GLFW_KEY_E = 69;
const int GLFW_KEY_F = 70;
const int GLFW_KEY_G = 71;
const int GLFW_KEY_H = 72;
const int GLFW_KEY_I = 73;
const int GLFW_KEY_J = 74;
const int GLFW_KEY_K = 75;
const int GLFW_KEY_L = 76;
const int GLFW_KEY_M = 77;
const int GLFW_KEY_N = 78;
const int GLFW_KEY_O = 79;
const int GLFW_KEY_P = 80;
const int GLFW_KEY_Q = 81;
const int GLFW_KEY_R = 82;
const int GLFW_KEY_S = 83;
const int GLFW_KEY_T = 84;
const int GLFW_KEY_U = 85;
const int GLFW_KEY_V = 86;
const int GLFW_KEY_W = 87;
const int GLFW_KEY_X = 88;
const int GLFW_KEY_Y = 89;
const int GLFW_KEY_Z = 90;
const int GLFW_KEY_LEFT_BRACKET = 91;
const int GLFW_KEY_BACKSLASH = 92;
const int GLFW_KEY_RIGHT_BRACKET = 93;
const int GLFW_KEY_GRAVE_ACCENT = 96;
const int GLFW_KEY_ESCAPE = 256;
const int GLFW_KEY_ENTER = 257;
const int GLFW_KEY_TAB = 258;
const int GLFW_KEY_BACKSPACE = 259;
const int GLFW_KEY_INSERT = 260;
const int GLFW_KEY_DELETE = 261;
const int GLFW_KEY_RIGHT = 262;
const int GLFW_KEY_LEFT = 263;
const int GLFW_KEY_DOWN = 264;
const int GLFW_KEY_UP = 265;
const int GLFW_KEY_PAGE_UP = 266;
const int GLFW_KEY_PAGE_DOWN = 267;
const int GLFW_KEY_HOME = 268;
const int GLFW_KEY_END = 269;
const int GLFW_KEY_CAPS_LOCK = 280;
const int GLFW_KEY_SCROLL_LOCK = 281;
const int GLFW_KEY_NUM_LOCK = 282;
const int GLFW_KEY_PRINT_SCREEN = 283;
const int GLFW_KEY_PAUSE = 284;
const int GLFW_KEY_F1 = 290;
const int GLFW_KEY_F2 = 291;
const int GLFW_KEY_F3 = 292;
const int GLFW_KEY_F4 = 293;
const int GLFW_KEY_F5 = 294;
const int GLFW_KEY_F6 = 295;
const int GLFW_KEY_F7 = 296;
const int GLFW_KEY_F8 = 297;
const int GLFW_KEY_F9 = 298;
const int GLFW_KEY_F10 = 299;
const int GLFW_KEY_F11 = 300;
const int GLFW_KEY_F12 = 301;
const int GLFW_KEY_F13 = 302;
const int GLFW_KEY_F14 = 303;
const int GLFW_KEY_F15 = 304;
const int GLFW_KEY_F16 = 305;
const int GLFW_KEY_F17 = 306;
const int GLFW_KEY_F18 = 307;
const int GLFW_KEY_F19 = 308;
const int GLFW_KEY_F20 = 309;
const int GLFW_KEY_F21 = 310;
const int GLFW_KEY_F22 = 311;
const int GLFW_KEY_F23 = 312;
const int GLFW_KEY_F24 = 313;
const int GLFW_KEY_F25 = 314;
const int GLFW_KEY_KP_0 = 320;
const int GLFW_KEY_KP_1 = 321;
const int GLFW_KEY_KP_2 = 322;
const int GLFW_KEY_KP_3 = 323;
const int GLFW_KEY_KP_4 = 324;
const int GLFW_KEY_KP_5 = 325;
const int GLFW_KEY_KP_6 = 326;
const int GLFW_KEY_KP_7 = 327;
const int GLFW_KEY_KP_8 = 328;
const int GLFW_KEY_KP_9 = 329;
const int GLFW_KEY_KP_DECIMAL = 330;
const int GLFW_KEY_KP_DIVIDE = 331;
const int GLFW_KEY_KP_MULTIPLY = 332;
const int GLFW_KEY_KP_SUBTRACT = 333;
const int GLFW_KEY_KP_ADD = 334;
const int GLFW_KEY_KP_ENTER = 335;
const int GLFW_KEY_KP_EQUAL = 336;
const int GLFW_KEY_LEFT_SHIFT = 340;
const int GLFW_KEY_LEFT_CONTROL = 341;
const int GLFW_KEY_LEFT_ALT = 342;
const int GLFW_KEY_LEFT_SUPER = 343;
const int GLFW_KEY_RIGHT_SHIFT = 344;
const int GLFW_KEY_RIGHT_CONTROL = 345;
const int GLFW_KEY_RIGHT_ALT = 346;
const int GLFW_KEY_RIGHT_SUPER = 347;
const int GLFW_KEY_MENU = 348;
const int GLFW_ACTION_RELEASE = 0;
const int GLFW_ACTION_PRESS = 1;
const int GLFW_ACTION_REPEAT = 2;
#endif

#include <glm/glm.hpp>

namespace Sparky {
    class InputManager {
    public:
        static InputManager& getInstance();
        
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