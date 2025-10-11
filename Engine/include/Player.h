#pragma once

namespace Sparky {
    // Forward declarations
    class HealthComponent;
    
    class Player {
    public:
        Player();
        ~Player();

        void update(float deltaTime);
        void render();

        // Health system
        HealthComponent* getHealthComponent() { return healthComponent; }
        const HealthComponent* getHealthComponent() const { return healthComponent; }

    private:
        // Health component
        HealthComponent* healthComponent;
    };
}