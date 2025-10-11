#pragma once

#include "Enemy.h"

namespace Sparky {
    class Player;
    
    class TankEnemy : public Enemy {
    public:
        TankEnemy(const std::string& name = "TankEnemy");
        virtual ~TankEnemy();

        virtual void performAI(float deltaTime, Player* player) override;
        
        // Tank enemy specific methods
        void setArmor(float armor) { this->armor = armor; }
        float getArmor() const { return armor; }
        
        void setStompDamage(float damage) { stompDamage = damage; }
        float getStompDamage() const { return stompDamage; }
        
        void setStompRange(float range) { stompRange = range; }
        float getStompRange() const { return stompRange; }

    private:
        float armor; // Damage reduction
        float stompDamage;
        float stompRange;
        float lastStompTime;
        float stompCooldown;
    };
}