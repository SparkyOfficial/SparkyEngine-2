#include "../include/WeaponSystem.h"
#include "../../Game/src/Gun.h"
#include "../include/WeaponItem.h"
#include "../include/Player.h"
#include "../include/Logger.h"
#include <algorithm>

namespace Sparky {

    WeaponSystem::WeaponSystem(Player* player) : 
        player(player), inventory(nullptr), currentWeapon(nullptr), 
        currentWeaponIndex(-1) {
        
        // Initialize quick slots
        for (int i = 0; i < 10; i++) {
            quickSlots[i] = -1; // -1 means empty slot
        }
        
        Logger::getInstance().info("WeaponSystem created");
    }

    WeaponSystem::~WeaponSystem() {
        Logger::getInstance().info("WeaponSystem destroyed");
    }
    
    void WeaponSystem::setInventory(Inventory* inventory) {
        this->inventory = inventory;
        updateEquippedWeaponsList();
        Logger::getInstance().info("WeaponSystem inventory set");
    }
    
    bool WeaponSystem::equipWeapon(int inventorySlot) {
        if (!inventory || !isValidWeaponSlot(inventorySlot)) {
            Logger::getInstance().warning("Cannot equip weapon: Invalid inventory slot " + std::to_string(inventorySlot));
            return false;
        }
        
        // Get item from inventory
        Item* item = inventory->getItemAt(inventorySlot);
        if (!item) {
            Logger::getInstance().warning("Cannot equip weapon: No item in slot " + std::to_string(inventorySlot));
            return false;
        }
        
        // Check if it's a weapon item
        WeaponItem* weaponItem = dynamic_cast<WeaponItem*>(item);
        if (!weaponItem) {
            Logger::getInstance().warning("Cannot equip weapon: Item is not a weapon");
            return false;
        }
        
        // Unequip current weapon if any
        if (currentWeapon) {
            unequipWeapon();
        }
        
        // Equip new weapon
        currentWeapon = weaponItem->getWeapon();
        currentWeaponIndex = inventorySlot;
        
        // Set player's camera to the weapon
        // In a full implementation, we would get the player's camera
        // currentWeapon->setCamera(player->getCamera());
        
        onWeaponEquipped();
        Logger::getInstance().info("Equipped weapon: " + item->getName());
        return true;
    }
    
    bool WeaponSystem::equipWeapon(const std::string& weaponName) {
        if (!inventory) {
            return false;
        }
        
        // Find weapon in inventory
        for (int i = 0; i < inventory->getSize(); i++) {
            Item* item = inventory->getItemAt(i);
            if (item && item->getName() == weaponName) {
                return equipWeapon(i);
            }
        }
        
        Logger::getInstance().warning("Cannot equip weapon: Weapon not found in inventory - " + weaponName);
        return false;
    }
    
    bool WeaponSystem::unequipWeapon() {
        if (!currentWeapon) {
            return false;
        }
        
        // In a full implementation, we would return the weapon to the inventory
        // For now, we'll just clear the reference
        currentWeapon = nullptr;
        currentWeaponIndex = -1;
        
        onWeaponUnequipped();
        Logger::getInstance().info("Unequipped current weapon");
        return true;
    }
    
    bool WeaponSystem::switchToNextWeapon() {
        if (equippedWeaponSlots.empty()) {
            Logger::getInstance().warning("Cannot switch weapon: No weapons equipped");
            return false;
        }
        
        if (equippedWeaponSlots.size() == 1) {
            // Only one weapon, nothing to switch to
            return false;
        }
        
        // Find current weapon in the list
        auto it = std::find(equippedWeaponSlots.begin(), equippedWeaponSlots.end(), currentWeaponIndex);
        if (it == equippedWeaponSlots.end()) {
            // Current weapon not in list, equip first weapon
            return equipWeapon(equippedWeaponSlots[0]);
        }
        
        // Move to next weapon
        ++it;
        if (it == equippedWeaponSlots.end()) {
            // Wrap around to first weapon
            it = equippedWeaponSlots.begin();
        }
        
        return equipWeapon(*it);
    }
    
    bool WeaponSystem::switchToPreviousWeapon() {
        if (equippedWeaponSlots.empty()) {
            Logger::getInstance().warning("Cannot switch weapon: No weapons equipped");
            return false;
        }
        
        if (equippedWeaponSlots.size() == 1) {
            // Only one weapon, nothing to switch to
            return false;
        }
        
        // Find current weapon in the list
        auto it = std::find(equippedWeaponSlots.begin(), equippedWeaponSlots.end(), currentWeaponIndex);
        if (it == equippedWeaponSlots.end()) {
            // Current weapon not in list, equip first weapon
            return equipWeapon(equippedWeaponSlots[0]);
        }
        
        // Move to previous weapon
        if (it == equippedWeaponSlots.begin()) {
            // Wrap around to last weapon
            it = equippedWeaponSlots.end() - 1;
        } else {
            --it;
        }
        
        return equipWeapon(*it);
    }
    
    bool WeaponSystem::switchToWeapon(int index) {
        if (index < 0 || index >= static_cast<int>(equippedWeaponSlots.size())) {
            Logger::getInstance().warning("Cannot switch to weapon: Invalid index " + std::to_string(index));
            return false;
        }
        
        return equipWeapon(equippedWeaponSlots[index]);
    }
    
    void WeaponSystem::update(float deltaTime) {
        if (currentWeapon) {
            currentWeapon->update(deltaTime);
        }
    }
    
    void WeaponSystem::shoot() {
        if (currentWeapon && canShoot()) {
            currentWeapon->shoot();
        }
    }
    
    void WeaponSystem::reload() {
        if (currentWeapon) {
            currentWeapon->reload();
        }
    }
    
    void WeaponSystem::aim() {
        if (currentWeapon) {
            currentWeapon->aim();
        }
    }
    
    void WeaponSystem::unaim() {
        if (currentWeapon) {
            currentWeapon->unaim();
        }
    }
    
    int WeaponSystem::getTotalAmmo() const {
        if (currentWeapon) {
            return currentWeapon->getTotalAmmo();
        }
        return 0;
    }
    
    int WeaponSystem::getMagazineAmmo() const {
        if (currentWeapon) {
            return currentWeapon->getAmmo();
        }
        return 0;
    }
    
    bool WeaponSystem::canShoot() const {
        if (!currentWeapon) {
            return false;
        }
        return currentWeapon->canShoot();
    }
    
    bool WeaponSystem::isReloading() const {
        if (!currentWeapon) {
            return false;
        }
        // In a full implementation, we would check if the weapon is reloading
        return false;
    }
    
    void WeaponSystem::setQuickSlot(int slot, int inventoryIndex) {
        if (slot >= 0 && slot < 10) {
            quickSlots[slot] = inventoryIndex;
            Logger::getInstance().info("Set quick slot " + std::to_string(slot) + " to inventory index " + std::to_string(inventoryIndex));
        }
    }
    
    int WeaponSystem::getQuickSlot(int slot) const {
        if (slot >= 0 && slot < 10) {
            return quickSlots[slot];
        }
        return -1;
    }
    
    void WeaponSystem::useQuickSlot(int slot) {
        if (slot >= 0 && slot < 10) {
            int inventoryIndex = quickSlots[slot];
            if (inventoryIndex >= 0) {
                equipWeapon(inventoryIndex);
            }
        }
    }
    
    void WeaponSystem::addWeaponSlot(int slotIndex) {
        if (isValidWeaponSlot(slotIndex)) {
            // Check if already in list
            auto it = std::find(equippedWeaponSlots.begin(), equippedWeaponSlots.end(), slotIndex);
            if (it == equippedWeaponSlots.end()) {
                equippedWeaponSlots.push_back(slotIndex);
                Logger::getInstance().info("Added weapon slot: " + std::to_string(slotIndex));
            }
        }
    }
    
    void WeaponSystem::removeWeaponSlot(int slotIndex) {
        auto it = std::find(equippedWeaponSlots.begin(), equippedWeaponSlots.end(), slotIndex);
        if (it != equippedWeaponSlots.end()) {
            equippedWeaponSlots.erase(it);
            Logger::getInstance().info("Removed weapon slot: " + std::to_string(slotIndex));
            
            // If this was the current weapon, unequip it
            if (currentWeaponIndex == slotIndex) {
                unequipWeapon();
            }
        }
    }
    
    bool WeaponSystem::isValidWeaponSlot(int slotIndex) const {
        if (!inventory || slotIndex < 0 || slotIndex >= inventory->getSize()) {
            return false;
        }
        
        Item* item = inventory->getItemAt(slotIndex);
        if (!item) {
            return false;
        }
        
        // Check if it's a weapon item
        WeaponItem* weaponItem = dynamic_cast<WeaponItem*>(item);
        return weaponItem != nullptr;
    }
    
    void WeaponSystem::updateEquippedWeaponsList() {
        equippedWeaponSlots.clear();
        
        if (!inventory) {
            return;
        }
        
        // Scan inventory for weapons
        for (int i = 0; i < inventory->getSize(); i++) {
            if (isValidWeaponSlot(i)) {
                equippedWeaponSlots.push_back(i);
            }
        }
        
        Logger::getInstance().info("Updated equipped weapons list: " + std::to_string(equippedWeaponSlots.size()) + " weapons found");
    }
    
    void WeaponSystem::onWeaponEquipped() {
        // Notify systems that a weapon has been equipped
        Logger::getInstance().info("Weapon equipped event");
    }
    
    void WeaponSystem::onWeaponUnequipped() {
        // Notify systems that a weapon has been unequipped
        Logger::getInstance().info("Weapon unequipped event");
    }
}