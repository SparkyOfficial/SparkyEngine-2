/*
 * BallisticsSystem.h
 * 
 * Author: Андрій Будильников
 * 
 * Advanced ballistics system for realistic bullet physics
 */

#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <memory>

namespace Sparky {
    
    // Structure to represent a bullet in flight
    struct Bullet {
        glm::vec3 position;
        glm::vec3 velocity;
        glm::vec3 acceleration;
        float mass;
        float dragCoefficient;
        float crossSectionalArea;
        float damage;
        float lifetime;
        float maxLifetime;
        bool isActive;
        int ownerId; // ID of the entity that fired the bullet
        
        Bullet() : position(0.0f), velocity(0.0f), acceleration(0.0f),
                   mass(0.01f), dragCoefficient(0.295f), crossSectionalArea(0.0000785f),
                   damage(10.0f), lifetime(0.0f), maxLifetime(10.0f),
                   isActive(false), ownerId(-1) {}
    };
    
    // Structure to represent ballistics configuration
    struct BallisticsConfig {
        glm::vec3 gravity;           // Gravity vector
        float airDensity;            // Air density (kg/m³)
        float windSpeed;             // Wind speed (m/s)
        glm::vec3 windDirection;     // Wind direction
        bool enableAirResistance;    // Whether to simulate air resistance
        bool enableWind;             // Whether to simulate wind effects
        
        BallisticsConfig() : gravity(0.0f, -9.81f, 0.0f),
                            airDensity(1.225f),
                            windSpeed(0.0f),
                            windDirection(1.0f, 0.0f, 0.0f),
                            enableAirResistance(true),
                            enableWind(false) {}
    };
    
    class BallisticsSystem {
    public:
        static BallisticsSystem& getInstance();
        
        // Initialize the ballistics system
        void initialize();
        
        // Update all active bullets
        void update(float deltaTime);
        
        // Create a new bullet
        int createBullet(const glm::vec3& position, const glm::vec3& velocity, 
                        float mass, float damage, int ownerId, float lifetime = 10.0f);
        
        // Get bullet by ID
        Bullet* getBullet(int id);
        
        // Remove a bullet
        void removeBullet(int id);
        
        // Get all active bullets
        const std::vector<Bullet>& getActiveBullets() const;
        
        // Set ballistics configuration
        void setConfig(const BallisticsConfig& config);
        const BallisticsConfig& getConfig() const;
        
        // Raycast to check for hits (simplified collision detection)
        bool raycast(const glm::vec3& start, const glm::vec3& end, glm::vec3& hitPoint, int& hitEntityId) const;
        
        // Get bullet count
        size_t getBulletCount() const;
        
        // Clear all bullets
        void clear();
        
    private:
        BallisticsSystem();
        ~BallisticsSystem();
        
        // Calculate forces acting on a bullet
        glm::vec3 calculateForces(const Bullet& bullet) const;
        
        // Update bullet physics
        void updateBullet(Bullet& bullet, float deltaTime);
        
        std::vector<Bullet> bullets;
        int nextBulletId;
        BallisticsConfig config;
        
        // Private copy constructor and assignment operator to prevent copying
        BallisticsSystem(const BallisticsSystem&) = delete;
        BallisticsSystem& operator=(const BallisticsSystem&) = delete;
    };
}