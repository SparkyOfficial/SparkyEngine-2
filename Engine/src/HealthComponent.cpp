#include "../include/HealthComponent.h"
#include "../include/Logger.h"
#include "../include/DamageFeedbackComponent.h"

#ifdef HAS_GLFW
#include <GLFW/glfw3.h>
#endif

namespace Sparky {

    HealthComponent::HealthComponent(float maxHealth) 
        : Component(), currentHealth(maxHealth), maxHealth(maxHealth), 
          regenerationRate(0.0f), lastRegenerationTime(0.0f),
          damageFeedback(nullptr) {
        SPARKY_LOG_DEBUG("HealthComponent created with " + std::to_string(maxHealth) + " max health");
    }

    HealthComponent::~HealthComponent() {
    }

    void HealthComponent::update(float deltaTime) {
        // Handle health regeneration
        if (regenerationRate > 0.0f && currentHealth < maxHealth && currentHealth > 0) {
#ifdef HAS_GLFW
            float currentTime = static_cast<float>(glfwGetTime());
#else
            // Fallback implementation
            static float fallbackTime = 0.0f;
            fallbackTime += deltaTime;
            float currentTime = fallbackTime;
#endif
            if (lastRegenerationTime == 0.0f) {
                lastRegenerationTime = currentTime;
            }
            
            float timePassed = currentTime - lastRegenerationTime;
            float healthToRegenerate = regenerationRate * timePassed;
            
            currentHealth += healthToRegenerate;
            if (currentHealth > maxHealth) {
                currentHealth = maxHealth;
            }
            
            lastRegenerationTime = currentTime;
            
            if (onHealCallback) {
                onHealCallback(healthToRegenerate);
            }
            
            // Trigger healing feedback
            if (damageFeedback) {
                damageFeedback->onHeal(healthToRegenerate);
            }
        }
    }

    void HealthComponent::render() {
        // Health components don't render anything directly
    }

    void HealthComponent::takeDamage(float damage) {
        if (damage <= 0 || currentHealth <= 0) {
            return;
        }
        
        currentHealth -= damage;
        if (currentHealth < 0) {
            currentHealth = 0;
        }
        
        SPARKY_LOG_DEBUG("HealthComponent took " + std::to_string(damage) + " damage. Health: " + std::to_string(currentHealth));
        
        // Trigger damage feedback
        if (damageFeedback) {
            // For now, we'll use a default direction (from front)
            damageFeedback->onDamageTaken(damage, 0.0f, 0.0f, 1.0f);
        }
        
        if (onDamageCallback) {
            onDamageCallback(damage);
        }
        
        // Check for death
        if (currentHealth <= 0 && onDeathCallback) {
            onDeathCallback();
            
            // Trigger death feedback
            if (damageFeedback) {
                damageFeedback->onDeath();
            }
        }
    }

    void HealthComponent::heal(float amount) {
        if (amount <= 0 || currentHealth <= 0) {
            return;
        }
        
        currentHealth += amount;
        if (currentHealth > maxHealth) {
            currentHealth = maxHealth;
        }
        
        SPARKY_LOG_DEBUG("HealthComponent healed " + std::to_string(amount) + " health. Health: " + std::to_string(currentHealth));
        
        // Trigger healing feedback
        if (damageFeedback) {
            damageFeedback->onHeal(amount);
        }
        
        if (onHealCallback) {
            onHealCallback(amount);
        }
    }

    void HealthComponent::setHealth(float health) {
        float oldHealth = currentHealth;
        currentHealth = health;
        
        if (currentHealth > maxHealth) {
            currentHealth = maxHealth;
        }
        
        if (currentHealth < 0) {
            currentHealth = 0;
        }
        
        // Trigger callbacks if health changed
        if (currentHealth != oldHealth) {
            float difference = currentHealth - oldHealth;
            if (difference > 0 && onHealCallback) {
                onHealCallback(difference);
                
                // Trigger healing feedback
                if (damageFeedback) {
                    damageFeedback->onHeal(difference);
                }
            } else if (difference < 0 && onDamageCallback) {
                onDamageCallback(-difference);
                
                // Trigger damage feedback
                if (damageFeedback) {
                    damageFeedback->onDamageTaken(-difference, 0.0f, 0.0f, 1.0f);
                }
            }
        }
        
        // Check for death
        if (currentHealth <= 0 && onDeathCallback) {
            onDeathCallback();
            
            // Trigger death feedback
            if (damageFeedback) {
                damageFeedback->onDeath();
            }
        }
    }

    void HealthComponent::setMaxHealth(float maxHealth) {
        this->maxHealth = maxHealth;
        if (currentHealth > this->maxHealth) {
            currentHealth = this->maxHealth;
        }
    }

    void HealthComponent::setRegenerationRate(float rate) {
        this->regenerationRate = rate;
    }
}