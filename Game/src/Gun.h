#pragma once

#include "../../Engine/include/GameObject.h"
#include "../../Engine/include/Camera.h"
#include <glm/glm.hpp>

namespace Sparky {
    class Gun : public GameObject {
    public:
        Gun();
        ~Gun();

        void update(float deltaTime) override;
        void render() override;

        // Weapon functions
        void shoot();
        void reload();
        bool canShoot() const;
        
        // Getters and setters
        int getAmmo() const { return currentAmmo; }
        int getMagazineSize() const { return magazineSize; }
        int getTotalAmmo() const { return totalAmmo; }
        float getFireRate() const { return fireRate; }
        float getSpread() const { return spread; }
        float getDamage() const { return damage; }
        
        void setAmmo(int ammo) { currentAmmo = ammo; }
        void setMagazineSize(int size) { magazineSize = size; }
        void setTotalAmmo(int ammo) { totalAmmo = ammo; }
        void setFireRate(float rate) { fireRate = rate; }
        void setSpread(float sp) { spread = sp; }
        void setDamage(float dmg) { damage = dmg; }
        
        void setCamera(Camera* camera) { this->camera = camera; }
        Camera* getCamera() const { return camera; }
        
        // New getters and setters for advanced features
        float getRecoil() const;
        void setRecoil(float recoil);
        
        float getRecoilRecovery() const;
        void setRecoilRecovery(float recovery);
        
        float getMuzzleVelocity() const;
        void setMuzzleVelocity(float velocity);
        
        const std::string& getWeaponType() const;
        void setWeaponType(const std::string& type);

    private:
        Camera* camera;
        
        // Weapon properties
        int currentAmmo;
        int magazineSize;
        int totalAmmo;
        float fireRate;         // Shots per second
        float spread;           // Accuracy spread in degrees
        float damage;
        float lastShotTime;
        bool isReloading;
        float reloadTime;
        float lastReloadTime;
        
        // Advanced weapon properties
        float recoil;           // Amount of camera kick per shot
        float recoilRecovery;   // How fast recoil returns to normal
        float currentRecoil;    // Current recoil amount
        float muzzleVelocity;   // Speed of bullets when fired
        std::string weaponType; // Type of weapon (e.g., "Assault Rifle", "Pistol")
        
        // Private helper methods
        glm::vec3 calculateSpreadDirection() const;
        void createBullet(const glm::vec3& direction);
        void createMuzzleFlash();
    };
}