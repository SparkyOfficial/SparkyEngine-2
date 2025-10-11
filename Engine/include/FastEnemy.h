#pragma once

#include "Enemy.h"

namespace Sparky {
    class Player;
    
    class FastEnemy : public Enemy {
    public:
        FastEnemy(const std::string& name = "FastEnemy");
        virtual ~FastEnemy();

        virtual void performAI(float deltaTime, Player* player) override;
        
        // Fast enemy specific methods
        void setStrafeSpeed(float speed) { strafeSpeed = speed; }
        float getStrafeSpeed() const { return strafeSpeed; }
        
        void setJumpForce(float force) { jumpForce = force; }
        float getJumpForce() const { return jumpForce; }

    private:
        float strafeSpeed;
        float jumpForce;
        float lastJumpTime;
        float jumpCooldown;
    };
}