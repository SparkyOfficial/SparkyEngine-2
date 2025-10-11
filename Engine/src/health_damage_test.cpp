#include "../include/HealthComponent.h"
#include "../include/DamageFeedbackComponent.h"
#include "../include/ParticleComponent.h"
#include "../include/ParticleSystem.h"
#include "../include/GameObject.h"
#include "../include/Logger.h"
#include <memory>
#include <iostream>

int main() {
    Sparky::Logger::getInstance().setLogLevel(Sparky::LogLevel::DEBUG);
    
    std::cout << "Testing HealthComponent with DamageFeedbackComponent integration...\n";
    
    // Create a game object
    auto gameObject = std::make_unique<Sparky::GameObject>("TestObject");
    
    // Add particle component for visual effects
    auto particleComponent = gameObject->addComponent<Sparky::ParticleComponent>();
    auto particleSystem = std::make_unique<Sparky::ParticleSystem>();
    particleComponent->setParticleSystem(std::move(particleSystem));
    
    // Add damage feedback component
    auto damageFeedback = gameObject->addComponent<Sparky::DamageFeedbackComponent>();
    damageFeedback->setParticleComponent(particleComponent);
    
    // Add health component
    auto healthComponent = gameObject->addComponent<Sparky::HealthComponent>(100.0f);
    
    // Connect health component with damage feedback
    healthComponent->setDamageFeedbackComponent(damageFeedback);
    
    // Set up callbacks for logging
    healthComponent->setOnDamageCallback([](float damage) {
        std::cout << "Callback: Took " << damage << " damage!\n";
    });
    
    healthComponent->setOnHealCallback([](float amount) {
        std::cout << "Callback: Healed " << amount << " health!\n";
    });
    
    healthComponent->setOnDeathCallback([]() {
        std::cout << "Callback: Entity died!\n";
    });
    
    std::cout << "Components created and connected successfully\n";
    std::cout << "Initial health: " << healthComponent->getHealth() << "/" << healthComponent->getMaxHealth() << "\n";
    
    // Test taking damage
    std::cout << "\n--- Testing damage ---\n";
    healthComponent->takeDamage(25.0f);
    std::cout << "Health after 25 damage: " << healthComponent->getHealth() << "\n";
    
    // Test healing
    std::cout << "\n--- Testing healing ---\n";
    healthComponent->heal(10.0f);
    std::cout << "Health after 10 healing: " << healthComponent->getHealth() << "\n";
    
    // Test direct health setting
    std::cout << "\n--- Testing direct health setting ---\n";
    healthComponent->setHealth(80.0f);
    std::cout << "Health after setting to 80: " << healthComponent->getHealth() << "\n";
    
    // Test death
    std::cout << "\n--- Testing death ---\n";
    healthComponent->takeDamage(85.0f); // Should trigger death
    std::cout << "Health after fatal damage: " << healthComponent->getHealth() << "\n";
    std::cout << "Is alive: " << (healthComponent->isAlive() ? "Yes" : "No") << "\n";
    
    // Test resurrection
    std::cout << "\n--- Testing resurrection ---\n";
    healthComponent->setHealth(50.0f);
    std::cout << "Health after resurrection: " << healthComponent->getHealth() << "\n";
    std::cout << "Is alive: " << (healthComponent->isAlive() ? "Yes" : "No") << "\n";
    
    std::cout << "\nHealth and damage feedback integration test completed successfully!\n";
    return 0;
}