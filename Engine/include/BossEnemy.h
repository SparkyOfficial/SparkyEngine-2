#pragma once

#include "Enemy.h"
#include <vector>

namespace Sparky {
    class Player;
    
    enum class BossPhase {
        NORMAL,
        ENRAGED,
        FINAL
    };
    
    class BossEnemy : public Enemy {
    public:
        BossEnemy(const std::string& name = "BossEnemy");
        virtual ~BossEnemy();

        virtual void performAI(float deltaTime, Player* player) override;
        virtual void takeDamage(float damage) override;
        
        // Boss enemy specific methods
        void setPhase(BossPhase phase) { currentPhase = phase; }
        BossPhase getPhase() const { return currentPhase; }
        
        void setEnrageThreshold(float threshold) { enrageThreshold = threshold; }
        float getEnrageThreshold() const { return enrageThreshold; }
        
        void setSpecialAttackCooldown(float cooldown) { specialAttackCooldown = cooldown; }
        float getSpecialAttackCooldown() const { return specialAttackCooldown; }
        
        void addMinion(std::shared_ptr<Enemy> minion) { minions.push_back(minion); }
        const std::vector<std::shared_ptr<Enemy>>& getMinions() const { return minions; }

    private:
        BossPhase currentPhase;
        float enrageThreshold;
        float specialAttackCooldown;
        float lastSpecialAttackTime;
        std::vector<std::shared_ptr<Enemy>> minions;
        
        // Phase-specific behaviors
        void performNormalPhaseAI(float deltaTime, Player* player);
        void performEnragedPhaseAI(float deltaTime, Player* player);
        void performFinalPhaseAI(float deltaTime, Player* player);
        
        // Special attacks
        void performAreaAttack();
        void performSummonMinions();
        void performChargeAttack(Player* player);
    };
}