#include "Enemy.h"
#include "../../Engine/include/HealthComponent.h"

namespace Sparky {

    Enemy::Enemy() : GameObject("Enemy"), healthComponent(nullptr) {
        // Add health component
        healthComponent = addComponent<HealthComponent>();
    }

    Enemy::~Enemy() {
        // Components are automatically cleaned up by GameObject destructor
    }

    void Enemy::update(float deltaTime) {
        GameObject::update(deltaTime);
        // Update health component
        if (healthComponent) {
            healthComponent->update(deltaTime);
        }
    }

    void Enemy::render() {
        GameObject::render();
        // Render enemy
    }
}