#pragma once

#include "Inventory.h"
#include <string>
#include <memory>

namespace Sparky {
    // Forward declaration
    class Gun;
    
    class WeaponItem : public Item {
    public:
        WeaponItem(const std::string& name, std::unique_ptr<Gun> weapon);
        virtual ~WeaponItem();

        virtual bool isStackable() const override;
        virtual std::unique_ptr<Item> clone() const override;
        
        // Weapon-specific methods
        Gun* getWeapon() const { return weapon.get(); }
        std::unique_ptr<Gun> releaseWeapon() { return std::move(weapon); }
        
        int getAmmo() const;
        int getMagazineSize() const;
        int getTotalAmmo() const;
        float getDamage() const;
        const std::string& getWeaponType() const { return weaponType; }
        
        void setAmmo(int ammo);
        void setTotalAmmo(int ammo);
        
        // Weapon stats
        int getShotsFired() const { return shotsFired; }
        int getShotsHit() const { return shotsHit; }
        float getAccuracy() const { return accuracy; }
        
        void addShot(bool hit);
        void updateAccuracy();

    private:
        std::unique_ptr<Gun> weapon;
        std::string weaponType;
        
        // Weapon statistics
        int shotsFired;
        int shotsHit;
        float accuracy;
    };
}