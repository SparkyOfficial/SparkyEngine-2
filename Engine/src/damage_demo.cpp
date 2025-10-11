#include "../include/HealthComponent.h"
#include "../include/DamageFeedbackComponent.h"
#include "../include/ParticleComponent.h"
#include "../include/ParticleSystem.h"
#include "../include/GameObject.h"
#include "../include/Enemy.h"
#include "../include/Logger.h"
#include <memory>
#include <iostream>
#include <vector>

// Simple player class for demonstration
class Player : public Sparky::GameObject {
public:
    Player() : GameObject("Player") {
        // Add components
        healthComponent = addComponent<Sparky::HealthComponent>(100.0f);
        
        particleComponent = addComponent<Sparky::ParticleComponent>();
        auto particleSystem = std::make_unique<Sparky::ParticleSystem>();
        particleComponent->setParticleSystem(std::move(particleSystem));
        
        damageFeedback = addComponent<Sparky::DamageFeedbackComponent>();
        damageFeedback->setParticleComponent(particleComponent);
        
        // Connect health component with damage feedback
        healthComponent->setDamageFeedbackComponent(damageFeedback);
        
        // Set up callbacks
        healthComponent->setOnDamageCallback([this](float damage) {
            Sparky::Logger::getInstance().info("Player took " + std::to_string(damage) + " damage!");
        });
        
        healthComponent->setOnHealCallback([this](float amount) {
            Sparky::Logger::getInstance().info("Player healed " + std::to_string(amount) + " health!");
        });
        
        healthComponent->setOnDeathCallback([this]() {
            Sparky::Logger::getInstance().info("Player died!");
        });
    }
    
    Sparky::HealthComponent* getHealthComponent() { return healthComponent; }
    
private:
    Sparky::HealthComponent* healthComponent;
    Sparky::ParticleComponent* particleComponent;
    Sparky::DamageFeedbackComponent* damageFeedback;
};

int main() {
    Sparky::Logger::getInstance().setLogLevel(Sparky::LogLevel::DEBUG);
    
    std::cout << "Damage Feedback Demo\n";
    std::cout << "===================\n\n";
    
    // Create player
    auto player = std::make_unique<Player>();
    std::cout << "Player created with " << player->getHealthComponent()->getHealth() << " health\n";
    
    // Create enemies
    std::vector<std::unique_ptr<Sparky::Enemy>> enemies;
    
    // Basic enemy
    auto basicEnemy = std::make_unique<Sparky::Enemy>("BasicEnemy", Sparky::EnemyType::BASIC);
    enemies.push_back(std::move(basicEnemy));
    
    // Tank enemy
    auto tankEnemy = std::make_unique<Sparky::TankEnemy>("TankEnemy");
    enemies.push_back(std::move(tankEnemy));
    
    // Fast enemy
    auto fastEnemy = std::make_unique<Sparky::FastEnemy>("FastEnemy");
    enemies.push_back(std::move(fastEnemy));
    
    std::cout << "Created " << enemies.size() << " enemies\n\n";
    
    // Simulate combat
    std::cout << "--- Combat Simulation ---\n";
    
    // Player takes damage from enemies
    std::cout << "Player is attacked by enemies...\n";
    player->getHealthComponent()->takeDamage(15.0f); // Basic attack
    player->getHealthComponent()->takeDamage(25.0f); // Tank attack
    player->getHealthComponent()->takeDamage(10.0f); // Fast attack
    
    std::cout << "Player health: " << player->getHealthComponent()->getHealth() << "\n\n";
    
    // Player heals
    std::cout << "Player finds a health pack...\n";
    player->getHealthComponent()->heal(20.0f);
    std::cout << "Player health: " << player->getHealthComponent()->getHealth() << "\n\n";
    
    // Player takes fatal damage
    std::cout << "Player is hit by a powerful attack...\n";
    player->getHealthComponent()->takeDamage(85.0f);
    std::cout << "Player health: " << player->getHealthComponent()->getHealth() << "\n";
    std::cout << "Player alive: " << (player->getHealthComponent()->isAlive() ? "Yes" : "No") << "\n\n";
    
    // Player resurrected
    std::cout << "Player is resurrected...\n";
    player->getHealthComponent()->setHealth(50.0f);
    std::cout << "Player health: " << player->getHealthComponent()->getHealth() << "\n";
    std::cout << "Player alive: " << (player->getHealthComponent()->isAlive() ? "Yes" : "No") << "\n\n";
    
    std::cout << "Damage feedback demo completed successfully!\n";
    std::cout << "Visual effects would be shown in a full implementation with rendering.\n";
    
    return 0;
}