#pragma once

#include "Component.h"
#include <string>
#include <unordered_map>
#include <any>
#include <functional>

namespace Sparky {
    class ScriptComponent : public Component {
    public:
        ScriptComponent();
        virtual ~ScriptComponent();

        // Component interface
        void update(float deltaTime) override;
        void render() override;

        // Script properties
        void setProperty(const std::string& name, const std::any& value);
        template<typename T>
        T getProperty(const std::string& name, const T& defaultValue = T{}) const;

        // Script events
        void setOnStartCallback(std::function<void()> callback);
        void setOnUpdateCallback(std::function<void(float)> callback);
        void setOnDestroyCallback(std::function<void()> callback);

        // Message passing
        void sendMessage(const std::string& message, const std::any& data = std::any{});
        void addMessageHandler(const std::string& message, std::function<void(const std::any&)> handler);

    private:
        std::unordered_map<std::string, std::any> properties;
        std::unordered_map<std::string, std::function<void(const std::any&)>> messageHandlers;
        
        std::function<void()> onStartCallback;
        std::function<void(float)> onUpdateCallback;
        std::function<void()> onDestroyCallback;
        
        bool started;
    };

    // Template implementation
    template<typename T>
    T ScriptComponent::getProperty(const std::string& name, const T& defaultValue) const {
        auto it = properties.find(name);
        if (it != properties.end()) {
            try {
                return std::any_cast<T>(it->second);
            } catch (const std::bad_any_cast& e) {
                // Return default value if type doesn't match
            }
        }
        return defaultValue;
    }
}