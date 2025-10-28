#pragma once

#include "../../Engine/include/GameObject.h"

namespace Sparky {
    class HealthComponent;
    
    class Player : public GameObject {
    public:
        Player();
        ~Player();

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