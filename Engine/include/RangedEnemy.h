#pragma once

#include "Enemy.h"

namespace Sparky {
    class Player;
    
    class RangedEnemy : public Enemy {
    public:
        RangedEnemy(const std::string& name = "RangedEnemy");
        virtual ~RangedEnemy();

        virtual void performAI(float deltaTime, Player* player) override;
        
        // Ranged enemy specific methods
        void setProjectileSpeed(float speed) { projectileSpeed = speed; }
        float getProjectileSpeed() const { return projectileSpeed; }
        
        void setAccuracy(float accuracy) { this->accuracy = accuracy; }
        float getAccuracy() const { return accuracy; }
        
        void setReloadTime(float time) { reloadTime = time; }
        float getReloadTime() const { return reloadTime; }
        
        bool isReloading() const { return reloading; }

    private:
        float projectileSpeed;
        float accuracy; // 0.0 to 1.0
        float reloadTime;
        float lastShotTime;
        bool reloading;
    };
}