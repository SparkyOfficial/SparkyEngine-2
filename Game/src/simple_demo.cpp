#include "../../Engine/include/Logger.h"
#include "../../Engine/include/HealthComponent.h"
#include "../../Engine/include/GameObject.h"
#include "../../Engine/include/Inventory.h"
#include <iostream>
#include <memory>

int main() {
    // Set up logger
    Sparky::Logger::getInstance().setLogLevel(Sparky::LogLevel::DEBUG);
    
    std::cout << "========================================\n";
    std::cout << "      Sparky Engine 2 - Simple Demo     \n";
    std::cout << "========================================\n\n";
    
    // Demonstrate logging system
    std::cout << "1. Testing Logging System:\n";
    SPARKY_LOG_INFO("This is an info message");
    SPARKY_LOG_DEBUG("This is a debug message");
    SPARKY_LOG_WARNING("This is a warning message");
    SPARKY_LOG_ERROR("This is an error message");
    std::cout << "   Logging system works correctly!\n\n";
    
    // Demonstrate GameObject and Component system
    std::cout << "2. Testing GameObject and Component System:\n";
    auto player = std::make_unique<Sparky::GameObject>("Player");
    auto healthComponent = player->addComponent<Sparky::HealthComponent>(100.0f);
    
    std::cout << "   Created player with " << healthComponent->getHealth() << " health\n";
    healthComponent->takeDamage(25.0f);
    std::cout << "   After taking 25 damage: " << healthComponent->getHealth() << " health\n";
    healthComponent->heal(10.0f);
    std::cout << "   After healing 10: " << healthComponent->getHealth() << " health\n";
    std::cout << "   Player is " << (healthComponent->isAlive() ? "alive" : "dead") << "\n\n";
    
    // Demonstrate inventory system
    std::cout << "3. Testing Inventory System:\n";
    auto inventory = std::make_unique<Sparky::Inventory>(10); // 10 slots
    
    auto weapon = std::make_unique<Sparky::Item>("Pistol", 1);
    weapon->setQuantity(1);
    
    auto ammo = std::make_unique<Sparky::Item>("9mm Ammo", 30);
    ammo->setQuantity(30);
    
    if (inventory->addItem(std::move(weapon))) {
        std::cout << "   Added weapon to inventory\n";
    }
    
    if (inventory->addItem(std::move(ammo))) {
        std::cout << "   Added ammo to inventory\n";
    }
    
    std::cout << "   Inventory has " << inventory->getItemCount() << " items\n\n";
    
    std::cout << "========================================\n";
    std::cout << "         Demo completed successfully!   \n";
    std::cout << "========================================\n";
    
    return 0;
}