#include "../include/Pickup.h"
#include "../include/Player.h"
#include "../include/HealthComponent.h"
#include "../include/Logger.h"

namespace Sparky {

    Pickup::Pickup(const std::string& name) : name(name), pickedUp(false) {
    }

    Pickup::~Pickup() {
    }

    void Pickup::update(float deltaTime) {
        // Add any pickup-specific update logic here
    }

    void Pickup::render() {
        // Add any pickup-specific rendering logic here
    }
    
    bool Pickup::canPickup(Player* player) const {
        if (pickedUp || !player) {
            return false;
        }
        
        // For now, we'll just return true since we don't have position data
        // In a full implementation, we would check distance to player
        return true;
    }
    
    void Pickup::setItem(std::unique_ptr<Item> newItem) {
        item = std::move(newItem);
    }
    
    // HealthPickup implementation
    HealthPickup::HealthPickup(float healthAmount) : Pickup("HealthPickup"), healthAmount(healthAmount) {
    }

    HealthPickup::~HealthPickup() {
    }
    
    void HealthPickup::onPickup(Player* player) {
        if (!player || pickedUp) {
            return;
        }
        
        HealthComponent* healthComp = player->getHealthComponent();
        if (healthComp) {
            healthComp->heal(healthAmount);
            Logger::getInstance().info("Picked up health pack. Restored " + std::to_string(healthAmount) + " health.");
            pickedUp = true;
        }
    }
    
    // AmmoPickup implementation
    AmmoPickup::AmmoPickup(const std::string& ammoType, int ammoCount) : 
        Pickup("AmmoPickup"), ammoType(ammoType), ammoCount(ammoCount) {
    }

    AmmoPickup::~AmmoPickup() {
    }
    
    void AmmoPickup::onPickup(Player* player) {
        if (!player || pickedUp) {
            return;
        }
        
        // In a full implementation, we would add ammo to the player's inventory
        Logger::getInstance().info("Picked up " + std::to_string(ammoCount) + " " + ammoType + " ammo.");
        pickedUp = true;
    }
}