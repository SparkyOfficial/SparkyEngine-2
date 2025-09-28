#include "../include/ScriptComponent.h"
#include "../include/Logger.h"

namespace Sparky {

    ScriptComponent::ScriptComponent() : started(false) {
        SPARKY_LOG_DEBUG("Created script component");
    }

    ScriptComponent::~ScriptComponent() {
        if (onDestroyCallback) {
            onDestroyCallback();
        }
    }

    void ScriptComponent::update(float deltaTime) {
        // Call start callback on first update
        if (!started) {
            if (onStartCallback) {
                onStartCallback();
            }
            started = true;
        }

        // Call update callback
        if (onUpdateCallback) {
            onUpdateCallback(deltaTime);
        }
    }

    void ScriptComponent::render() {
        // Script components typically don't have rendering logic
        // But we'll leave this here for flexibility
    }

    void ScriptComponent::setProperty(const std::string& name, const std::any& value) {
        properties[name] = value;
        SPARKY_LOG_DEBUG("Set script property: " + name);
    }

    void ScriptComponent::setOnStartCallback(std::function<void()> callback) {
        onStartCallback = callback;
    }

    void ScriptComponent::setOnUpdateCallback(std::function<void(float)> callback) {
        onUpdateCallback = callback;
    }

    void ScriptComponent::setOnDestroyCallback(std::function<void()> callback) {
        onDestroyCallback = callback;
    }

    void ScriptComponent::sendMessage(const std::string& message, const std::any& data) {
        auto it = messageHandlers.find(message);
        if (it != messageHandlers.end()) {
            it->second(data);
            SPARKY_LOG_DEBUG("Sent message: " + message);
        }
    }

    void ScriptComponent::addMessageHandler(const std::string& message, std::function<void(const std::any&)> handler) {
        messageHandlers[message] = handler;
        SPARKY_LOG_DEBUG("Added message handler: " + message);
    }
}