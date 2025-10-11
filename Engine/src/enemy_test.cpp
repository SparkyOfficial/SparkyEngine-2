#include "../include/Enemy.h"
#include "../include/FastEnemy.h"
#include "../include/TankEnemy.h"
#include "../include/RangedEnemy.h"
#include "../include/BossEnemy.h"
#include "../include/Logger.h"

int main() {
    Sparky::Logger::getInstance().setLogLevel(Sparky::LogLevel::DEBUG);
    
    // Test basic enemy
    Sparky::Logger::getInstance().info("Creating basic enemy...");
    auto basicEnemy = std::make_unique<Sparky::Enemy>("BasicEnemy", Sparky::EnemyType::BASIC);
    Sparky::Logger::getInstance().info("Basic enemy created with " + 
        std::to_string(basicEnemy->getHealthComponent()->getMaxHealth()) + " health");
    
    // Test fast enemy
    Sparky::Logger::getInstance().info("Creating fast enemy...");
    auto fastEnemy = std::make_unique<Sparky::FastEnemy>("FastEnemy");
    Sparky::Logger::getInstance().info("Fast enemy created with " + 
        std::to_string(fastEnemy->getHealthComponent()->getMaxHealth()) + " health and " +
        std::to_string(fastEnemy->getMoveSpeed()) + " move speed");
    
    // Test tank enemy
    Sparky::Logger::getInstance().info("Creating tank enemy...");
    auto tankEnemy = std::make_unique<Sparky::TankEnemy>("TankEnemy");
    Sparky::Logger::getInstance().info("Tank enemy created with " + 
        std::to_string(tankEnemy->getHealthComponent()->getMaxHealth()) + " health and " +
        std::to_string(tankEnemy->getArmor()) + " armor");
    
    // Test ranged enemy
    Sparky::Logger::getInstance().info("Creating ranged enemy...");
    auto rangedEnemy = std::make_unique<Sparky::RangedEnemy>("RangedEnemy");
    Sparky::Logger::getInstance().info("Ranged enemy created with " + 
        std::to_string(rangedEnemy->getHealthComponent()->getMaxHealth()) + " health and " +
        std::to_string(rangedEnemy->getAttackRange()) + " attack range");
    
    // Test boss enemy
    Sparky::Logger::getInstance().info("Creating boss enemy...");
    auto bossEnemy = std::make_unique<Sparky::BossEnemy>("BossEnemy");
    Sparky::Logger::getInstance().info("Boss enemy created with " + 
        std::to_string(bossEnemy->getHealthComponent()->getMaxHealth()) + " health");
    
    // Test taking damage
    Sparky::Logger::getInstance().info("Testing damage...");
    basicEnemy->takeDamage(25.0f);
    Sparky::Logger::getInstance().info("Basic enemy health: " + 
        std::to_string(basicEnemy->getHealthComponent()->getHealth()));
    
    // Test enemy death
    basicEnemy->takeDamage(50.0f);
    Sparky::Logger::getInstance().info("Basic enemy alive: " + 
        std::string(basicEnemy->isAlive() ? "true" : "false"));
    
    Sparky::Logger::getInstance().info("Enemy diversity test completed successfully!");
    return 0;
}