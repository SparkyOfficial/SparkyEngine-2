#include "../include/ExampleState.h"
#include "../include/Logger.h"
#include <iostream>

namespace Sparky {

    ExampleState::ExampleState(const std::string& name) : State(name), timer(0.0f) {
    }

    ExampleState::~ExampleState() {
    }

    void ExampleState::onEnter() {
        SPARKY_LOG_INFO("Entering state: " + getName());
        timer = 0.0f;
    }

    void ExampleState::onUpdate(float deltaTime) {
        timer += deltaTime;
        
        // Log every 5 seconds
        if (static_cast<int>(timer) % 5 == 0 && timer - static_cast<int>(timer) < deltaTime) {
            SPARKY_LOG_INFO("State " + getName() + " running for " + std::to_string(static_cast<int>(timer)) + " seconds");
        }
    }

    void ExampleState::onExit() {
        SPARKY_LOG_INFO("Exiting state: " + getName());
        SPARKY_LOG_INFO("State " + getName() + " ran for " + std::to_string(timer) + " seconds");
    }
}