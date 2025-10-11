#include "../include/TankEnemy.h"
#include "../include/Player.h"
#include "../include/Logger.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Sparky {

    TankEnemy::TankEnemy(const std::string& name) : 
        Enemy(name, EnemyType::TANK), armor(0.5f), stompDamage(30.0f), 
        stompRange(3.0f), lastStompTime(0.0f), stompCooldown(5.0f) {
        
        // Tank enemies have special properties
        setMoveSpeed(1.0f);
        setAttackDamage(25.0f);
        setAttackRate(0.5f);
        setDetectionRange(8.0f);
    }

    TankEnemy::~TankEnemy() {
    }

    void TankEnemy::performAI(float deltaTime, Player* player) {
        if (!player || !isAlive()) {
            return;
        }
        
        float distance = distanceToPlayer(player);
        
        // Tank enemies are slow but powerful
        if (distance <= getAttackRange()) {
            // Regular attack
            attack(player);
        } else if (distance <= stompRange) {
            // Stomp attack for close range
            lastStompTime += deltaTime;
            if (lastStompTime >= stompCooldown) {
                Logger::getInstance().info("TankEnemy performs stomp attack for " + std::to_string(stompDamage) + " damage");
                lastStompTime = 0.0f;
            }
        } else if (distance <= getDetectionRange()) {
            // Slowly move toward player
            Logger::getInstance().debug("TankEnemy moving toward player");
        }
    }
}