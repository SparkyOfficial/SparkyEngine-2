#include "../include/RangedEnemy.h"
#include "../include/Player.h"
#include "../include/Logger.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Sparky {

    RangedEnemy::RangedEnemy(const std::string& name) : 
        Enemy(name, EnemyType::RANGED), projectileSpeed(15.0f), accuracy(0.8f), 
        reloadTime(1.0f), lastShotTime(0.0f), reloading(false) {
        
        // Ranged enemies have special properties
        setMoveSpeed(2.0f);
        setAttackDamage(15.0f);
        setAttackRange(10.0f);
        setAttackRate(1.0f);
        setDetectionRange(20.0f);
    }

    RangedEnemy::~RangedEnemy() {
    }

    void RangedEnemy::performAI(float deltaTime, Player* player) {
        if (!player || !isAlive()) {
            return;
        }
        
        float distance = distanceToPlayer(player);
        
        // Handle reloading
        if (reloading) {
            lastShotTime += deltaTime;
            if (lastShotTime >= reloadTime) {
                reloading = false;
                lastShotTime = 0.0f;
                Logger::getInstance().debug("RangedEnemy finished reloading");
            }
            return;
        }
        
        // Ranged enemies prefer to stay at distance
        if (distance <= getAttackRange() && distance > 3.0f) {
            // Shoot at player
            lastShotTime += deltaTime;
            if (lastShotTime >= 1.0f / getAttackRate()) {
                Logger::getInstance().info("RangedEnemy shoots at player");
                lastShotTime = 0.0f;
                reloading = true;
            }
        } else if (distance <= getDetectionRange()) {
            // Move to maintain optimal distance
            if (distance < 5.0f) {
                Logger::getInstance().debug("RangedEnemy moving away from player");
            } else if (distance > 8.0f) {
                Logger::getInstance().debug("RangedEnemy moving toward player");
            }
        }
    }
}