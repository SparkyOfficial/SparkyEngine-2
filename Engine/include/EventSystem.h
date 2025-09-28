#pragma once

#include <functional>
#include <unordered_map>
#include <vector>
#include <string>

namespace Sparky {
    enum class EventType {
        WINDOW_CLOSE,
        WINDOW_RESIZE,
        KEY_PRESSED,
        KEY_RELEASED,
        MOUSE_MOVED,
        MOUSE_PRESSED,
        MOUSE_RELEASED,
        COLLISION,
        CUSTOM
    };

    struct Event {
        EventType type;
        std::string name;
        std::unordered_map<std::string, int> intParams;
        std::unordered_map<std::string, float> floatParams;
        std::unordered_map<std::string, std::string> stringParams;
    };

    class EventSystem {
    public:
        static EventSystem& getInstance();

        using EventCallback = std::function<void(const Event&)>;

        void subscribe(EventType type, const EventCallback& callback);
        void subscribe(const std::string& eventName, const EventCallback& callback);
        void unsubscribe(EventType type, const EventCallback& callback);
        void unsubscribe(const std::string& eventName, const EventCallback& callback);

        void publish(const Event& event);
        void publish(EventType type);
        void publish(const std::string& eventName);

        // Helper methods for common events
        void publishWindowResize(int width, int height);
        void publishKeyPressed(int key);
        void publishMouseMoved(float x, float y);

    private:
        EventSystem();
        ~EventSystem();

        std::unordered_map<EventType, std::vector<EventCallback>> eventCallbacks;
        std::unordered_map<std::string, std::vector<EventCallback>> namedEventCallbacks;
    };
}