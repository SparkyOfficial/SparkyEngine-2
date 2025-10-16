#pragma once

#include "WeaponSystem.h"
#include "BallisticsSystem.h"
#include "GameObject.h"
#include "Component.h"
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include <functional>

namespace Sparky {
    // Forward declarations
    class AdvancedWeapon;
    class Projectile;
    class MuzzleFlash;
    class RecoilSystem;
    
    // Weapon types
    enum class WeaponType {
        PISTOL,
        RIFLE,
        SHOTGUN,
        SNIPER,
        SMG,
        HEAVY
    };
    
    // Weapon attachment types
    enum class AttachmentType {
        SCOPE,
        SILENCER,
        GRIP,
        MAGAZINE,
        BARREL,
        UNDERBARREL
    };
    
    // Fire modes
    enum class FireMode {
        SINGLE,
        BURST,
        FULL_AUTO
    };
    
    // Weapon states
    enum class WeaponState {
        IDLE,
        FIRING,
        RELOADING,
        INSPECTING,
        EQUIPPING,
        UNEQUIPPING
    };
    
    // Attachment base class
    class WeaponAttachment : public Component {
    public:
        WeaponAttachment(const std::string& name, AttachmentType type);
        virtual ~WeaponAttachment() = default;
        
        // Component interface
        virtual void initialize();
        virtual void update(float deltaTime) override;
        virtual void destroy();
        virtual void render() override;
        
        // Attachment properties
        const std::string& getName() const { return m_name; }
        AttachmentType getType() const { return m_type; }
        
        // Modifier methods
        virtual float getDamageModifier() const { return 1.0f; }
        virtual float getAccuracyModifier() const { return 1.0f; }
        virtual float getFireRateModifier() const { return 1.0f; }
        virtual float getRecoilModifier() const { return 1.0f; }
        virtual float getRangeModifier() const { return 1.0f; }
        virtual float getSpreadModifier() const { return 1.0f; }
        
    protected:
        std::string m_name;
        AttachmentType m_type;
    };
    
    // Scope attachment
    class ScopeAttachment : public WeaponAttachment {
    public:
        ScopeAttachment(const std::string& name, float zoomFactor, float aimSpeedModifier);
        virtual ~ScopeAttachment() = default;
        
        virtual float getAccuracyModifier() const override { return m_accuracyModifier; }
        virtual float getAimSpeedModifier() const { return m_aimSpeedModifier; }
        
        float getZoomFactor() const { return m_zoomFactor; }
        
    private:
        float m_zoomFactor;
        float m_accuracyModifier;
        float m_aimSpeedModifier;
    };
    
    // Silencer attachment
    class SilencerAttachment : public WeaponAttachment {
    public:
        SilencerAttachment(const std::string& name, float noiseReduction, float damagePenalty);
        virtual ~SilencerAttachment() = default;
        
        virtual float getNoiseModifier() const { return m_noiseReduction; }
        virtual float getDamageModifier() const override { return m_damagePenalty; }
        
    private:
        float m_noiseReduction;
        float m_damagePenalty;
    };
    
    // Projectile class
    class Projectile : public GameObject {
    public:
        Projectile(const glm::vec3& position, const glm::vec3& velocity, float damage, float lifetime);
        virtual ~Projectile() = default;
        
        virtual void update(float deltaTime) override;
        virtual void render() override;
        
        // Projectile properties
        void setDamage(float damage) { m_damage = damage; }
        float getDamage() const { return m_damage; }
        
        void setVelocity(const glm::vec3& velocity) { m_velocity = velocity; }
        glm::vec3 getVelocity() const { return m_velocity; }
        
        void setLifetime(float lifetime) { m_lifetime = lifetime; }
        float getLifetime() const { return m_lifetime; }
        
        bool isExpired() const { return m_lifetime <= 0.0f; }
        
        // Collision handling
        void onHit(GameObject* target);
        
    private:
        glm::vec3 m_velocity;
        float m_damage;
        float m_lifetime;
        bool m_hasHit;
    };
    
    // Muzzle flash effect
    class MuzzleFlash : public Component {
    public:
        MuzzleFlash(float duration, const glm::vec3& color);
        virtual ~MuzzleFlash() = default;
        
        virtual void initialize();
        virtual void update(float deltaTime) override;
        virtual void destroy();
        virtual void render() override;
        
        void activate();
        bool isActive() const { return m_isActive; }
        
    private:
        float m_duration;
        float m_elapsedTime;
        glm::vec3 m_color;
        bool m_isActive;
    };
    
    // Recoil system
    class RecoilSystem : public Component {
    public:
        RecoilSystem();
        virtual ~RecoilSystem() = default;
        
        virtual void initialize();
        virtual void update(float deltaTime) override;
        virtual void destroy();
        virtual void render() override;
        
        // Recoil properties
        void setRecoilAmount(float amount) { m_recoilAmount = amount; }
        float getRecoilAmount() const { return m_recoilAmount; }
        
        void setRecoilRecovery(float recovery) { m_recoilRecovery = recovery; }
        float getRecoilRecovery() const { return m_recoilRecovery; }
        
        // Recoil patterns
        void setRecoilPattern(const std::vector<glm::vec2>& pattern) { m_recoilPattern = pattern; }
        const std::vector<glm::vec2>& getRecoilPattern() const { return m_recoilPattern; }
        
        // Recoil application
        void applyRecoil();
        glm::vec2 getCurrentRecoil() const { return m_currentRecoil; }
        
    private:
        float m_recoilAmount;
        float m_recoilRecovery;
        std::vector<glm::vec2> m_recoilPattern;
        glm::vec2 m_currentRecoil;
        size_t m_patternIndex;
        float m_recoilTimer;
    };
    
    // Advanced weapon class
    class AdvancedWeapon : public Component {
    public:
        AdvancedWeapon(const std::string& name, WeaponType type);
        virtual ~AdvancedWeapon() = default;
        
        // Component interface
        virtual void initialize();
        virtual void update(float deltaTime) override;
        virtual void destroy();
        virtual void render() override;
        
        // Weapon properties
        void setDamage(float damage) { m_damage = damage; }
        float getDamage() const { return m_damage; }
        
        void setFireRate(float fireRate) { m_fireRate = fireRate; }
        float getFireRate() const { return m_fireRate; }
        
        void setMagazineSize(int size) { m_magazineSize = size; }
        int getMagazineSize() const { return m_magazineSize; }
        
        void setAmmoCount(int ammo) { m_ammoCount = ammo; }
        int getAmmoCount() const { return m_ammoCount; }
        
        void setMaxAmmo(int maxAmmo) { m_maxAmmo = maxAmmo; }
        int getMaxAmmo() const { return m_maxAmmo; }
        
        void setSpread(float spread) { m_spread = spread; }
        float getSpread() const { return m_spread; }
        
        void setRecoil(float recoil) { m_recoil = recoil; }
        float getRecoil() const { return m_recoil; }
        
        void setRange(float range) { m_range = range; }
        float getRange() const { return m_range; }
        
        // Fire modes
        void setFireMode(FireMode mode) { m_fireMode = mode; }
        FireMode getFireMode() const { return m_fireMode; }
        
        void setBurstCount(int count) { m_burstCount = count; }
        int getBurstCount() const { return m_burstCount; }
        
        // Weapon state
        void setState(WeaponState state) { m_state = state; }
        WeaponState getState() const { return m_state; }
        
        // Attachments
        void addAttachment(std::unique_ptr<WeaponAttachment> attachment);
        WeaponAttachment* getAttachment(AttachmentType type) const;
        void removeAttachment(AttachmentType type);
        
        // Weapon actions
        bool fire(const glm::vec3& direction);
        void reload();
        void inspect();
        
        // Ammo management
        bool hasAmmo() const { return m_ammoCount > 0; }
        bool isReloading() const { return m_state == WeaponState::RELOADING; }
        bool isFiring() const { return m_state == WeaponState::FIRING; }
        
        // Getters for calculated values with attachments
        float getEffectiveDamage() const;
        float getEffectiveFireRate() const;
        float getEffectiveAccuracy() const;
        float getEffectiveRecoil() const;
        float getEffectiveRange() const;
        
        // Components
        MuzzleFlash* getMuzzleFlash() const { return m_muzzleFlash; }
        RecoilSystem* getRecoilSystem() const { return m_recoilSystem; }
        
    private:
        // Basic weapon properties
        float m_damage;
        float m_fireRate;
        int m_magazineSize;
        int m_ammoCount;
        int m_maxAmmo;
        float m_spread;
        float m_recoil;
        float m_range;
        
        // Fire mode properties
        FireMode m_fireMode;
        int m_burstCount;
        int m_burstShotsFired;
        
        // Weapon state
        WeaponState m_state;
        float m_stateTimer;
        float m_lastFireTime;
        
        // Attachments
        std::vector<std::unique_ptr<WeaponAttachment>> m_attachments;
        
        // Components
        MuzzleFlash* m_muzzleFlash;
        RecoilSystem* m_recoilSystem;
        
        // Internal methods
        void updateFiring(float deltaTime);
        void updateReloading(float deltaTime);
        void updateState(float deltaTime);
        glm::vec3 calculateSpread(const glm::vec3& direction) const;
        void createProjectile(const glm::vec3& position, const glm::vec3& velocity);
    };
    
    // Advanced weapon system
    class AdvancedWeaponSystem : public WeaponSystem {
    public:
        AdvancedWeaponSystem(Player* player);
        virtual ~AdvancedWeaponSystem() = default;
        
        // WeaponSystem interface
        virtual void initialize();
        virtual void update(float deltaTime);
        virtual void destroy();
        
        // Weapon management
        void addWeapon(std::unique_ptr<AdvancedWeapon> weapon);
        AdvancedWeapon* getWeapon(const std::string& name) const;
        AdvancedWeapon* getCurrentWeapon() const { return m_currentWeapon; }
        void setCurrentWeapon(const std::string& name);
        void switchToNextWeapon();
        void switchToPreviousWeapon();
        
        // Weapon actions
        bool fireCurrentWeapon(const glm::vec3& direction);
        void reloadCurrentWeapon();
        
        // Inventory management
        std::vector<std::string> getWeaponNames() const;
        int getWeaponCount() const { return static_cast<int>(m_weapons.size()); }
        
        // Ammo management
        int getTotalAmmo(WeaponType type) const;
        void addAmmo(WeaponType type, int amount);
        
    private:
        std::vector<std::unique_ptr<AdvancedWeapon>> m_weapons;
        AdvancedWeapon* m_currentWeapon;
        std::unordered_map<WeaponType, int> m_ammoReserves;
        
        // Internal methods
        void updateWeaponStates(float deltaTime);
    };
}