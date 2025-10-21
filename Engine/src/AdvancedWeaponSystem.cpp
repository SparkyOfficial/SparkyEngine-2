#include "../include/AdvancedWeaponSystem.h"
#include "../include/GameObject.h"
#include "../include/BallisticsSystem.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <random>
#include <cmath>

namespace Sparky {
    // WeaponAttachment implementation
    WeaponAttachment::WeaponAttachment(const std::string& name, AttachmentType type)
        : m_name(name)
        , m_type(type) {
    }
    
    void WeaponAttachment::initialize() {
        // Initialize attachment
    }
    
    void WeaponAttachment::update(float deltaTime) {
        // Update attachment
    }
    
    void WeaponAttachment::destroy() {
        // Cleanup attachment
    }
    
    void WeaponAttachment::render() {
        // Render attachment
    }
    
    // ScopeAttachment implementation
    ScopeAttachment::ScopeAttachment(const std::string& name, float zoomFactor, float aimSpeedModifier)
        : WeaponAttachment(name, AttachmentType::SCOPE)
        , m_zoomFactor(zoomFactor)
        , m_accuracyModifier(1.5f)  // Scopes generally improve accuracy
        , m_aimSpeedModifier(aimSpeedModifier) {
    }
    
    // SilencerAttachment implementation
    SilencerAttachment::SilencerAttachment(const std::string& name, float noiseReduction, float damagePenalty)
        : WeaponAttachment(name, AttachmentType::SILENCER)
        , m_noiseReduction(noiseReduction)
        , m_damagePenalty(damagePenalty) {
    }
    
    // Projectile implementation
    Projectile::Projectile(const glm::vec3& position, const glm::vec3& velocity, float damage, float lifetime)
        : GameObject("Projectile")
        , m_velocity(velocity)
        , m_damage(damage)
        , m_lifetime(lifetime)
        , m_hasHit(false) {
        setPosition(position);
    }
    
    void Projectile::update(float deltaTime) {
        if (m_hasHit || m_lifetime <= 0.0f) return;
        
        // Update position based on velocity
        glm::vec3 newPosition = getPosition() + m_velocity * deltaTime;
        setPosition(newPosition);
        
        // Update lifetime
        m_lifetime -= deltaTime;
        
        // In a real implementation, this would check for collisions with objects
        // For now, we'll just expire after lifetime
    }
    
    void Projectile::render() {
        // Render projectile
        // In a real implementation, this would render a mesh or particle effect
    }
    
    void Projectile::onHit(GameObject* target) {
        if (m_hasHit) return;
        
        m_hasHit = true;
        
        // Apply damage to target
        // In a real implementation, this would interact with a damage system
        if (target) {
            // Damage application would go here
        }
    }
    
    // MuzzleFlash implementation
    MuzzleFlash::MuzzleFlash(float duration, const glm::vec3& color)
        : m_duration(duration)
        , m_elapsedTime(0.0f)
        , m_color(color)
        , m_isActive(false) {
    }
    
    void MuzzleFlash::initialize() {
        // Initialize muzzle flash
    }
    
    void MuzzleFlash::update(float deltaTime) {
        if (!m_isActive) return;
        
        m_elapsedTime += deltaTime;
        
        if (m_elapsedTime >= m_duration) {
            m_isActive = false;
            m_elapsedTime = 0.0f;
        }
    }
    
    void MuzzleFlash::destroy() {
        // Cleanup muzzle flash
    }
    
    void MuzzleFlash::render() {
        if (!m_isActive) return;
        
        // Render muzzle flash effect
        // In a real implementation, this would render a particle effect or sprite
    }
    
    void MuzzleFlash::activate() {
        m_isActive = true;
        m_elapsedTime = 0.0f;
    }
    
    // RecoilSystem implementation
    RecoilSystem::RecoilSystem()
        : m_recoilAmount(1.0f)
        , m_recoilRecovery(2.0f)
        , m_patternIndex(0)
        , m_recoilTimer(0.0f) {
        m_currentRecoil = glm::vec2(0.0f, 0.0f);
        
        // Default recoil pattern (simple vertical recoil)
        m_recoilPattern.push_back(glm::vec2(0.0f, 1.0f));
        m_recoilPattern.push_back(glm::vec2(0.2f, 0.8f));
        m_recoilPattern.push_back(glm::vec2(-0.1f, 0.9f));
        m_recoilPattern.push_back(glm::vec2(0.1f, 0.7f));
        m_recoilPattern.push_back(glm::vec2(0.0f, 0.6f));
    }
    
    void RecoilSystem::initialize() {
        // Initialize recoil system
    }
    
    void RecoilSystem::update(float deltaTime) {
        // Recover from recoil over time
        if (m_currentRecoil.y > 0.0f) {
            m_currentRecoil.y -= m_recoilRecovery * deltaTime;
            if (m_currentRecoil.y < 0.0f) m_currentRecoil.y = 0.0f;
        }
        
        if (m_currentRecoil.x > 0.0f) {
            m_currentRecoil.x -= m_recoilRecovery * deltaTime;
            if (m_currentRecoil.x < 0.0f) m_currentRecoil.x = 0.0f;
        }
        
        if (m_currentRecoil.x < 0.0f) {
            m_currentRecoil.x += m_recoilRecovery * deltaTime;
            if (m_currentRecoil.x > 0.0f) m_currentRecoil.x = 0.0f;
        }
    }
    
    void RecoilSystem::destroy() {
        // Cleanup recoil system
    }
    
    void RecoilSystem::render() {
        // Recoil system doesn't render directly
    }
    
    void RecoilSystem::applyRecoil() {
        if (!m_recoilPattern.empty()) {
            glm::vec2 recoil = m_recoilPattern[m_patternIndex];
            m_currentRecoil += recoil * m_recoilAmount;
            
            // Move to next pattern point
            m_patternIndex = (m_patternIndex + 1) % m_recoilPattern.size();
        }
    }
    
    // AdvancedWeapon implementation
    AdvancedWeapon::AdvancedWeapon(const std::string& name, WeaponType type)
        : m_damage(10.0f)
        , m_fireRate(10.0f)  // Shots per second
        , m_magazineSize(30)
        , m_ammoCount(30)
        , m_maxAmmo(120)
        , m_spread(0.1f)
        , m_recoil(1.0f)
        , m_range(100.0f)
        , m_fireMode(FireMode::SINGLE)
        , m_burstCount(3)
        , m_burstShotsFired(0)
        , m_state(WeaponState::IDLE)
        , m_stateTimer(0.0f)
        , m_lastFireTime(0.0f)
        , m_muzzleFlash(nullptr)
        , m_recoilSystem(nullptr) {
    }
    
    void AdvancedWeapon::initialize() {
        // Create components
        m_muzzleFlash = owner->addComponent<MuzzleFlash>(0.1f, glm::vec3(1.0f, 1.0f, 0.0f));  // Yellow flash
        m_recoilSystem = owner->addComponent<RecoilSystem>();
        
        // Initialize attachments
        for (auto& attachment : m_attachments) {
            attachment->initialize();
        }
    }
    
    void AdvancedWeapon::update(float deltaTime) {
        // Update components
        if (m_muzzleFlash) m_muzzleFlash->update(deltaTime);
        if (m_recoilSystem) m_recoilSystem->update(deltaTime);
        
        // Update weapon state
        updateState(deltaTime);
        
        // Update attachments
        for (auto& attachment : m_attachments) {
            attachment->update(deltaTime);
        }
    }
    
    void AdvancedWeapon::destroy() {
        // Cleanup weapon
        for (auto& attachment : m_attachments) {
            attachment->destroy();
        }
    }
    
    void AdvancedWeapon::render() {
        // Render weapon
        if (m_muzzleFlash) m_muzzleFlash->render();
        
        for (auto& attachment : m_attachments) {
            attachment->render();
        }
    }
    
    void AdvancedWeapon::addAttachment(std::unique_ptr<WeaponAttachment> attachment) {
        if (attachment) {
            m_attachments.push_back(std::move(attachment));
        }
    }
    
    WeaponAttachment* AdvancedWeapon::getAttachment(AttachmentType type) const {
        for (const auto& attachment : m_attachments) {
            if (attachment->getType() == type) {
                return attachment.get();
            }
        }
        return nullptr;
    }
    
    void AdvancedWeapon::removeAttachment(AttachmentType type) {
        m_attachments.erase(
            std::remove_if(m_attachments.begin(), m_attachments.end(),
                [type](const std::unique_ptr<WeaponAttachment>& attachment) {
                    return attachment->getType() == type;
                }),
            m_attachments.end()
        );
    }
    
    bool AdvancedWeapon::fire(const glm::vec3& direction) {
        if (m_state != WeaponState::IDLE && m_state != WeaponState::FIRING) return false;
        if (m_ammoCount <= 0) return false;
        
        float currentTime = 0.0f; // In a real implementation, this would be actual time
        float fireInterval = 1.0f / getEffectiveFireRate();
        
        if (currentTime - m_lastFireTime < fireInterval) return false;
        
        // Handle different fire modes
        bool canFire = false;
        
        switch (m_fireMode) {
            case FireMode::SINGLE:
                canFire = true;
                break;
                
            case FireMode::BURST:
                if (m_burstShotsFired < m_burstCount) {
                    canFire = true;
                    m_burstShotsFired++;
                } else {
                    // Reset burst counter after a short delay
                    // In a real implementation, this would be time-based
                    m_burstShotsFired = 0;
                }
                break;
                
            case FireMode::FULL_AUTO:
                canFire = true;
                break;
        }
        
        if (!canFire) return false;
        
        // Fire the weapon
        m_ammoCount--;
        m_lastFireTime = currentTime;
        m_state = WeaponState::FIRING;
        m_stateTimer = 0.1f; // Short firing animation time
        
        // Apply recoil
        if (m_recoilSystem) {
            m_recoilSystem->applyRecoil();
        }
        
        // Create muzzle flash
        if (m_muzzleFlash) {
            m_muzzleFlash->activate();
        }
        
        // Calculate spread
        glm::vec3 spreadDirection = calculateSpread(direction);
        
        // Create projectile
        if (owner) {
            glm::vec3 startPosition = owner->getPosition() + glm::vec3(0.0f, 1.0f, 0.0f); // Offset from owner
            createProjectile(startPosition, spreadDirection * 100.0f); // Scale velocity
        }
        
        return true;
    }
    
    void AdvancedWeapon::reload() {
        if (m_state == WeaponState::RELOADING) return;
        if (m_ammoCount >= m_magazineSize) return;
        if (m_maxAmmo <= 0) return;
        
        m_state = WeaponState::RELOADING;
        m_stateTimer = 2.0f; // 2 second reload time
    }
    
    void AdvancedWeapon::inspect() {
        if (m_state != WeaponState::IDLE) return;
        
        m_state = WeaponState::INSPECTING;
        m_stateTimer = 3.0f; // 3 second inspect time
    }
    
    float AdvancedWeapon::getEffectiveDamage() const {
        float effectiveDamage = m_damage;
        
        // Apply attachment modifiers
        for (const auto& attachment : m_attachments) {
            effectiveDamage *= attachment->getDamageModifier();
        }
        
        return effectiveDamage;
    }
    
    float AdvancedWeapon::getEffectiveFireRate() const {
        float effectiveFireRate = m_fireRate;
        
        // Apply attachment modifiers
        for (const auto& attachment : m_attachments) {
            effectiveFireRate *= attachment->getFireRateModifier();
        }
        
        return effectiveFireRate;
    }
    
    float AdvancedWeapon::getEffectiveAccuracy() const {
        float effectiveAccuracy = 1.0f - m_spread;
        
        // Apply attachment modifiers
        for (const auto& attachment : m_attachments) {
            effectiveAccuracy *= attachment->getAccuracyModifier();
        }
        
        return effectiveAccuracy;
    }
    
    float AdvancedWeapon::getEffectiveRecoil() const {
        float effectiveRecoil = m_recoil;
        
        // Apply attachment modifiers
        for (const auto& attachment : m_attachments) {
            effectiveRecoil *= attachment->getRecoilModifier();
        }
        
        return effectiveRecoil;
    }
    
    float AdvancedWeapon::getEffectiveRange() const {
        float effectiveRange = m_range;
        
        // Apply attachment modifiers
        for (const auto& attachment : m_attachments) {
            effectiveRange *= attachment->getRangeModifier();
        }
        
        return effectiveRange;
    }
    
    void AdvancedWeapon::updateFiring(float deltaTime) {
        if (m_state == WeaponState::FIRING) {
            m_stateTimer -= deltaTime;
            
            if (m_stateTimer <= 0.0f) {
                m_state = WeaponState::IDLE;
            }
        }
    }
    
    void AdvancedWeapon::updateReloading(float deltaTime) {
        if (m_state == WeaponState::RELOADING) {
            m_stateTimer -= deltaTime;
            
            if (m_stateTimer <= 0.0f) {
                // Reload complete
                int ammoNeeded = m_magazineSize - m_ammoCount;
                int ammoToLoad = std::min(ammoNeeded, m_maxAmmo);
                
                m_ammoCount += ammoToLoad;
                m_maxAmmo -= ammoToLoad;
                
                m_state = WeaponState::IDLE;
            }
        }
    }
    
    void AdvancedWeapon::updateState(float deltaTime) {
        updateFiring(deltaTime);
        updateReloading(deltaTime);
        
        // Update other states
        if (m_state == WeaponState::INSPECTING) {
            m_stateTimer -= deltaTime;
            
            if (m_stateTimer <= 0.0f) {
                m_state = WeaponState::IDLE;
            }
        }
        
        if (m_state == WeaponState::EQUIPPING) {
            m_stateTimer -= deltaTime;
            
            if (m_stateTimer <= 0.0f) {
                m_state = WeaponState::IDLE;
            }
        }
        
        if (m_state == WeaponState::UNEQUIPPING) {
            m_stateTimer -= deltaTime;
            
            if (m_stateTimer <= 0.0f) {
                m_state = WeaponState::IDLE;
            }
        }
    }
    
    glm::vec3 AdvancedWeapon::calculateSpread(const glm::vec3& direction) const {
        float effectiveSpread = m_spread;
        
        // Apply attachment modifiers
        for (const auto& attachment : m_attachments) {
            effectiveSpread *= attachment->getSpreadModifier();
        }
        
        // If no spread, return original direction
        if (effectiveSpread <= 0.0f) return direction;
        
        // Generate random spread
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<float> dis(-1.0f, 1.0f);
        
        glm::vec3 spreadOffset(
            dis(gen) * effectiveSpread,
            dis(gen) * effectiveSpread,
            dis(gen) * effectiveSpread
        );
        
        return glm::normalize(direction + spreadOffset);
    }
    
    void AdvancedWeapon::createProjectile(const glm::vec3& position, const glm::vec3& velocity) {
        // In a real implementation, this would create a projectile object
        // and add it to the game world for physics simulation
        auto projectile = std::make_unique<Projectile>(position, velocity, getEffectiveDamage(), 5.0f);
        
        // Add to game world or projectile manager
        // For now, we'll just let it go out of scope
    }
    
    // AdvancedWeaponSystem implementation
    AdvancedWeaponSystem::AdvancedWeaponSystem()
        : WeaponSystem(nullptr)
        , m_currentWeapon(nullptr) {
        // Initialize ammo reserves
        m_ammoReserves.insert({WeaponType::PISTOL, 120});
        m_ammoReserves.insert({WeaponType::RIFLE, 180});
        m_ammoReserves.insert({WeaponType::SHOTGUN, 60});
        m_ammoReserves.insert({WeaponType::SNIPER, 30});
        m_ammoReserves.insert({WeaponType::SMG, 240});
        m_ammoReserves.insert({WeaponType::HEAVY, 50});
    }
    
    AdvancedWeaponSystem::AdvancedWeaponSystem(Player* player)
        : WeaponSystem(player)
        , m_currentWeapon(nullptr) {
        // Initialize ammo reserves
        m_ammoReserves.insert({WeaponType::PISTOL, 120});
        m_ammoReserves.insert({WeaponType::RIFLE, 180});
        m_ammoReserves.insert({WeaponType::SHOTGUN, 60});
        m_ammoReserves.insert({WeaponType::SNIPER, 30});
        m_ammoReserves.insert({WeaponType::SMG, 240});
        m_ammoReserves.insert({WeaponType::HEAVY, 50});
    }
    
    void AdvancedWeaponSystem::initialize() {
        // Initialize weapon system
        for (auto& weapon : m_weapons) {
            weapon->initialize();
        }
    }
    
    void AdvancedWeaponSystem::update(float deltaTime) {
        // Update all weapons
        for (auto& weapon : m_weapons) {
            weapon->update(deltaTime);
        }
        
        // Update weapon states
        updateWeaponStates(deltaTime);
    }
    
    void AdvancedWeaponSystem::destroy() {
        // Cleanup weapon system
        for (auto& weapon : m_weapons) {
            weapon->destroy();
        }
    }
    
    void AdvancedWeaponSystem::render() {
        // Render all weapons
        for (auto& weapon : m_weapons) {
            weapon->render();
        }
    }
    
    void AdvancedWeaponSystem::addWeapon(std::unique_ptr<AdvancedWeapon> weapon) {
        if (weapon) {
            m_weapons.push_back(std::move(weapon));
            
            // Set as current weapon if it's the first one
            if (m_weapons.size() == 1) {
                m_currentWeapon = m_weapons[0].get();
            }
        }
    }
    
    AdvancedWeapon* AdvancedWeaponSystem::getWeapon(const std::string& name) const {
        for (const auto& weapon : m_weapons) {
            if (weapon->getOwner() && weapon->getOwner()->getName() == name) {
                return weapon.get();
            }
        }
        return nullptr;
    }
    
    void AdvancedWeaponSystem::setCurrentWeapon(const std::string& name) {
        for (const auto& weapon : m_weapons) {
            if (weapon->getOwner() && weapon->getOwner()->getName() == name) {
                // Transition current weapon to unequipping state
                if (m_currentWeapon && m_currentWeapon != weapon.get()) {
                    m_currentWeapon->setState(WeaponState::UNEQUIPPING);
                    m_currentWeapon->setState(WeaponState::UNEQUIPPING);
                }
                
                // Set new weapon to equipping state
                weapon->setState(WeaponState::EQUIPPING);
                
                m_currentWeapon = weapon.get();
                break;
            }
        }
    }
    
    void AdvancedWeaponSystem::switchToNextAdvancedWeapon() {
        if (m_weapons.size() <= 1) return;
        
        // Find current weapon index
        for (size_t i = 0; i < m_weapons.size(); ++i) {
            if (m_weapons[i].get() == m_currentWeapon) {
                // Switch to next weapon
                size_t nextIndex = (i + 1) % m_weapons.size();
                m_currentWeapon = m_weapons[nextIndex].get();
                m_currentWeapon->setState(WeaponState::EQUIPPING);
                break;
            }
        }
    }
    
    void AdvancedWeaponSystem::switchToPreviousAdvancedWeapon() {
        if (m_weapons.size() <= 1) return;
        
        // Find current weapon index
        for (size_t i = 0; i < m_weapons.size(); ++i) {
            if (m_weapons[i].get() == m_currentWeapon) {
                // Switch to previous weapon
                size_t prevIndex = (i + m_weapons.size() - 1) % m_weapons.size();
                m_currentWeapon = m_weapons[prevIndex].get();
                m_currentWeapon->setState(WeaponState::EQUIPPING);
                break;
            }
        }
    }
    
    bool AdvancedWeaponSystem::fireCurrentWeapon(const glm::vec3& direction) {
        if (m_currentWeapon) {
            return m_currentWeapon->fire(direction);
        }
        return false;
    }
    
    void AdvancedWeaponSystem::reloadCurrentWeapon() {
        if (m_currentWeapon) {
            m_currentWeapon->reload();
        }
    }
    
    std::vector<std::string> AdvancedWeaponSystem::getWeaponNames() const {
        std::vector<std::string> names;
        for (const auto& weapon : m_weapons) {
            if (weapon->getOwner()) {
                names.push_back(weapon->getOwner()->getName());
            }
        }
        return names;
    }
    
    int AdvancedWeaponSystem::getTotalAmmo(WeaponType type) const {
        auto it = m_ammoReserves.find(type);
        if (it != m_ammoReserves.end()) {
            return it->second;
        }
        return 0;
    }
    
    void AdvancedWeaponSystem::addAmmo(WeaponType type, int amount) {
        m_ammoReserves[type] += amount;
    }
    
    void AdvancedWeaponSystem::updateWeaponStates(float deltaTime) {
        // Update all weapon states
        for (auto& weapon : m_weapons) {
            // This is already handled in weapon->update()
        }
    }
    
    // WeaponSystem interface implementation
    bool AdvancedWeaponSystem::equipWeapon(int inventorySlot) {
        // Not implemented for advanced weapon system
        return false;
    }
    
    bool AdvancedWeaponSystem::equipWeapon(const std::string& weaponName) {
        // Not implemented for advanced weapon system
        return false;
    }
    
    bool AdvancedWeaponSystem::unequipWeapon() {
        // Not implemented for advanced weapon system
        return false;
    }
    
    bool AdvancedWeaponSystem::switchToNextWeapon() {
        switchToNextAdvancedWeapon();
        return true;
    }
    
    bool AdvancedWeaponSystem::switchToPreviousWeapon() {
        switchToPreviousAdvancedWeapon();
        return true;
    }
    
    bool AdvancedWeaponSystem::switchToWeapon(int index) {
        if (index >= 0 && index < static_cast<int>(m_weapons.size())) {
            m_currentWeapon = m_weapons[index].get();
            return true;
        }
        return false;
    }
    
    void AdvancedWeaponSystem::shoot() {
        // Not implemented for advanced weapon system
    }
    
    void AdvancedWeaponSystem::reload() {
        reloadCurrentWeapon();
    }
    
    void AdvancedWeaponSystem::aim() {
        // Not implemented for advanced weapon system
    }
    
    void AdvancedWeaponSystem::unaim() {
        // Not implemented for advanced weapon system
    }
    
    int AdvancedWeaponSystem::getTotalAmmo() const {
        if (m_currentWeapon) {
            return m_currentWeapon->getMaxAmmo();
        }
        return 0;
    }
    
    int AdvancedWeaponSystem::getMagazineAmmo() const {
        if (m_currentWeapon) {
            return m_currentWeapon->getAmmoCount();
        }
        return 0;
    }
    
    bool AdvancedWeaponSystem::canShoot() const {
        if (m_currentWeapon) {
            return m_currentWeapon->hasAmmo() && !m_currentWeapon->isReloading();
        }
        return false;
    }
    
    bool AdvancedWeaponSystem::isReloading() const {
        if (m_currentWeapon) {
            return m_currentWeapon->isReloading();
        }
        return false;
    }
}