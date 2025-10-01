#include "../include/Timer.h"

#ifdef HAS_GLFW
#include <GLFW/glfw3.h>
#endif

namespace Sparky {

    Timer::Timer() : startTime(0.0f), elapsedTime(0.0f), running(false) {
    }

    void Timer::start() {
        if (!running) {
#ifdef HAS_GLFW
            startTime = static_cast<float>(glfwGetTime());
#else
            // Fallback implementation
            startTime = 0.0f;
#endif
            running = true;
        }
    }

    void Timer::stop() {
        if (running) {
#ifdef HAS_GLFW
            elapsedTime += static_cast<float>(glfwGetTime()) - startTime;
#else
            // Fallback implementation
            elapsedTime += 0.016f; // Assume 60 FPS
#endif
            running = false;
        }
    }

    void Timer::reset() {
#ifdef HAS_GLFW
        startTime = static_cast<float>(glfwGetTime());
#else
        // Fallback implementation
        startTime = 0.0f;
#endif
        elapsedTime = 0.0f;
    }

    float Timer::getElapsedTime() const {
        if (running) {
#ifdef HAS_GLFW
            return elapsedTime + (static_cast<float>(glfwGetTime()) - startTime);
#else
            // Fallback implementation
            return elapsedTime + 0.016f; // Assume 60 FPS
#endif
        }
        return elapsedTime;
    }

    bool Timer::isRunning() const {
        return running;
    }
}