#include "../include/WeaponItem.h"
#include "../include/Gun.h"
#include "../include/Logger.h"

namespace Sparky {

    WeaponItem::WeaponItem(const std::string& name, std::unique_ptr<Gun> weapon) : 
        Item(name, 1), weapon(std::move(weapon)), weaponType("unknown"),
        shotsFired(0), shotsHit(0), accuracy(0.0f) {
        
        if (this->weapon) {
            // Get weapon type from the gun
            this->weaponType = this->weapon->getWeaponType() ? this->weapon->getWeaponType() : "unknown";
        }
        
        Logger::getInstance().info("Created weapon item: " + name + " (" + weaponType + ")");
    }

    WeaponItem::~WeaponItem() {
    }

    bool WeaponItem::isStackable() const {
        return false; // Weapons are not stackable
    }

    std::unique_ptr<Item> WeaponItem::clone() const {
        // Weapons can't be cloned in the traditional sense
        // In a real implementation, we would create a new weapon with the same properties
        Logger::getInstance().warning("Attempted to clone weapon item: " + getName());
        return std::make_unique<WeaponItem>(getName(), nullptr);
    }
    
    int WeaponItem::getAmmo() const {
        if (weapon) {
            return weapon->getAmmo();
        }
        return 0;
    }
    
    int WeaponItem::getMagazineSize() const {
        if (weapon) {
            return weapon->getMagazineSize();
        }
        return 0;
    }
    
    int WeaponItem::getTotalAmmo() const {
        if (weapon) {
            return weapon->getTotalAmmo();
        }
        return 0;
    }
    
    float WeaponItem::getDamage() const {
        if (weapon) {
            return weapon->getDamage();
        }
        return 0.0f;
    }
    
    void WeaponItem::setAmmo(int ammo) {
        if (weapon) {
            weapon->setAmmo(ammo);
        }
    }
    
    void WeaponItem::setTotalAmmo(int ammo) {
        if (weapon) {
            weapon->setTotalAmmo(ammo);
        }
    }
    
    void WeaponItem::addShot(bool hit) {
        shotsFired++;
        if (hit) {
            shotsHit++;
        }
        updateAccuracy();
    }
    
    void WeaponItem::updateAccuracy() {
        if (shotsFired > 0) {
            accuracy = static_cast<float>(shotsHit) / static_cast<float>(shotsFired) * 100.0f;
        } else {
            accuracy = 0.0f;
        }
    }
}