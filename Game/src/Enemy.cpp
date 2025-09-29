#include "Enemy.h"
#include "Player.h"
#include "../../Engine/include/Logger.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <random>

namespace Sparky {

    Enemy::Enemy() : GameObject(), targetPlayer(nullptr), health(100.0f), maxHealth(100.0f),
                    speed(2.0f), attackDamage(10.0f), attackRange(2.0f), attackRate(1.0f),
                    lastAttackTime(0.0f), aggressive(true), alive(true), currentState(AIState::PATROL) {
        setName("Enemy");
        SPARKY_LOG_INFO("Enemy created with " + std::to_string(health) + " health");
        
        // Add physics component
        addComponent<PhysicsComponent>();
    }

    Enemy::~Enemy() {
    }

    void Enemy::update(float deltaTime) {
        GameObject::update(deltaTime);
        
        if (!alive) {
            return;
        }
        
        // Simple AI state machine
        switch (currentState) {
            case AIState::PATROL:
                patrol();
                break;
            case AIState::CHASE:
                chasePlayer();
                break;
            case AIState::ATTACK:
                attackPlayer();
                break;
            case AIState::DEAD:
                // Do nothing
                break;
        }
    }

    void Enemy::render() {
        GameObject::render();
    }

    void Enemy::setPlayerTarget(Player* player) {
        targetPlayer = player;
    }

    void Enemy::patrol() {
        // Simple patrol behavior - move randomly or along a path
        // For now, we'll just check if we should chase the player
        if (targetPlayer && aggressive) {
            float distance = distanceToPlayer();
            if (distance < 10.0f) { // Detection range
                changeState(AIState::CHASE);
            }
        }
        
        // In a full implementation, we would move along patrol points
        // For now, we'll just log the patrol state
        // SPARKY_LOG_DEBUG("Enemy patrolling");
    }

    void Enemy::chasePlayer() {
        if (!targetPlayer) {
            changeState(AIState::PATROL);
            return;
        }
        
        float distance = distanceToPlayer();
        
        // Check if player is in attack range
        if (distance <= attackRange) {
            changeState(AIState::ATTACK);
            return;
        }
        
        // Check if player is out of detection range
        if (distance > 20.0f) {
            changeState(AIState::PATROL);
            return;
        }
        
        // Move towards player
        glm::vec3 playerPos = targetPlayer->getPosition();
        glm::vec3 enemyPos = getPosition();
        glm::vec3 direction = glm::normalize(playerPos - enemyPos);
        
        // Apply movement
        PhysicsComponent* physics = getComponent<PhysicsComponent>();
        if (physics) {
            glm::vec3 velocity = direction * speed;
            velocity.y = physics->getVelocity().y; // Preserve vertical velocity
            physics->setVelocity(velocity);
        }
        
        // SPARKY_LOG_DEBUG("Enemy chasing player, distance: " + std::to_string(distance));
    }

    void Enemy::attackPlayer() {
        if (!targetPlayer) {
            changeState(AIState::PATROL);
            return;
        }
        
        float distance = distanceToPlayer();
        
        // Check if player is out of attack range
        if (distance > attackRange + 1.0f) {
            changeState(AIState::CHASE);
            return;
        }
        
        // Check attack cooldown
        float currentTime = static_cast<float>(glfwGetTime());
        if (currentTime - lastAttackTime >= 1.0f / attackRate) {
            // Perform attack
            SPARKY_LOG_INFO("Enemy attacking player for " + std::to_string(attackDamage) + " damage");
            lastAttackTime = currentTime;
            
            // In a full implementation, we would call player->takeDamage(attackDamage)
        }
    }

    void Enemy::takeDamage(float damage) {
        health -= damage;
        SPARKY_LOG_INFO("Enemy took " + std::to_string(damage) + " damage. Health: " + std::to_string(health));
        
        if (health <= 0) {
            health = 0;
            alive = false;
            changeState(AIState::DEAD);
            SPARKY_LOG_INFO("Enemy died");
        } else if (targetPlayer && aggressive) {
            // Become aggressive when damaged
            changeState(AIState::CHASE);
        }
    }

    bool Enemy::isAlive() const {
        return alive;
    }

    float Enemy::distanceToPlayer() const {
        if (!targetPlayer) {
            return std::numeric_limits<float>::max();
        }
        
        glm::vec3 playerPos = targetPlayer->getPosition();
        glm::vec3 enemyPos = getPosition();
        return glm::distance(playerPos, enemyPos);
    }

    void Enemy::changeState(AIState newState) {
        if (currentState != newState) {
            currentState = newState;
            SPARKY_LOG_DEBUG("Enemy changed state to " + std::to_string(static_cast<int>(newState)));
        }
    }
}