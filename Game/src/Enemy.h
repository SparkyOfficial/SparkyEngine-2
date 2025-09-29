#pragma once

#include "../../Engine/include/GameObject.h"
#include "../../Engine/include/PhysicsComponent.h"
#include "../../Engine/include/AIComponent.h"
#include <glm/glm.hpp>

namespace Sparky {
    class Player; // Forward declaration
    
    class Enemy : public GameObject {
    public:
        Enemy();
        ~Enemy();

        void update(float deltaTime) override;
        void render() override;

        // Enemy AI functions
        void setPlayerTarget(Player* player);
        void patrol();
        void chasePlayer();
        void attackPlayer();
        void takeDamage(float damage);
        bool isAlive() const;
        
        // Getters and setters
        float getHealth() const { return health; }
        float getMaxHealth() const { return maxHealth; }
        float getSpeed() const { return speed; }
        float getAttackDamage() const { return attackDamage; }
        float getAttackRange() const { return attackRange; }
        float getAttackRate() const { return attackRate; }
        bool isAggressive() const { return aggressive; }
        
        void setHealth(float hp) { health = hp; }
        void setMaxHealth(float maxHp) { maxHealth = maxHp; }
        void setSpeed(float spd) { speed = spd; }
        void setAttackDamage(float damage) { attackDamage = damage; }
        void setAttackRange(float range) { attackRange = range; }
        void setAttackRate(float rate) { attackRate = rate; }
        void setAggressive(bool aggro) { aggressive = aggro; }

    private:
        Player* targetPlayer;
        
        // Enemy properties
        float health;
        float maxHealth;
        float speed;
        float attackDamage;
        float attackRange;
        float attackRate;
        float lastAttackTime;
        bool aggressive;
        bool alive;
        
        // AI state
        enum class AIState {
            PATROL,
            CHASE,
            ATTACK,
            DEAD
        };
        
        AIState currentState;
        
        // Private helper methods
        float distanceToPlayer() const;
        void changeState(AIState newState);
    };
}