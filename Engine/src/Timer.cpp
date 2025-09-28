#include "../include/Timer.h"
#include <GLFW/glfw3.h>

namespace Sparky {

    Timer::Timer() : startTime(0.0f), elapsedTime(0.0f), running(false) {
    }

    void Timer::start() {
        if (!running) {
            startTime = static_cast<float>(glfwGetTime());
            running = true;
        }
    }

    void Timer::stop() {
        if (running) {
            elapsedTime += static_cast<float>(glfwGetTime()) - startTime;
            running = false;
        }
    }

    void Timer::reset() {
        startTime = static_cast<float>(glfwGetTime());
        elapsedTime = 0.0f;
    }

    float Timer::getElapsedTime() const {
        if (running) {
            return elapsedTime + (static_cast<float>(glfwGetTime()) - startTime);
        }
        return elapsedTime;
    }

    bool Timer::isRunning() const {
        return running;
    }
}