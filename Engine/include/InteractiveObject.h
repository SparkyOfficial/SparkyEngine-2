#pragma once

#include <string>
#include <functional>

namespace Sparky {
    class Player;
    
    class InteractiveObject {
    public:
        InteractiveObject(const std::string& name = "InteractiveObject");
        virtual ~InteractiveObject();

        virtual void update(float deltaTime);
        virtual void render();
        
        // Interaction functionality
        virtual void onInteract(Player* player);
        virtual bool canInteract(Player* player) const;
        
        // Getters and setters
        const std::string& getName() const { return name; }
        void setName(const std::string& objectName) { name = objectName; }
        
        bool isEnabled() const { return enabled; }
        void setEnabled(bool isEnabled) { enabled = isEnabled; }
        
        // Callback for interaction
        void setOnInteractCallback(std::function<void(Player*)> callback) { onInteractCallback = callback; }

    protected:
        std::string name;
        bool enabled;
        std::function<void(Player*)> onInteractCallback;
    };
    
    // Specific interactive object types
    class Door : public InteractiveObject {
    public:
        Door(bool locked = false);
        virtual ~Door();
        
        virtual void onInteract(Player* player) override;
        
        bool isLocked() const { return locked; }
        void setLocked(bool isLocked) { locked = isLocked; }
        
        bool isOpen() const { return open; }
        void setOpen(bool isOpen) { open = isOpen; }
        
    private:
        bool locked;
        bool open;
    };
    
    class Button : public InteractiveObject {
    public:
        Button();
        virtual ~Button();
        
        virtual void onInteract(Player* player) override;
        
        bool isPressed() const { return pressed; }
        void setPressed(bool isPressed) { pressed = isPressed; }
        
        // Callback for button press
        void setOnPressCallback(std::function<void()> callback) { onPressCallback = callback; }
        
    private:
        bool pressed;
        std::function<void()> onPressCallback;
    };
}