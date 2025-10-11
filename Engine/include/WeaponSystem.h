#pragma once

#include "Inventory.h"
#include <memory>
#include <vector>

namespace Sparky {
    // Forward declarations
    class Gun;
    class Player;
    
    class WeaponSystem {
    public:
        WeaponSystem(Player* player);
        ~WeaponSystem();
        
        // Weapon management
        bool equipWeapon(int inventorySlot);
        bool equipWeapon(const std::string& weaponName);
        bool unequipWeapon();
        bool switchToNextWeapon();
        bool switchToPreviousWeapon();
        bool switchToWeapon(int index);
        
        // Weapon usage
        void update(float deltaTime);
        void shoot();
        void reload();
        void aim();
        void unaim();
        
        // Inventory management
        void setInventory(Inventory* inventory);
        Inventory* getInventory() const { return inventory; }
        
        // Current weapon access
        Gun* getCurrentWeapon() const { return currentWeapon; }
        int getCurrentWeaponIndex() const { return currentWeaponIndex; }
        bool hasWeaponEquipped() const { return currentWeapon != nullptr; }
        
        // Weapon list management
        const std::vector<int>& getEquippedWeapons() const { return equippedWeaponSlots; }
        void addWeaponSlot(int slotIndex);
        void removeWeaponSlot(int slotIndex);
        
        // Weapon stats
        int getTotalAmmo() const;
        int getMagazineAmmo() const;
        bool canShoot() const;
        bool isReloading() const;
        
        // Quick weapon switching
        void setQuickSlot(int slot, int inventoryIndex);
        int getQuickSlot(int slot) const;
        void useQuickSlot(int slot);
        
    private:
        Player* player;
        Inventory* inventory;
        Gun* currentWeapon;
        int currentWeaponIndex;
        std::vector<int> equippedWeaponSlots; // Inventory slots that contain weapons
        int quickSlots[10]; // Slots 0-9 for quick weapon switching
        
        // Private helper methods
        bool isValidWeaponSlot(int slotIndex) const;
        void updateEquippedWeaponsList();
        void onWeaponEquipped();
        void onWeaponUnequipped();
    };
}