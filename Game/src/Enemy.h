#pragma once

namespace Sparky {
    class HealthComponent;
    
    class Enemy {
    public:
        Enemy();
        ~Enemy();

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