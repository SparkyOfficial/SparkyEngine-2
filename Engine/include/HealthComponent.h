#pragma once

#include "Component.h"
#include <functional>

namespace Sparky {
    class HealthComponent : public Component {
    public:
        HealthComponent(float maxHealth = 100.0f);
        ~HealthComponent();

        void update(float deltaTime) override;
        void render() override;

        // Health management
        void takeDamage(float damage);
        void heal(float amount);
        void setHealth(float health);
        void setMaxHealth(float maxHealth);
        void setRegenerationRate(float rate); // Health regenerated per second
        
        // Getters
        float getHealth() const { return currentHealth; }
        float getMaxHealth() const { return maxHealth; }
        float getRegenerationRate() const { return regenerationRate; }
        bool isAlive() const { return currentHealth > 0; }
        
        // Events
        void setOnDeathCallback(std::function<void()> callback) { onDeathCallback = callback; }
        void setOnDamageCallback(std::function<void(float)> callback) { onDamageCallback = callback; }
        void setOnHealCallback(std::function<void(float)> callback) { onHealCallback = callback; }

    private:
        float currentHealth;
        float maxHealth;
        float regenerationRate; // Health regenerated per second
        float lastRegenerationTime;
        
        // Callbacks
        std::function<void()> onDeathCallback;
        std::function<void(float)> onDamageCallback;
        std::function<void(float)> onHealCallback;
    };
}