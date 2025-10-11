#pragma once

#include "Inventory.h"
#include <string>

namespace Sparky {
    
    enum class AmmoType {
        REGULAR = 0,
        ARMOR_PIERCING = 1,
        EXPLOSIVE = 2,
        INCENDIARY = 3
    };
    
    class AmmoItem : public Item {
    public:
        AmmoItem(const std::string& name, AmmoType type, int quantity = 30, int maxStack = 100);
        virtual ~AmmoItem();

        virtual bool isStackable() const override;
        virtual std::unique_ptr<Item> clone() const override;
        
        // Ammo-specific methods
        AmmoType getAmmoType() const { return type; }
        void setAmmoType(AmmoType ammoType) { type = ammoType; }
        
        const std::string& getAmmoTypeName() const { return typeName; }
        void setAmmoTypeName(const std::string& name) { typeName = name; }
        
        int getDamageModifier() const { return damageModifier; }
        void setDamageModifier(int modifier) { damageModifier = modifier; }
        
        int getPenetration() const { return penetration; }
        void setPenetration(int pen) { penetration = pen; }

    private:
        AmmoType type;
        std::string typeName;
        int damageModifier; // Percentage modifier (e.g., 110 = 10% more damage)
        int penetration; // Armor penetration value
    };
}