#pragma once

#include <string>
#include <memory>
#include "Inventory.h"

namespace Sparky {
    // Forward declarations
    class Item;
    class Player;
    
    class Pickup {
    public:
        Pickup(const std::string& name = "Pickup");
        virtual ~Pickup();

        virtual void update(float deltaTime);
        virtual void render();
        
        // Pickup functionality
        virtual void onPickup(Player* player) = 0; // Pure virtual function
        virtual bool canPickup(Player* player) const;
        
        // Getters and setters
        Item* getItem() const { return item.get(); }
        void setItem(std::unique_ptr<Item> newItem);
        
        bool isPickedUp() const { return pickedUp; }
        void setPickedUp(bool picked) { pickedUp = picked; }
        
        const std::string& getName() const { return name; }
        void setName(const std::string& pickupName) { name = pickupName; }

    protected:
        std::string name;
        std::unique_ptr<Item> item;
        bool pickedUp;
    };
    
    // Specific pickup types
    class HealthPickup : public Pickup {
    public:
        HealthPickup(float healthAmount = 25.0f);
        virtual ~HealthPickup();
        
        virtual void onPickup(Player* player) override;
        
        float getHealthAmount() const { return healthAmount; }
        void setHealthAmount(float amount) { healthAmount = amount; }
        
    private:
        float healthAmount;
    };
    
    class AmmoPickup : public Pickup {
    public:
        AmmoPickup(const std::string& ammoType = "default", int ammoCount = 30);
        virtual ~AmmoPickup();
        
        virtual void onPickup(Player* player) override;
        
        const std::string& getAmmoType() const { return ammoType; }
        int getAmmoCount() const { return ammoCount; }
        
        void setAmmoType(const std::string& type) { ammoType = type; }
        void setAmmoCount(int count) { ammoCount = count; }
        
    private:
        std::string ammoType;
        int ammoCount;
    };
}