#include "../include/InputManager.h"
#include <cstring>

namespace Sparky {
    
    InputManager* InputManager::instance = nullptr;

    InputManager::InputManager() : mouseX(0), mouseY(0), prevMouseX(0), prevMouseY(0), 
                                 scrollX(0), scrollY(0), prevScrollX(0), prevScrollY(0),
                                 window(nullptr), cursorMode(GLFW_CURSOR_NORMAL) {
        memset(keys, 0, sizeof(keys));
        memset(prevKeys, 0, sizeof(prevKeys));
        memset(mouseButtons, 0, sizeof(mouseButtons));
        memset(prevMouseButtons, 0, sizeof(prevMouseButtons));
        instance = this;
    }

    InputManager::~InputManager() {
        instance = nullptr;
    }

    void InputManager::initialize(GLFWwindow* window) {
        this->window = window;
        
        // Set callbacks
        glfwSetKeyCallback(window, keyCallback);
        glfwSetCursorPosCallback(window, mouseCallback);
        glfwSetMouseButtonCallback(window, mouseButtonCallback);
        glfwSetScrollCallback(window, scrollCallback);
    }

    void InputManager::update() {
        // Copy current state to previous state
        memcpy(prevKeys, keys, sizeof(keys));
        memcpy(prevMouseButtons, mouseButtons, sizeof(mouseButtons));
        prevMouseX = mouseX;
        prevMouseY = mouseY;
        prevScrollX = scrollX;
        prevScrollY = scrollY;
    }

    bool InputManager::isKeyPressed(int key) {
        if (key >= 0 && key < KEY_COUNT) {
            return keys[key];
        }
        return false;
    }

    bool InputManager::isKeyJustPressed(int key) {
        if (key >= 0 && key < KEY_COUNT) {
            return keys[key] && !prevKeys[key];
        }
        return false;
    }

    bool InputManager::isMouseButtonPressed(int button) {
        if (button >= 0 && button < MOUSE_BUTTON_COUNT) {
            return mouseButtons[button];
        }
        return false;
    }

    bool InputManager::isMouseButtonJustPressed(int button) {
        if (button >= 0 && button < MOUSE_BUTTON_COUNT) {
            return mouseButtons[button] && !prevMouseButtons[button];
        }
        return false;
    }

    void InputManager::setMousePosition(float x, float y) {
        mouseX = x;
        mouseY = y;
        if (window) {
            glfwSetCursorPos(window, x, y);
        }
    }

    void InputManager::setCursorMode(int mode) {
        cursorMode = mode;
        if (window) {
            glfwSetInputMode(window, GLFW_CURSOR, mode);
        }
    }

    void InputManager::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (instance && key >= 0 && key < KEY_COUNT) {
            if (action == GLFW_PRESS) {
                instance->keys[key] = true;
            } else if (action == GLFW_RELEASE) {
                instance->keys[key] = false;
            }
        }
    }

    void InputManager::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
        if (instance) {
            instance->mouseX = static_cast<float>(xpos);
            instance->mouseY = static_cast<float>(ypos);
        }
    }

    void InputManager::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
        if (instance && button >= 0 && button < MOUSE_BUTTON_COUNT) {
            if (action == GLFW_PRESS) {
                instance->mouseButtons[button] = true;
            } else if (action == GLFW_RELEASE) {
                instance->mouseButtons[button] = false;
            }
        }
    }

    void InputManager::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
        if (instance) {
            instance->scrollX += static_cast<float>(xoffset);
            instance->scrollY += static_cast<float>(yoffset);
        }
    }
}