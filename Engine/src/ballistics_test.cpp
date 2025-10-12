/*
 * ballistics_test.cpp
 * 
 * Author: Андрій Будильников
 * 
 * Simple test to demonstrate the ballistics system
 */

#include "../include/SparkyEngine.h"
#include "../include/BallisticsSystem.h"
#include "../include/Logger.h"
#include <iostream>
#include <memory>

using namespace Sparky;

class BallisticsTest {
private:
    BallisticsSystem& ballisticsSystem;

public:
    BallisticsTest() : ballisticsSystem(BallisticsSystem::getInstance()) {
        // Initialize the ballistics system
        ballisticsSystem.initialize();
    }

    ~BallisticsTest() {
        // Cleanup
        ballisticsSystem.clear();
    }

    bool initialize() {
        SPARKY_LOG_INFO("Initializing Ballistics Test...");

        // Configure the ballistics system
        BallisticsConfig config;
        config.gravity = glm::vec3(0.0f, -9.81f, 0.0f);  // Standard Earth gravity
        config.airDensity = 1.225f;  // Standard air density at sea level
        config.enableAirResistance = true;
        config.enableWind = false;
        
        ballisticsSystem.setConfig(config);

        SPARKY_LOG_INFO("Ballistics Test initialized successfully!");
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
                               std::to_string(bullets[i].position.z) + ")");
            }
        }
    }

    void run() {
        if (!initialize()) {
            SPARKY_LOG_ERROR("Failed to initialize Ballistics Test!");
            return;
        }

        // Create a few bullets to demonstrate the system
        glm::vec3 position(0.0f, 1.5f, 0.0f);  // Start at 1.5m height
        glm::vec3 velocity(50.0f, 0.0f, 0.0f); // 50 m/s horizontal velocity
        
        // Create 3 bullets with different properties
        ballisticsSystem.createBullet(position, velocity, 0.01f, 10.0f, 0, 5.0f);
        ballisticsSystem.createBullet(position + glm::vec3(0.0f, 1.0f, 0.0f), velocity, 0.01f, 10.0f, 0, 5.0f);
        ballisticsSystem.createBullet(position + glm::vec3(0.0f, 2.0f, 0.0f), velocity, 0.01f, 10.0f, 0, 5.0f);

        // Simulate for 3 seconds
        float totalTime = 0.0f;
        float deltaTime = 1.0f / 60.0f; // 60 FPS
        
        SPARKY_LOG_INFO("Starting ballistics simulation...");
        
        while (totalTime < 3.0f) {
            update(deltaTime);
            totalTime += deltaTime;
            
            // Sleep to simulate frame time (in a real application, this would be handled by the engine)
            // std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }
        
        SPARKY_LOG_INFO("Ballistics simulation completed!");
    }
};

int main() {
    try {
        BallisticsTest test;
        test.run();
        return 0;
    } catch (const std::exception& e) {
        SPARKY_LOG_ERROR("Exception occurred: " + std::string(e.what()));
        return -1;
    }
}