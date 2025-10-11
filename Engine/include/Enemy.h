#pragma once

#include "GameObject.h"
#include <string>

namespace Sparky {
    // Forward declarations
    class HealthComponent;
    class Player;
    class AIComponent;
    
    enum class EnemyType {
        BASIC,
        FAST,
        TANK,
        RANGED,
        BOSS
    };
    
    class Enemy : public GameObject {
    public:
        Enemy(const std::string& name = "Enemy", EnemyType type = EnemyType::BASIC);
        virtual ~Enemy();

        virtual void update(float deltaTime) override;
        virtual void render() override;
        
        // AI behavior
        virtual void performAI(float deltaTime, Player* player) = 0;
        
        // Combat
        virtual void takeDamage(float damage);
        virtual void attack(Player* player);
        virtual bool isAlive() const;
        
        // Getters and setters
        HealthComponent* getHealthComponent() { return healthComponent; }
        const HealthComponent* getHealthComponent() const { return healthComponent; }
        
        EnemyType getEnemyType() const { return type; }
        void setEnemyType(EnemyType enemyType) { type = enemyType; }
        
        float getMoveSpeed() const { return moveSpeed; }
        void setMoveSpeed(float speed) { moveSpeed = speed; }
        
        float getAttackDamage() const { return attackDamage; }
        void setAttackDamage(float damage) { attackDamage = damage; }
        
        float getAttackRange() const { return attackRange; }
        void setAttackRange(float range) { attackRange = range; }
        
        float getAttackRate() const { return attackRate; }
        void setAttackRate(float rate) { attackRate = rate; }
        
        float getDetectionRange() const { return detectionRange; }
        void setDetectionRange(float range) { detectionRange = range; }

    protected:
        EnemyType type;
        HealthComponent* healthComponent;
        AIComponent* aiComponent;
        
        // Enemy properties
        float moveSpeed;
        float attackDamage;
        float attackRange;
        float attackRate;
        float detectionRange;
        float lastAttackTime;
        
        // Helper methods
        float distanceToPlayer(Player* player) const;
    };
}