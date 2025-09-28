#include "../include/EventSystem.h"
#include "../include/Logger.h"

namespace Sparky {

    EventSystem::EventSystem() {
    }

    EventSystem::~EventSystem() {
    }

    EventSystem& EventSystem::getInstance() {
        static EventSystem instance;
        return instance;
    }

    void EventSystem::subscribe(EventType type, const EventCallback& callback) {
        eventCallbacks[type].push_back(callback);
        SPARKY_LOG_DEBUG("Subscribed to event type: " + std::to_string(static_cast<int>(type)));
    }

    void EventSystem::subscribe(const std::string& eventName, const EventCallback& callback) {
        namedEventCallbacks[eventName].push_back(callback);
        SPARKY_LOG_DEBUG("Subscribed to named event: " + eventName);
    }

    void EventSystem::unsubscribe(EventType type, const EventCallback& callback) {
        auto it = eventCallbacks.find(type);
        if (it != eventCallbacks.end()) {
            auto& callbacks = it->second;
            callbacks.erase(
                std::remove_if(callbacks.begin(), callbacks.end(),
                    [&callback](const EventCallback& cb) { return cb.target_type() == callback.target_type(); }),
                callbacks.end()
            );
            SPARKY_LOG_DEBUG("Unsubscribed from event type: " + std::to_string(static_cast<int>(type)));
        }
    }

    void EventSystem::unsubscribe(const std::string& eventName, const EventCallback& callback) {
        auto it = namedEventCallbacks.find(eventName);
        if (it != namedEventCallbacks.end()) {
            auto& callbacks = it->second;
            callbacks.erase(
                std::remove_if(callbacks.begin(), callbacks.end(),
                    [&callback](const EventCallback& cb) { return cb.target_type() == callback.target_type(); }),
                callbacks.end()
            );
            SPARKY_LOG_DEBUG("Unsubscribed from named event: " + eventName);
        }
    }

    void EventSystem::publish(const Event& event) {
        // Publish to type-based callbacks
        auto it = eventCallbacks.find(event.type);
        if (it != eventCallbacks.end()) {
            for (const auto& callback : it->second) {
                callback(event);
            }
        }

        // Publish to name-based callbacks
        auto namedIt = namedEventCallbacks.find(event.name);
        if (namedIt != namedEventCallbacks.end()) {
            for (const auto& callback : namedIt->second) {
                callback(event);
            }
        }
    }

    void EventSystem::publish(EventType type) {
        Event event;
        event.type = type;
        publish(event);
    }

    void EventSystem::publish(const std::string& eventName) {
        Event event;
        event.type = EventType::CUSTOM;
        event.name = eventName;
        publish(event);
    }

    void EventSystem::publishWindowResize(int width, int height) {
        Event event;
        event.type = EventType::WINDOW_RESIZE;
        event.intParams["width"] = width;
        event.intParams["height"] = height;
        publish(event);
    }

    void EventSystem::publishKeyPressed(int key) {
        Event event;
        event.type = EventType::KEY_PRESSED;
        event.intParams["key"] = key;
        publish(event);
    }

    void EventSystem::publishMouseMoved(float x, float y) {
        Event event;
        event.type = EventType::MOUSE_MOVED;
        event.floatParams["x"] = x;
        event.floatParams["y"] = y;
        publish(event);
    }
}