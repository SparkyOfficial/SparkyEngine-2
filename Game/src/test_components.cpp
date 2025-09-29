#include "../../Engine/include/HealthComponent.h"
#include "../../Engine/include/AIComponent.h"
#include "../../Engine/include/GameObject.h"
#include "../../Engine/include/Logger.h"
#include <iostream>
#include <memory>

int main() {
    Sparky::Logger::getInstance().setLogLevel(Sparky::LogLevel::DEBUG);
    
    std::cout << "Testing new components...\n";
    
    // Test HealthComponent
    {
        std::cout << "\n--- Testing HealthComponent ---\n";
        auto healthComponent = std::make_unique<Sparky::HealthComponent>(100.0f);
        
        std::cout << "Initial health: " << healthComponent->getHealth() << "/" << healthComponent->getMaxHealth() << "\n";
        
        healthComponent->takeDamage(25.0f);
        std::cout << "After taking 25 damage: " << healthComponent->getHealth() << "\n";
        
        healthComponent->heal(10.0f);
        std::cout << "After healing 10: " << healthComponent->getHealth() << "\n";
        
        healthComponent->setRegenerationRate(5.0f); // 5 health per second
        std::cout << "Set regeneration rate to 5.0\n";
        
        // Test callbacks
        healthComponent->setOnDamageCallback([](float damage) {
            std::cout << "Callback: Took " << damage << " damage!\n";
        });
        
        healthComponent->setOnHealCallback([](float amount) {
            std::cout << "Callback: Healed " << amount << " health!\n";
        });
        
        healthComponent->setOnDeathCallback([]() {
            std::cout << "Callback: Entity died!\n";
        });
        
        healthComponent->takeDamage(85.0f); // Should trigger death callback
        std::cout << "Final health: " << healthComponent->getHealth() << "\n";
        std::cout << "Is alive: " << (healthComponent->isAlive() ? "Yes" : "No") << "\n";
    }
    
    // Test AIComponent
    {
        std::cout << "\n--- Testing AIComponent ---\n";
        auto aiComponent = std::make_unique<Sparky::AIComponent>();
        
        std::cout << "Initial AI state: " << static_cast<int>(aiComponent->getState()) << "\n";
        
        aiComponent->setMoveSpeed(3.5f);
        aiComponent->setDetectionRange(15.0f);
        aiComponent->setAttackRange(2.5f);
        aiComponent->setAttackDamage(15.0f);
        aiComponent->setAttackRate(2.0f); // 2 attacks per second
        
        std::cout << "Move speed: " << aiComponent->getMoveSpeed() << "\n";
        std::cout << "Detection range: " << aiComponent->getDetectionRange() << "\n";
        std::cout << "Attack range: " << aiComponent->getAttackRange() << "\n";
        std::cout << "Attack damage: " << aiComponent->getAttackDamage() << "\n";
        std::cout << "Attack rate: " << aiComponent->getAttackRate() << "\n";
        
        // Test state changes
        aiComponent->setState(Sparky::AIState::PATROL);
        std::cout << "Changed state to PATROL: " << static_cast<int>(aiComponent->getState()) << "\n";
        
        aiComponent->setState(Sparky::AIState::CHASE);
        std::cout << "Changed state to CHASE: " << static_cast<int>(aiComponent->getState()) << "\n";
    }
    
    // Test with GameObject
    {
        std::cout << "\n--- Testing Components with GameObject ---\n";
        auto gameObject = std::make_unique<Sparky::GameObject>("TestObject");
        
        // Add components
        auto healthComp = gameObject->addComponent<Sparky::HealthComponent>(50.0f);
        auto aiComp = gameObject->addComponent<Sparky::AIComponent>();
        
        std::cout << "GameObject created with " << gameObject->getComponents().size() << " components\n";
        
        // Test component access
        auto retrievedHealth = gameObject->getComponent<Sparky::HealthComponent>();
        if (retrievedHealth) {
            std::cout << "Retrieved HealthComponent with " << retrievedHealth->getHealth() << " health\n";
        }
        
        auto retrievedAI = gameObject->getComponent<Sparky::AIComponent>();
        if (retrievedAI) {
            std::cout << "Retrieved AIComponent with state " << static_cast<int>(retrievedAI->getState()) << "\n";
        }
    }
    
    std::cout << "\nAll tests completed successfully!\n";
    return 0;
}