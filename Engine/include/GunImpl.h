#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>

namespace Sparky {
    
    // Forward declarations
    class BallisticsSystem;
    struct Bullet;
    
    // Structure for weapon attachments
    struct WeaponAttachment {
        std::string name;
        float modifier;
        bool active;
    };
    
    // Structure for weapon statistics
    struct WeaponStats {
        int shotsFired;
        int shotsHit;
        int headshots;
        int reloads;
        int jams;
        float accuracy;
        float avgDamage;
    };
    
    // Structure for recoil pattern
    struct RecoilPattern {
        float verticalRecoil;
        float horizontalRecoil;
        float lateralRecoil;
        float recoverySpeed;
        float punchAngle;
    };
    
    // Firing modes
    enum class FiringMode {
        SEMI_AUTO = 0,
        FULL_AUTO = 1,
        BURST = 2
    };
    
    // Ammo types
    enum class AmmoType {
        REGULAR = 0,
        ARMOR_PIERCING = 1,
        EXPLOSIVE = 2,
        INCENDIARY = 3
    };
    
    // Ammo properties
    struct AmmoProperties {
        float mass;              // Mass of the bullet (kg)
        float muzzleVelocity;    // Muzzle velocity (m/s)
        float damage;            // Base damage
        float penetration;       // Armor penetration factor
        float explosiveRadius;   // Radius for explosive ammo
        float fireRateModifier;  // Modifier to fire rate
        float recoilModifier;    // Modifier to recoil
        
        AmmoProperties() : mass(0.01f), muzzleVelocity(500.0f), damage(10.0f),
                          penetration(1.0f), explosiveRadius(0.0f),
                          fireRateModifier(1.0f), recoilModifier(1.0f) {}
    };
    
    class Camera;
    
    class GunImpl {
    public:
        GunImpl();
        ~GunImpl();
        
        // Basic weapon properties
        int currentAmmo;
        int magazineSize;
        int totalAmmo;
        float fireRate;
        float spread;
        float damage;
        float lastShotTime;
        bool isReloading;
        float reloadTime;
        float lastReloadTime;
        bool isAiming;
        
        // Advanced weapon properties
        float recoil;
        float recoilRecovery;
        float currentRecoil;
        float muzzleVelocity;
        std::string weaponType;
        
        // Recoil pattern system
        RecoilPattern recoilPattern;
        
        // Weapon attachments
        std::unordered_map<std::string, WeaponAttachment> attachments;
        
        // Weapon statistics
        WeaponStats stats;
        
        // Heat and wear system
        float heat;
        float wear;
        bool overheated;
        float lastHeatTime;
        
        // Jamming system
        bool jammed;
        float jamProbability;
        
        // Burst fire settings
        FiringMode firingMode;
        AmmoType ammoType;
        int burstCount;
        int currentBurstShot;
        float burstDelay;
        float lastBurstShotTime;
        
        // Camera reference
        Camera* camera;
        
        // Ammo properties
        AmmoProperties ammoProperties;
        
        // Methods
        void update(float deltaTime);
        void shoot();
        void reload();
        void aim();
        void unaim();
        bool canShoot() const;
        void applyRecoil();
        void calculateSpreadDirection(float* direction) const;
        
        // Burst fire control
        void startBurst();
        void updateBurst(float deltaTime);
        
        // Heat and wear system
        void updateHeat(float deltaTime);
        void coolDown(float deltaTime);
        
        // Jamming system
        void updateJamming();
        void jam();
        void unjam();
        
        // Weapon attachments
        void addAttachment(const std::string& name, float modifier, bool active);
        void removeAttachment(const std::string& name);
        void toggleAttachment(const std::string& name);
        
        // Weapon statistics
        void updateStats();
        void resetStats();
        
        // Getters and setters
        int getAmmo() const { return currentAmmo; }
        int getMagazineSize() const { return magazineSize; }
        int getTotalAmmo() const { return totalAmmo; }
        float getFireRate() const { return fireRate; }
        float getSpread() const { return spread; }
        float getDamage() const { return damage; }
        bool getIsAiming() const { return isAiming; }
        int getFiringMode() const { return static_cast<int>(firingMode); }
        int getAmmoType() const { return static_cast<int>(ammoType); }
        
        void setAmmo(int ammo) { currentAmmo = ammo; }
        void setMagazineSize(int size) { magazineSize = size; }
        void setTotalAmmo(int ammo) { totalAmmo = ammo; }
        void setFireRate(float rate) { fireRate = rate; }
        void setSpread(float sp) { spread = sp; }
        void setDamage(float dmg) { damage = dmg; }
        void setFiringMode(int mode) { firingMode = static_cast<FiringMode>(mode); }
        void setAmmoType(int type) { ammoType = static_cast<AmmoType>(type); }
        
        void setCamera(Camera* cam) { camera = cam; }
        Camera* getCamera() const { return camera; }
        
        float getRecoil() const { return recoil; }
        void setRecoil(float rec) { recoil = rec; }
        
        float getRecoilRecovery() const { return recoilRecovery; }
        void setRecoilRecovery(float recovery) { recoilRecovery = recovery; }
        
        float getMuzzleVelocity() const { return muzzleVelocity; }
        void setMuzzleVelocity(float velocity) { muzzleVelocity = velocity; }
        
        const char* getWeaponType() const { return weaponType.c_str(); }
        void setWeaponType(const char* type) { weaponType = type; }
        
        // Ammo properties
        const AmmoProperties& getAmmoProperties() const { return ammoProperties; }
        void setAmmoProperties(const AmmoProperties& props) { ammoProperties = props; }
        
        // Recoil pattern system
        void getRecoilPattern(float* pattern) const;
        void setRecoilPattern(const float* pattern);
        
        int getAttachmentCount() const { return static_cast<int>(attachments.size()); }
        
        // Weapon statistics
        void getStats(int* statsArray) const;
        
        // Heat and wear system
        float getHeat() const { return heat; }
        float getWear() const { return wear; }
        bool isOverheated() const { return overheated; }
        
        // Jamming system
        bool isJammed() const { return jammed; }
        
        // Reload mechanics
        float getReloadTime() const { return reloadTime; }
        void setReloadTime(float time) { reloadTime = time; }
        
        // Burst fire settings
        int getBurstCount() const { return burstCount; }
        void setBurstCount(int count) { burstCount = count; }
        int getCurrentBurstShot() const { return currentBurstShot; }
        
    private:
        void createBullet(const float* direction);
        void createMuzzleFlash(const float* direction);
        void playShootSound();
    };
}