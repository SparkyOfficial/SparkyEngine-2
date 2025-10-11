#include "../include/FastEnemy.h"
#include "../include/Player.h"
#include "../include/Logger.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Sparky {

    FastEnemy::FastEnemy(const std::string& name) : 
        Enemy(name, EnemyType::FAST), strafeSpeed(3.0f), jumpForce(8.0f), 
        lastJumpTime(0.0f), jumpCooldown(3.0f) {
        
        // Fast enemies have special properties
        setMoveSpeed(4.0f);
        setAttackDamage(8.0f);
        setAttackRate(1.5f);
        setDetectionRange(15.0f);
    }

    FastEnemy::~FastEnemy() {
    }

    void FastEnemy::performAI(float deltaTime, Player* player) {
        if (!player || !isAlive()) {
            return;
        }
        
        float distance = distanceToPlayer(player);
        
        // Fast enemies use hit-and-run tactics
        if (distance <= getAttackRange()) {
            // Attack if close enough
            attack(player);
        } else if (distance <= getDetectionRange()) {
            // Strafe around the player
            // In a full implementation, we would move the enemy position
            Logger::getInstance().debug("FastEnemy strafing around player");
            
            // Random jumping to avoid attacks
            lastJumpTime += deltaTime;
            if (lastJumpTime >= jumpCooldown) {
                Logger::getInstance().debug("FastEnemy jumping");
                lastJumpTime = 0.0f;
            }
        }
    }
}