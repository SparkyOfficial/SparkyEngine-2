#include "GunImpl.h"
#include "../../Engine/include/InputManager.h"
#include "../../Engine/include/Logger.h"
#include "../../Engine/include/PhysicsWorld.h"
#include "../../Engine/include/RigidBodyComponent.h"
#include "../../Engine/include/RenderComponent.h"
#include "../../Engine/include/Mesh.h"
#include "../../Engine/include/ParticleComponent.h"
#include "../../Engine/include/ParticleSystem.h"
#include "../../Engine/include/Camera.h"

#ifdef ENABLE_AUDIO
#include "../../Engine/include/AudioEngine.h"
#include "../../Engine/include/AudioComponent.h"
#endif
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <random>
#include <algorithm>

#ifdef HAS_GLFW
#include <GLFW/glfw3.h>
#endif

namespace Sparky {
    
    GunImpl::GunImpl() : 
        currentAmmo(30), magazineSize(30), totalAmmo(120),
        fireRate(10.0f), spread(1.0f), damage(10.0f),
        lastShotTime(0.0f), isReloading(false), reloadTime(2.0f), lastReloadTime(0.0f),
        isAiming(false),
        recoil(0.5f), recoilRecovery(1.0f), currentRecoil(0.0f),
        muzzleVelocity(500.0f), weaponType("Assault Rifle"),
        recoilPattern{0.5f, 0.2f, 0.1f, 1.0f, 0.3f},
        stats{0, 0, 0, 0, 0, 0.0f, 0.0f},
        heat(0.0f), wear(0.0f), overheated(false), lastHeatTime(0.0f),
        jammed(false), jamProbability(0.05f),
        firingMode(FiringMode::SEMI_AUTO), ammoType(AmmoType::REGULAR),
        burstCount(3), currentBurstShot(0), burstDelay(0.1f), lastBurstShotTime(0.0f),
        camera(nullptr) {
    }
    
    GunImpl::~GunImpl() {
    }
    
    void GunImpl::update(float deltaTime) {
        // Handle reloading
        if (isReloading) {
#ifdef HAS_GLFW
            if (glfwGetTime() - lastReloadTime >= reloadTime) {
#else
            static float fallbackTime = 0.0f;
            fallbackTime += deltaTime;
            if (fallbackTime - lastReloadTime >= reloadTime) {
#endif
                isReloading = false;
                int ammoNeeded = magazineSize - currentAmmo;
                int ammoToLoad = std::min(ammoNeeded, totalAmmo);
                currentAmmo += ammoToLoad;
                totalAmmo -= ammoToLoad;
                SPARKY_LOG_INFO("Reloaded. Current ammo: " + std::to_string(currentAmmo));
            }
        }
        
        // Handle recoil recovery
        if (currentRecoil > 0.0f) {
            currentRecoil -= recoilRecovery * deltaTime;
            if (currentRecoil < 0.0f) {
                currentRecoil = 0.0f;
            }
        }
        
        // Update heat and wear
        updateHeat(deltaTime);
        coolDown(deltaTime);
        
        // Update jamming
        updateJamming();
        
        // Update burst fire
        updateBurst(deltaTime);
        
        // Handle shooting input (this would typically be handled by the parent Gun class)
        // For now, we'll just provide the functionality
    }
    
    void GunImpl::shoot() {
        if (!canShoot() || !camera) {
            return;
        }
        
        // Update last shot time
#ifdef HAS_GLFW
        lastShotTime = static_cast<float>(glfwGetTime());
#else
        static float fallbackTime = 0.0f;
        lastShotTime = fallbackTime;
#endif
        
        // Update burst shot time if in burst mode
        if (firingMode == FiringMode::BURST) {
            lastBurstShotTime = lastShotTime;
        }
        
        // Decrease ammo
        currentAmmo--;
        
        // Apply recoil
        applyRecoil();
        
        // Calculate shot direction with spread
        float shotDirection[3];
        calculateSpreadDirection(shotDirection);
        
        // Create a bullet object
        createBullet(shotDirection);
        
        // Play shooting sound
        playShootSound();
        
        // Create muzzle flash effect
        createMuzzleFlash(shotDirection);
        
        // Update stats
        stats.shotsFired++;
        
        // Update heat
        heat += 0.1f;
        if (heat > 1.0f) heat = 1.0f;
        
        // Update wear
        wear += 0.001f;
        if (wear > 1.0f) wear = 1.0f;
        
        // Log the shot
        SPARKY_LOG_INFO("Gun fired! Ammo: " + std::to_string(currentAmmo));
    }
    
    void GunImpl::reload() {
        if (isReloading || currentAmmo >= magazineSize || totalAmmo <= 0) {
            return;
        }
        
        isReloading = true;
#ifdef HAS_GLFW
        lastReloadTime = static_cast<float>(glfwGetTime());
#else
        static float fallbackTime = 0.0f;
        lastReloadTime = fallbackTime;
#endif
        stats.reloads++;
        SPARKY_LOG_INFO("Reloading weapon...");
    }
    
    void GunImpl::aim() {
        isAiming = true;
        // In a full implementation, we would adjust FOV and position
    }
    
    void GunImpl::unaim() {
        isAiming = false;
        // In a full implementation, we would reset FOV and position
    }
    
    bool GunImpl::canShoot() const {
        // Check if we have ammo and are not reloading
        if (currentAmmo <= 0 || isReloading || jammed || overheated) {
            return false;
        }
        
        // Check fire rate cooldown
#ifdef HAS_GLFW
        float currentTime = static_cast<float>(glfwGetTime());
#else
        static float fallbackTime = 0.0f;
        float currentTime = fallbackTime;
#endif
        
        float timeSinceLastShot = currentTime - lastShotTime;
        float fireInterval = 1.0f / fireRate;
        
        // For burst mode, also check burst delay
        if (firingMode == FiringMode::BURST) {
            float timeSinceLastBurstShot = currentTime - lastBurstShotTime;
            return timeSinceLastShot >= fireInterval && timeSinceLastBurstShot >= burstDelay;
        }
        
        return timeSinceLastShot >= fireInterval;
    }
    
    void GunImpl::applyRecoil() {
        currentRecoil += recoil;
        
        // Apply recoil to camera
        if (camera) {
            // Add some randomness to the recoil direction
            static std::random_device rd;
            static std::ranlux24 gen(rd());
            static std::uniform_real_distribution<float> dis(-0.5f, 0.5f);
            
            // Apply recoil pattern
            float recoilYaw = dis(gen) * currentRecoil * recoilPattern.horizontalRecoil;
            float recoilPitch = -currentRecoil * recoilPattern.verticalRecoil;
            
            camera->ProcessMouseMovement(recoilYaw, recoilPitch);
        }
    }
    
    void GunImpl::calculateSpreadDirection(float* direction) const {
        if (!camera) {
            direction[0] = 0.0f;
            direction[1] = 0.0f;
            direction[2] = -1.0f;
            return;
        }
        
        // Get the base direction from the camera
        direction[0] = camera->Front.x;
        direction[1] = camera->Front.y;
        direction[2] = camera->Front.z;
        
        // Add spread by randomly perturbing the direction
        if (spread > 0.0f) {
            // Create a random number generator
            static std::random_device rd;
            static std::ranlux24 gen(rd());
            static std::uniform_real_distribution<float> dis(-1.0f, 1.0f);
            
            // Generate random offsets
            float offsetX = dis(gen) * spread * 0.01f;
            float offsetY = dis(gen) * spread * 0.01f;
            
            // Apply the offsets
            direction[0] += offsetX;
            direction[1] += offsetY;
            
            // Normalize the direction
            float length = sqrt(direction[0]*direction[0] + direction[1]*direction[1] + direction[2]*direction[2]);
            if (length > 0.0f) {
                direction[0] /= length;
                direction[1] /= length;
                direction[2] /= length;
            }
        }
    }
    
    void GunImpl::startBurst() {
        if (firingMode == FiringMode::BURST && currentBurstShot == 0) {
            currentBurstShot = 1;
        }
    }
    
    void GunImpl::updateBurst(float deltaTime) {
        if (firingMode == FiringMode::BURST && currentBurstShot > 0) {
#ifdef HAS_GLFW
            float currentTime = static_cast<float>(glfwGetTime());
#else
            static float fallbackTime = 0.0f;
            float currentTime = fallbackTime;
#endif
            float timeSinceLastBurstShot = currentTime - lastBurstShotTime;
            
            if (timeSinceLastBurstShot >= burstDelay && currentBurstShot <= burstCount) {
                // Shoot the next burst shot
                if (canShoot()) {
                    shoot();
                    currentBurstShot++;
                    lastBurstShotTime = currentTime;
                }
            }
            
            if (currentBurstShot > burstCount) {
                currentBurstShot = 0;
            }
        }
    }
    
    void GunImpl::createBullet(const float* direction) {
        // Create a bullet object
        // Note: In a real implementation, this would be handled by the game world
        SPARKY_LOG_DEBUG("Bullet created with direction: " + 
                        std::to_string(direction[0]) + ", " + 
                        std::to_string(direction[1]) + ", " + 
                        std::to_string(direction[2]));
    }
    
    void GunImpl::createMuzzleFlash(const float* direction) {
        // In a real implementation, this would trigger particle effects
        SPARKY_LOG_DEBUG("Muzzle flash created");
    }
    
    void GunImpl::playShootSound() {
        // In a real implementation, this would play audio
        SPARKY_LOG_DEBUG("Gunshot sound played");
    }
    
    void GunImpl::getRecoilPattern(float* pattern) const {
        pattern[0] = recoilPattern.verticalRecoil;
        pattern[1] = recoilPattern.horizontalRecoil;
        pattern[2] = recoilPattern.lateralRecoil;
        pattern[3] = recoilPattern.recoverySpeed;
        pattern[4] = recoilPattern.punchAngle;
    }
    
    void GunImpl::setRecoilPattern(const float* pattern) {
        recoilPattern.verticalRecoil = pattern[0];
        recoilPattern.horizontalRecoil = pattern[1];
        recoilPattern.lateralRecoil = pattern[2];
        recoilPattern.recoverySpeed = pattern[3];
        recoilPattern.punchAngle = pattern[4];
    }
    
    void GunImpl::addAttachment(const std::string& name, float modifier, bool active) {
        WeaponAttachment attachment;
        attachment.name = name;
        attachment.modifier = modifier;
        attachment.active = active;
        attachments[name] = attachment;
    }
    
    void GunImpl::removeAttachment(const std::string& name) {
        attachments.erase(name);
    }
    
    void GunImpl::toggleAttachment(const std::string& name) {
        auto it = attachments.find(name);
        if (it != attachments.end()) {
            it->second.active = !it->second.active;
        }
    }
    
    void GunImpl::getStats(int* statsArray) const {
        statsArray[0] = stats.shotsFired;
        statsArray[1] = stats.shotsHit;
        statsArray[2] = stats.headshots;
        statsArray[3] = stats.reloads;
        statsArray[4] = stats.jams;
        // For the float values, we'll convert them to int representation
        // In a real implementation, you might want to pass them separately
        statsArray[5] = static_cast<int>(stats.accuracy * 100);
        statsArray[6] = static_cast<int>(stats.avgDamage);
    }
    
    void GunImpl::resetStats() {
        stats.shotsFired = 0;
        stats.shotsHit = 0;
        stats.headshots = 0;
        stats.reloads = 0;
        stats.jams = 0;
        stats.accuracy = 0.0f;
        stats.avgDamage = 0.0f;
    }
    
    void GunImpl::updateStats() {
        // In a real implementation, this would update the accuracy and average damage
        // based on the shots fired and hits
        if (stats.shotsFired > 0) {
            stats.accuracy = static_cast<float>(stats.shotsHit) / static_cast<float>(stats.shotsFired);
        } else {
            stats.accuracy = 0.0f;
        }
        
        if (stats.shotsHit > 0) {
            stats.avgDamage = stats.avgDamage / static_cast<float>(stats.shotsHit);
        } else {
            stats.avgDamage = 0.0f;
        }
    }
    
    void GunImpl::updateHeat(float deltaTime) {
#ifdef HAS_GLFW
        float currentTime = static_cast<float>(glfwGetTime());
#else
        static float fallbackTime = 0.0f;
        float currentTime = fallbackTime;
#endif
        
        // Cool down over time
        if (currentTime - lastHeatTime > 1.0f) {
            heat -= 0.05f * deltaTime;
            if (heat < 0.0f) heat = 0.0f;
            lastHeatTime = currentTime;
        }
        
        // Check for overheating
        if (heat > 0.8f) {
            overheated = true;
        } else if (heat < 0.2f) {
            overheated = false;
        }
    }
    
    void GunImpl::coolDown(float deltaTime) {
        // This is called continuously to cool down the weapon
        heat -= 0.01f * deltaTime;
        if (heat < 0.0f) heat = 0.0f;
    }
    
    void GunImpl::updateJamming() {
        // Simple jamming probability based on wear and heat
        static std::random_device rd;
        static std::ranlux24 gen(rd());
        static std::uniform_real_distribution<float> dis(0.0f, 1.0f);
        
        float jamChance = jamProbability * (wear * 0.5f + heat * 0.5f);
        if (dis(gen) < jamChance) {
            jam();
        }
    }
    
    void GunImpl::jam() {
        if (!jammed) {
            jammed = true;
            stats.jams++;
            SPARKY_LOG_INFO("Weapon jammed!");
        }
    }
    
    void GunImpl::unjam() {
        if (jammed) {
            jammed = false;
            SPARKY_LOG_INFO("Weapon unjammed!");
        }
    }
}