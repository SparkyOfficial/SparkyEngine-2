#pragma once

#include "../../Engine/include/GameObject.h"
#include "../../Engine/include/Camera.h"

namespace Sparky {
    
    // Forward declarations
    class GunImpl; // Pimpl idiom to hide implementation details
    
    class Gun : public GameObject {
    public:
        Gun();
        ~Gun();

        void update(float deltaTime) override;
        void render() override;

        // Weapon functions
        void shoot();
        void reload();
        void aim();
        void unaim();
        bool canShoot() const;
        void applyRecoil();
        
        // Burst fire control
        void startBurst();
        void updateBurst(float deltaTime);
        
        // Getters and setters
        int getAmmo() const;
        int getMagazineSize() const;
        int getTotalAmmo() const;
        float getFireRate() const;
        float getSpread() const;
        float getDamage() const;
        bool isAiming() const;
        int getFiringMode() const; // Returns int instead of enum
        int getAmmoType() const; // Returns int instead of enum
        
        void setAmmo(int ammo);
        void setMagazineSize(int size);
        void setTotalAmmo(int ammo);
        void setFireRate(float rate);
        void setSpread(float sp);
        void setDamage(float dmg);
        void setFiringMode(int mode); // Takes int instead of enum
        void setAmmoType(int type); // Takes int instead of enum
        
        void setCamera(Camera* camera);
        Camera* getCamera() const;
        
        // Advanced weapon properties
        float getRecoil() const;
        void setRecoil(float recoil);
        
        float getRecoilRecovery() const;
        void setRecoilRecovery(float recovery);
        
        float getMuzzleVelocity() const;
        void setMuzzleVelocity(float velocity);
        
        const char* getWeaponType() const;
        void setWeaponType(const char* type);
        
        // Recoil pattern system
        void getRecoilPattern(float* pattern) const; // Returns 5 floats
        void setRecoilPattern(const float* pattern); // Takes 5 floats
        
        // Weapon attachments
        void addAttachment(const char* name, float modifier, bool active);
        void removeAttachment(const char* name);
        void toggleAttachment(const char* name);
        int getAttachmentCount() const;
        
        // Weapon statistics
        void getStats(int* stats) const; // Returns 5 ints + 2 floats
        void resetStats();
        
        // Heat and wear system
        float getHeat() const;
        float getWear() const;
        bool isOverheated() const;
        void coolDown(float deltaTime);
        
        // Jamming system
        bool isJammed() const;
        void jam();
        void unjam();
        
        // Reload mechanics
        float getReloadTime() const;
        void setReloadTime(float time);
        
        // Burst fire settings
        int getBurstCount() const;
        void setBurstCount(int count);
        int getCurrentBurstShot() const;
        
        // Audio
        void playShootSound();

    private:
        GunImpl* impl; // Pointer to implementation
    };
}