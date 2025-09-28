#include "../include/StateMachine.h"
#include "../include/Logger.h"

namespace Sparky {

    State::State(const std::string& name) : name(name) {
    }

    State::~State() {
    }

    StateMachine::StateMachine() : currentState(nullptr), previousState(nullptr) {
    }

    StateMachine::~StateMachine() {
    }

    void StateMachine::addState(std::unique_ptr<State> state) {
        const std::string& name = state->getName();
        states[name] = std::move(state);
        SPARKY_LOG_DEBUG("Added state: " + name);
    }

    void StateMachine::setState(const std::string& stateName) {
        auto it = states.find(stateName);
        if (it == states.end()) {
            SPARKY_LOG_WARNING("State not found: " + stateName);
            return;
        }

        // Exit current state
        if (currentState) {
            SPARKY_LOG_DEBUG("Exiting state: " + currentState->getName());
            currentState->onExit();
        }

        // Set new state
        previousState = currentState;
        currentState = it->second.get();

        // Enter new state
        SPARKY_LOG_DEBUG("Entering state: " + currentState->getName());
        currentState->onEnter();
    }

    void StateMachine::update(float deltaTime) {
        if (currentState) {
            currentState->onUpdate(deltaTime);
        }
    }

    bool StateMachine::hasState(const std::string& stateName) const {
        return states.find(stateName) != states.end();
    }
}