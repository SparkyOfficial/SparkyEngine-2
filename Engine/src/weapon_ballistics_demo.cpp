/*
 * weapon_ballistics_demo.cpp
 * 
 * Author: Андрій Будильников
 * 
 * Demo to show weapon system with advanced ballistics
 */

#include "../include/SparkyEngine.h"
#include "../include/Gun.h"
#include "../include/GunImpl.h"
#include "../include/BallisticsSystem.h"
#include "../include/Logger.h"
#include <iostream>
#include <memory>

using namespace Sparky;

class WeaponBallisticsDemo {
private:
    std::unique_ptr<Gun> playerGun;
    BallisticsSystem& ballisticsSystem;

public:
    WeaponBallisticsDemo() : ballisticsSystem(BallisticsSystem::getInstance()) {
        // Initialize the ballistics system
        ballisticsSystem.initialize();
    }

    ~WeaponBallisticsDemo() {
        // Cleanup
        ballisticsSystem.clear();
    }

    bool initialize() {
        SPARKY_LOG_INFO("Initializing Weapon Ballistics Demo...");

        // Create a player gun
        playerGun = std::make_unique<Gun>();
        
        // Configure the gun
        playerGun->setAmmo(30);
        playerGun->setMagazineSize(30);
        playerGun->setTotalAmmo(120);
        playerGun->setFireRate(10.0f); // 10 shots per second
        playerGun->setSpread(1.0f);    // 1 degree spread
        playerGun->setDamage(10.0f);
        playerGun->setRecoil(0.5f);
        playerGun->setRecoilRecovery(1.0f);
        playerGun->setMuzzleVelocity(500.0f); // 500 m/s muzzle velocity
        
        // Configure ammo properties for more realistic ballistics
        AmmoProperties ammoProps;
        ammoProps.mass = 0.01f;              // 10g bullet
        ammoProps.muzzleVelocity = 500.0f;   // 500 m/s
        ammoProps.damage = 10.0f;
        ammoProps.penetration = 1.0f;
        ammoProps.explosiveRadius = 0.0f;
        ammoProps.fireRateModifier = 1.0f;
        ammoProps.recoilModifier = 1.0f;
        
        // We would need to add a method to set ammo properties in GunImpl
        // For now, we'll just log that we're configuring the ammo
        
        // Configure the ballistics system
        BallisticsConfig config;
        config.gravity = glm::vec3(0.0f, -9.81f, 0.0f);  // Standard Earth gravity
        config.airDensity = 1.225f;  // Standard air density at sea level
        config.enableAirResistance = true;
        config.enableWind = false;
        
        ballisticsSystem.setConfig(config);

        SPARKY_LOG_INFO("Weapon Ballistics Demo initialized successfully!");
        return true;
    }

    void update(float deltaTime) {
        // Update the ballistics system
        ballisticsSystem.update(deltaTime);
        
        // Print bullet count
        size_t bulletCount = ballisticsSystem.getBulletCount();
        SPARKY_LOG_INFO("Active bullets: " + std::to_string(bulletCount));
        
        // Print positions of active bullets
        const auto& bullets = ballisticsSystem.getActiveBullets();
        for (size_t i = 0; i < bullets.size(); ++i) {
            if (bullets[i].isActive) {
                SPARKY_LOG_INFO("Bullet " + std::to_string(i) + 
                               " position: (" + std::to_string(bullets[i].position.x) + ", " + 
                               std::to_string(bullets[i].position.y) + ", " + 
                               std::to_string(bullets[i].position.z) + ")" +
                               " velocity: (" + std::to_string(bullets[i].velocity.x) + ", " + 
                               std::to_string(bullets[i].velocity.y) + ", " + 
                               std::to_string(bullets[i].velocity.z) + ")");
            }
        }
    }

    void run() {
        if (!initialize()) {
            SPARKY_LOG_ERROR("Failed to initialize Weapon Ballistics Demo!");
            return;
        }

        // Simulate shooting a few bullets
        SPARKY_LOG_INFO("Simulating gun firing...");
        
        // In a real implementation, we would call playerGun->shoot()
        // For this demo, we'll directly create bullets in the ballistics system
        
        // Create a few bullets to demonstrate the system
        glm::vec3 position(0.0f, 1.5f, 0.0f);  // Start at 1.5m height
        glm::vec3 velocity(500.0f, 0.0f, 0.0f); // 500 m/s horizontal velocity (muzzle velocity)
        
        // Create 3 bullets with different properties
        ballisticsSystem.createBullet(position, velocity, 0.01f, 10.0f, 0, 5.0f);
        ballisticsSystem.createBullet(position + glm::vec3(0.0f, 0.1f, 0.0f), velocity, 0.01f, 10.0f, 0, 5.0f);
        ballisticsSystem.createBullet(position + glm::vec3(0.0f, 0.2f, 0.0f), velocity, 0.01f, 10.0f, 0, 5.0f);

        // Simulate for 2 seconds to show bullet drop due to gravity
        float totalTime = 0.0f;
        float deltaTime = 1.0f / 60.0f; // 60 FPS
        
        SPARKY_LOG_INFO("Starting weapon ballistics simulation...");
        
        while (totalTime < 2.0f) {
            update(deltaTime);
            totalTime += deltaTime;
            
            // Sleep to simulate frame time (in a real application, this would be handled by the engine)
            // std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }
        
        SPARKY_LOG_INFO("Weapon ballistics simulation completed!");
    }
};

int main() {
    try {
        WeaponBallisticsDemo demo;
        demo.run();
        return 0;
    } catch (const std::exception& e) {
        SPARKY_LOG_ERROR("Exception occurred: " + std::string(e.what()));
        return -1;
    }
}