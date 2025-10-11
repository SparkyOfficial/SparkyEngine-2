#include "../include/AmmoItem.h"
#include "../include/Logger.h"

namespace Sparky {

    AmmoItem::AmmoItem(const std::string& name, AmmoType type, int quantity, int maxStack) : 
        Item(name, maxStack), type(type), typeName("Regular"), 
        damageModifier(100), penetration(0) {
        
        setQuantity(quantity);
        
        // Set type-specific properties
        switch (type) {
            case AmmoType::REGULAR:
                typeName = "Regular";
                damageModifier = 100; // 100% base damage
                penetration = 0;
                break;
            case AmmoType::ARMOR_PIERCING:
                typeName = "Armor Piercing";
                damageModifier = 90; // Slightly less damage but better penetration
                penetration = 50;
                break;
            case AmmoType::EXPLOSIVE:
                typeName = "Explosive";
                damageModifier = 120; // More damage but less accurate
                penetration = 20;
                break;
            case AmmoType::INCENDIARY:
                typeName = "Incendiary";
                damageModifier = 110; // Good damage with burn effect
                penetration = 10;
                break;
        }
        
        Logger::getInstance().info("Created ammo item: " + name + " (" + typeName + ")");
    }

    AmmoItem::~AmmoItem() {
    }

    bool AmmoItem::isStackable() const {
        return true; // Ammo is stackable
    }

    std::unique_ptr<Item> AmmoItem::clone() const {
        auto clone = std::make_unique<AmmoItem>(getName(), type, getQuantity(), getMaxStack());
        clone->setAmmoTypeName(typeName);
        clone->setDamageModifier(damageModifier);
        clone->setPenetration(penetration);
        return clone;
    }
}