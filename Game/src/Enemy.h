#pragma once

#include "../../Engine/include/GameObject.h"

namespace Sparky {
    class HealthComponent;
    
    class Enemy : public GameObject {
    public:
        Enemy();
        ~Enemy();

        void update(float deltaTime) override;
        void render() override;

        // Health system
        HealthComponent* getHealthComponent() { return healthComponent; }
        const HealthComponent* getHealthComponent() const { return healthComponent; }

    private:
        // Health component
        HealthComponent* healthComponent;
    };
}