#include "Player.h"
#include "../../Engine/include/HealthComponent.h"

namespace Sparky {

    Player::Player() : healthComponent(nullptr) {
        // Add health component
        healthComponent = new HealthComponent();
    }

    Player::~Player() {
        delete healthComponent;
    }

    void Player::update(float deltaTime) {
        // Update health component
        if (healthComponent) {
            healthComponent->update(deltaTime);
        }
    }

    void Player::render() {
        // Render player
    }
}