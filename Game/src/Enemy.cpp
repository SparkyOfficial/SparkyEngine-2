#include "Enemy.h"
#include "../../Engine/include/HealthComponent.h"

namespace Sparky {

    Enemy::Enemy() : healthComponent(nullptr) {
        // Add health component
        healthComponent = new HealthComponent();
    }

    Enemy::~Enemy() {
        delete healthComponent;
    }

    void Enemy::update(float deltaTime) {
        // Update health component
        if (healthComponent) {
            healthComponent->update(deltaTime);
        }
    }

    void Enemy::render() {
        // Render enemy
    }
}