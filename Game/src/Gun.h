#pragma once

#include "../../Engine/include/GameEntity.h"
#include <vector>

namespace Sparky {
    class Bullet;

    class Gun : public GameEntity {
    public:
        Gun(const std::string& name);
        ~Gun();

        void update(float deltaTime) override;
        void render() override;

        // Gun actions
        void shoot();
        void reload();

        // Gun properties
        int getAmmo() const { return ammo; }
        int getMaxAmmo() const { return maxAmmo; }
        float getFireRate() const { return fireRate; }

    private:
        int ammo;
        int maxAmmo;
        float fireRate;
        float lastShotTime;
        std::vector<Bullet*> bullets;
    };

    class Bullet : public GameEntity {
    public:
        Bullet(const glm::vec3& position, const glm::vec3& direction);
        ~Bullet();

        void update(float deltaTime) override;
        void render() override;

        bool isAlive() const { return alive; }

    private:
        glm::vec3 direction;
        float speed;
        bool alive;
        float lifetime;
    };
}