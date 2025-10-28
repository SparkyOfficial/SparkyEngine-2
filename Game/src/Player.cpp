#include "Player.h"
#include "../../Engine/include/HealthComponent.h"

namespace Sparky {

    Player::Player() : GameObject("Player"), healthComponent(nullptr) {
        // Add health component as a proper component
        healthComponent = addComponent<HealthComponent>();
    }

    Player::~Player() {
        // Components are automatically cleaned up by GameObject destructor
    }

    void Player::update(float deltaTime) {
        GameObject::update(deltaTime);
        // Update health component
        if (healthComponent) {
            healthComponent->update(deltaTime);
        }
    }

    void Player::render() {
        GameObject::render();
        // Render player
    }
}