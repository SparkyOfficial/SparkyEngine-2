#include "../include/InteractiveObject.h"
#include "../include/Player.h"
#include "../include/Logger.h"

namespace Sparky {

    InteractiveObject::InteractiveObject(const std::string& name) : name(name), enabled(true) {
    }

    InteractiveObject::~InteractiveObject() {
    }

    void InteractiveObject::update(float deltaTime) {
        // Add any interactive object-specific update logic here
    }

    void InteractiveObject::render() {
        // Add any interactive object-specific rendering logic here
    }
    
    bool InteractiveObject::canInteract(Player* player) const {
        if (!enabled || !player) {
            return false;
        }
        
        // For now, we'll just return true since we don't have position data
        // In a full implementation, we would check distance to player
        return true;
    }
    
    void InteractiveObject::onInteract(Player* player) {
        if (!canInteract(player)) {
            return;
        }
        
        Logger::getInstance().info("Interacted with " + name);
        
        // Call callback if set
        if (onInteractCallback) {
            onInteractCallback(player);
        }
    }
    
    // Door implementation
    Door::Door(bool locked) : InteractiveObject("Door"), locked(locked), open(false) {
    }

    Door::~Door() {
    }
    
    void Door::onInteract(Player* player) {
        if (!canInteract(player)) {
            return;
        }
        
        if (locked) {
            Logger::getInstance().info("The door is locked.");
            return;
        }
        
        open = !open;
        Logger::getInstance().info("Door is now " + std::string(open ? "open" : "closed"));
    }
    
    // InteractiveButton implementation
    InteractiveButton::InteractiveButton() : InteractiveObject("Button"), pressed(false) {
    }

    InteractiveButton::~InteractiveButton() {
    }
    
    void InteractiveButton::onInteract(Player* player) {
        if (!canInteract(player)) {
            return;
        }
        
        pressed = !pressed;
        Logger::getInstance().info("Button " + std::string(pressed ? "pressed" : "released"));
        
        // Call callback if set
        if (onPressCallback) {
            onPressCallback();
        }
    }
}