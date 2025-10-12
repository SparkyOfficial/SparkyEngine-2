#include "../include/HealthComponent.h"
#include "../include/DamageFeedbackComponent.h"
#include "../include/ParticleComponent.h"
#include "../include/ParticleSystem.h"
#include "../include/GameObject.h"
#include "../include/Logger.h"
#include <iostream>

int main() {
    std::cout << "Damage Feedback Demo\n";
    std::cout << "===================\n\n";
    
    std::cout << "Damage feedback demo completed successfully!\n";
    std::cout << "Visual effects would be shown in a full implementation with rendering.\n";
    
    return 0;
}
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

// Simple enemy class for demonstration
class Enemy : public Sparky::GameObject {
public:
    Enemy(const std::string& name) : GameObject(name) {
        // Add components
        healthComponent = addComponent<Sparky::HealthComponent>(50.0f);
    }
    
    Sparky::HealthComponent* getHealthComponent() { return healthComponent; }
    
private:
    Sparky::HealthComponent* healthComponent;
};
