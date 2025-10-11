#include "../include/BossEnemy.h"
#include "../include/Player.h"
#include "../include/Logger.h"
#include "../include/FastEnemy.h"
#include "../include/HealthComponent.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

namespace Sparky {

    BossEnemy::BossEnemy(const std::string& name) : 
        Enemy(name, EnemyType::BOSS), currentPhase(BossPhase::NORMAL), 
        enrageThreshold(0.5f), specialAttackCooldown(10.0f), 
        lastSpecialAttackTime(0.0f) {
        
        // Boss enemies have special properties
        setMoveSpeed(1.5f);
        setAttackDamage(50.0f);
        setAttackRange(3.0f);
        setAttackRate(0.5f);
        setDetectionRange(25.0f);
    }

    BossEnemy::~BossEnemy() {
    }

    void BossEnemy::performAI(float deltaTime, Player* player) {
        if (!player || !isAlive()) {
            return;
        }
        
        // Check for phase transitions based on health
        float healthPercentage = getHealthComponent()->getHealth() / getHealthComponent()->getMaxHealth();
        
        if (healthPercentage <= 0.2f && currentPhase != BossPhase::FINAL) {
            currentPhase = BossPhase::FINAL;
            Logger::getInstance().info("Boss entering FINAL phase!");
        } else if (healthPercentage <= enrageThreshold && currentPhase == BossPhase::NORMAL) {
            currentPhase = BossPhase::ENRAGED;
            Logger::getInstance().info("Boss entering ENRAGED phase!");
        }
        
        // Perform phase-specific AI
        switch (currentPhase) {
            case BossPhase::NORMAL:
                performNormalPhaseAI(deltaTime, player);
                break;
            case BossPhase::ENRAGED:
                performEnragedPhaseAI(deltaTime, player);
                break;
            case BossPhase::FINAL:
                performFinalPhaseAI(deltaTime, player);
                break;
        }
        
        // Handle special attacks
        lastSpecialAttackTime += deltaTime;
        if (lastSpecialAttackTime >= specialAttackCooldown) {
            // Randomly choose a special attack
            int attackType = rand() % 3;
            switch (attackType) {
                case 0:
                    performAreaAttack();
                    break;
                case 1:
                    performSummonMinions();
                    break;
                case 2:
                    performChargeAttack(player);
                    break;
            }
            lastSpecialAttackTime = 0.0f;
        }
    }
    
    void BossEnemy::takeDamage(float damage) {
        // Bosses might have damage resistance or weaknesses
        float actualDamage = damage;
        
        switch (currentPhase) {
            case BossPhase::NORMAL:
                actualDamage = damage * 0.8f; // 20% damage reduction
                break;
            case BossPhase::ENRAGED:
                actualDamage = damage * 1.2f; // 20% damage increase (taking more damage when enraged)
                break;
            case BossPhase::FINAL:
                actualDamage = damage * 1.5f; // 50% damage increase (critical phase)
                break;
        }
        
        Enemy::takeDamage(actualDamage);
    }
    
    void BossEnemy::performNormalPhaseAI(float deltaTime, Player* player) {
        float distance = distanceToPlayer(player);
        
        if (distance <= getAttackRange()) {
            attack(player);
        } else if (distance <= getDetectionRange()) {
            Logger::getInstance().debug("Boss moving toward player");
        }
    }
    
    void BossEnemy::performEnragedPhaseAI(float deltaTime, Player* player) {
        float distance = distanceToPlayer(player);
        
        // Faster and more aggressive in enraged phase
        if (distance <= getAttackRange() * 1.5f) {
            attack(player);
        } else if (distance <= getDetectionRange()) {
            Logger::getInstance().debug("Boss rushing toward player (enraged)");
        }
    }
    
    void BossEnemy::performFinalPhaseAI(float deltaTime, Player* player) {
        float distance = distanceToPlayer(player);
        
        // Desperate attacks in final phase
        if (distance <= getAttackRange() * 2.0f) {
            attack(player);
        } else if (distance <= getDetectionRange()) {
            Logger::getInstance().debug("Boss making final desperate moves");
        }
    }
    
    void BossEnemy::performAreaAttack() {
        Logger::getInstance().info("Boss performs area attack!");
    }
    
    void BossEnemy::performSummonMinions() {
        Logger::getInstance().info("Boss summons minions!");
        
        // Create some fast enemies as minions
        for (int i = 0; i < 3; ++i) {
            auto minion = std::make_shared<FastEnemy>("Minion" + std::to_string(i));
            minions.push_back(minion);
        }
    }
    
    void BossEnemy::performChargeAttack(Player* player) {
        if (!player) return;
        Logger::getInstance().info("Boss performs charge attack!");
    }
}