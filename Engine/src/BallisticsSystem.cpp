/*
 * BallisticsSystem.cpp
 * 
 * Author: Андрій Будильников
 * 
 * Advanced ballistics system implementation for realistic bullet physics
 */

#include "../include/BallisticsSystem.h"
#include "../include/Logger.h"
#include <algorithm>
#include <cmath>

namespace Sparky {
    
    BallisticsSystem& BallisticsSystem::getInstance() {
        static BallisticsSystem instance;
        return instance;
    }
    
    BallisticsSystem::BallisticsSystem() : nextBulletId(0) {
        // Initialize with default configuration
        config = BallisticsConfig();
    }
    
    BallisticsSystem::~BallisticsSystem() {
        // Cleanup if needed
    }
    
    void BallisticsSystem::initialize() {
        SPARKY_LOG_INFO("BallisticsSystem initialized");
        bullets.clear();
        nextBulletId = 0;
    }
    
    void BallisticsSystem::update(float deltaTime) {
        // Update all active bullets
        for (auto it = bullets.begin(); it != bullets.end();) {
            if (it->isActive) {
                updateBullet(*it, deltaTime);
                
                // Check if bullet should be removed
                it->lifetime += deltaTime;
                if (it->lifetime >= it->maxLifetime || !it->isActive) {
                    it = bullets.erase(it);
                } else {
                    ++it;
                }
            } else {
                ++it;
            }
        }
    }
    
    int BallisticsSystem::createBullet(const glm::vec3& position, const glm::vec3& velocity, 
                                      float mass, float damage, int ownerId, float lifetime) {
        Bullet bullet;
        bullet.position = position;
        bullet.velocity = velocity;
        bullet.mass = mass;
        bullet.damage = damage;
        bullet.ownerId = ownerId;
        bullet.maxLifetime = lifetime;
        bullet.isActive = true;
        
        bullets.push_back(bullet);
        return static_cast<int>(bullets.size() - 1);
    }
    
    Bullet* BallisticsSystem::getBullet(int id) {
        if (id >= 0 && id < static_cast<int>(bullets.size())) {
            return &bullets[id];
        }
        return nullptr;
    }
    
    void BallisticsSystem::removeBullet(int id) {
        if (id >= 0 && id < static_cast<int>(bullets.size())) {
            bullets[id].isActive = false;
        }
    }
    
    const std::vector<Bullet>& BallisticsSystem::getActiveBullets() const {
        return bullets;
    }
    
    void BallisticsSystem::setConfig(const BallisticsConfig& config) {
        this->config = config;
    }
    
    const BallisticsConfig& BallisticsSystem::getConfig() const {
        return config;
    }
    
    glm::vec3 BallisticsSystem::calculateForces(const Bullet& bullet) const {
        glm::vec3 totalForce(0.0f);
        
        // Add gravity
        totalForce += config.gravity * bullet.mass;
        
        // Add air resistance (drag force)
        if (config.enableAirResistance) {
            float speed = glm::length(bullet.velocity);
            if (speed > 0.0f) {
                // Drag force: F = 0.5 * rho * v² * Cd * A
                // Where rho is air density, Cd is drag coefficient, A is cross-sectional area
                float dragMagnitude = 0.5f * config.airDensity * speed * speed * 
                                    bullet.dragCoefficient * bullet.crossSectionalArea;
                
                // Drag force is opposite to velocity direction
                glm::vec3 dragDirection = -glm::normalize(bullet.velocity);
                glm::vec3 dragForce = dragDirection * dragMagnitude;
                
                totalForce += dragForce;
            }
        }
        
        // Add wind effect
        if (config.enableWind) {
            // Simplified wind effect - adds a constant force in wind direction
            glm::vec3 windForce = config.windDirection * config.windSpeed * 0.01f;
            totalForce += windForce;
        }
        
        return totalForce;
    }
    
    void BallisticsSystem::updateBullet(Bullet& bullet, float deltaTime) {
        if (!bullet.isActive) return;
        
        // Calculate forces acting on the bullet
        glm::vec3 forces = calculateForces(bullet);
        
        // Calculate acceleration (F = ma => a = F/m)
        bullet.acceleration = forces / bullet.mass;
        
        // Update velocity (v = v0 + a * dt)
        bullet.velocity += bullet.acceleration * deltaTime;
        
        // Update position (p = p0 + v * dt)
        bullet.position += bullet.velocity * deltaTime;
        
        // Simple collision detection with ground plane (y = 0)
        if (bullet.position.y <= 0.0f) {
            bullet.position.y = 0.0f;
            bullet.isActive = false; // Bullet hit the ground
        }
    }
    
    bool BallisticsSystem::raycast(const glm::vec3& start, const glm::vec3& end, 
                                  glm::vec3& hitPoint, int& hitEntityId) const {
        // Simplified raycast implementation
        // In a real implementation, this would check against actual colliders
        
        // For now, just check if the ray intersects the ground plane (y = 0)
        if ((start.y > 0.0f && end.y < 0.0f) || (start.y < 0.0f && end.y > 0.0f)) {
            // Linear interpolation to find intersection point
            float t = start.y / (start.y - end.y);
            hitPoint = start + t * (end - start);
            hitEntityId = -1; // Ground plane ID
            return true;
        }
        
        return false;
    }
    
    size_t BallisticsSystem::getBulletCount() const {
        return bullets.size();
    }
    
    void BallisticsSystem::clear() {
        bullets.clear();
        nextBulletId = 0;
    }
}