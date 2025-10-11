#include "../include/DamageFeedbackComponent.h"
#include "../include/ParticleComponent.h"
#include "../include/ParticleSystem.h"
#include "../include/GameObject.h"
#include "../include/Logger.h"
#include <memory>
#include <iostream>

int main() {
    Sparky::Logger::getInstance().setLogLevel(Sparky::LogLevel::DEBUG);
    
    std::cout << "Testing DamageFeedbackComponent...\n";
    
    // Create a game object
    auto gameObject = std::make_unique<Sparky::GameObject>("TestObject");
    
    // Add particle component
    auto particleComponent = gameObject->addComponent<Sparky::ParticleComponent>();
    auto particleSystem = std::make_unique<Sparky::ParticleSystem>();
    particleComponent->setParticleSystem(std::move(particleSystem));
    
    // Add damage feedback component
    auto damageFeedback = gameObject->addComponent<Sparky::DamageFeedbackComponent>();
    damageFeedback->setParticleComponent(particleComponent);
    
    std::cout << "Components created successfully\n";
    
    // Test damage feedback
    std::cout << "\n--- Testing damage feedback ---\n";
    damageFeedback->onDamageTaken(25.0f, 0.0f, 0.0f, 1.0f);
    
    // Update the component
    damageFeedback->update(0.1f);
    
    // Test death feedback
    std::cout << "\n--- Testing death feedback ---\n";
    damageFeedback->onDeath();
    
    // Update the component
    damageFeedback->update(0.1f);
    
    // Test healing feedback
    std::cout << "\n--- Testing healing feedback ---\n";
    damageFeedback->onHeal(15.0f);
    
    // Test configuration
    std::cout << "\n--- Testing configuration ---\n";
    damageFeedback->setBloodEffectIntensity(1.5f);
    damageFeedback->setDamageFlashIntensity(0.8f);
    damageFeedback->setScreenShakeIntensity(1.2f);
    
    std::cout << "Blood effect intensity: " << damageFeedback->getBloodEffectIntensity() << "\n";
    std::cout << "Damage flash intensity: " << damageFeedback->getDamageFlashIntensity() << "\n";
    std::cout << "Screen shake intensity: " << damageFeedback->getScreenShakeIntensity() << "\n";
    
    // Test color customization
    damageFeedback->setBloodColor(1.0f, 0.0f, 0.0f, 1.0f); // Red
    damageFeedback->setDamageFlashColor(1.0f, 0.5f, 0.0f, 0.7f); // Orange
    
    float bloodColor[4];
    float flashColor[4];
    damageFeedback->getBloodColor(bloodColor);
    damageFeedback->getDamageFlashColor(flashColor);
    
    std::cout << "Blood color: (" << bloodColor[0] << ", " << bloodColor[1] << ", " << bloodColor[2] << ", " << bloodColor[3] << ")\n";
    std::cout << "Flash color: (" << flashColor[0] << ", " << flashColor[1] << ", " << flashColor[2] << ", " << flashColor[3] << ")\n";
    
    // Test effect enabling/disabling
    damageFeedback->enableEffect(Sparky::DamageEffectType::BLOOD_SPLATTER, false);
    damageFeedback->enableEffect(Sparky::DamageEffectType::SCREEN_SHAKE, false);
    
    std::cout << "Blood effect enabled: " << damageFeedback->isEffectEnabled(Sparky::DamageEffectType::BLOOD_SPLATTER) << "\n";
    std::cout << "Screen shake enabled: " << damageFeedback->isEffectEnabled(Sparky::DamageEffectType::SCREEN_SHAKE) << "\n";
    
    std::cout << "\nDamage feedback test completed successfully!\n";
    return 0;
}