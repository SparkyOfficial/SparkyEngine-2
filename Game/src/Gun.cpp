#include "Gun.h"
#include <glm/glm.hpp>

namespace Sparky {

    Gun::Gun(const std::string& name) : GameEntity(name), ammo(30), maxAmmo(30), 
                                        fireRate(0.1f), lastShotTime(0.0f) {
    }

    Gun::~Gun() {
        // Clean up bullets
        for (Bullet* bullet : bullets) {
            delete bullet;
        }
        bullets.clear();
    }

    void Gun::update(float deltaTime) {
        // Update bullets
        for (int i = bullets.size() - 1; i >= 0; i--) {
            Bullet* bullet = bullets[i];
            bullet->update(deltaTime);
            
            // Remove dead bullets
            if (!bullet->isAlive()) {
                delete bullet;
                bullets.erase(bullets.begin() + i);
            }
        }
    }

    void Gun::render() {
        // Render gun
        // Render bullets
        for (Bullet* bullet : bullets) {
            bullet->render();
        }
    }

    void Gun::shoot() {
        // In a real implementation, we would create a bullet and add it to the bullets vector
        // For now, just a placeholder
    }

    void Gun::reload() {
        ammo = maxAmmo;
    }

    Bullet::Bullet(const glm::vec3& position, const glm::vec3& direction) : 
        GameEntity("Bullet"), direction(direction), speed(20.0f), alive(true), lifetime(5.0f) {
        setPosition(position);
    }

    Bullet::~Bullet() {
    }

    void Bullet::update(float deltaTime) {
        // Move bullet
        glm::vec3 pos = getPosition();
        pos += direction * speed * deltaTime;
        setPosition(pos);
        
        // Decrease lifetime
        lifetime -= deltaTime;
        if (lifetime <= 0.0f) {
            alive = false;
        }
    }

    void Bullet::render() {
        // Bullet rendering implementation
    }
}