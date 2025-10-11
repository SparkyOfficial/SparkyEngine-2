#include "../include/Enemy.h"
#include "../include/HealthComponent.h"
#include "../include/AIComponent.h"
#include "../include/Player.h"
#include "../include/Logger.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Sparky {

    Enemy::Enemy(const std::string& name, EnemyType type) : 
        GameObject(name), type(type), healthComponent(nullptr), aiComponent(nullptr),
        moveSpeed(2.0f), attackDamage(10.0f), attackRange(1.5f), 
        attackRate(1.0f), detectionRange(10.0f), lastAttackTime(0.0f) {
        
        // Add health component
        healthComponent = addComponent<HealthComponent>();
        
        // Add AI component
        aiComponent = addComponent<AIComponent>();
        
        // Set default health based on enemy type
        if (healthComponent) {
            switch (type) {
                case EnemyType::BASIC:
                    healthComponent->setMaxHealth(50.0f);
                    healthComponent->setHealth(50.0f);
                    break;
                case EnemyType::FAST:
                    healthComponent->setMaxHealth(30.0f);
                    healthComponent->setHealth(30.0f);
                    moveSpeed = 4.0f;
                    break;
                case EnemyType::TANK:
                    healthComponent->setMaxHealth(150.0f);
                    healthComponent->setHealth(150.0f);
                    moveSpeed = 1.0f;
                    attackDamage = 25.0f;
                    break;
                case EnemyType::RANGED:
                    healthComponent->setMaxHealth(40.0f);
                    healthComponent->setHealth(40.0f);
                    attackRange = 8.0f;
                    break;
                case EnemyType::BOSS:
                    healthComponent->setMaxHealth(500.0f);
                    healthComponent->setHealth(500.0f);
                    attackDamage = 50.0f;
                    attackRange = 3.0f;
                    moveSpeed = 1.5f;
                    break;
            }
            
            healthComponent->setOnDamageCallback([](float damage) {
                Logger::getInstance().info("Enemy took " + std::to_string(damage) + " damage");
            });
            
            healthComponent->setOnDeathCallback([]() {
                Logger::getInstance().info("Enemy died");
            });
        }
    }

    Enemy::~Enemy() {
    }

    void Enemy::update(float deltaTime) {
        GameObject::update(deltaTime);
        // Additional enemy-specific update logic can be added here
    }

    void Enemy::render() {
        GameObject::render();
        // Additional enemy-specific rendering logic can be added here
    }
    
    void Enemy::takeDamage(float damage) {
        if (healthComponent) {
            healthComponent->takeDamage(damage);
        }
    }
    
    void Enemy::attack(Player* player) {
        if (!player || !healthComponent || !healthComponent->isAlive()) {
            return;
        }
        
        // In a full implementation, we would apply damage to the player
        Logger::getInstance().info("Enemy attacked player for " + std::to_string(attackDamage) + " damage");
    }
    
    bool Enemy::isAlive() const {
        return healthComponent && healthComponent->isAlive();
    }
    
    float Enemy::distanceToPlayer(Player* player) const {
        if (!player) {
            return 0.0f;
        }
        
        // In a full implementation, we would calculate the actual distance
        // For now, we'll return a default value
        return 5.0f;
    }
}