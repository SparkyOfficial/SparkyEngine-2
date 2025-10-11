#include "../include/DamageFeedbackComponent.h"
#include "../include/Logger.h"
#include "../include/ParticleComponent.h"
#include "../include/ParticleSystem.h"
#include <algorithm>

#ifdef HAS_GLFW
#include <GLFW/glfw3.h>
#endif

namespace Sparky {

    DamageFeedbackComponent::DamageFeedbackComponent() : 
        Component(),
        bloodEffectIntensity(1.0f),
        damageFlashIntensity(1.0f),
        screenShakeIntensity(1.0f),
        particleComponent(nullptr),
        damageFlashTimer(0.0f),
        damageFlashDuration(0.2f),
        screenShakeTimer(0.0f),
        screenShakeDuration(0.3f),
        bloodEffectEnabled(true),
        damageFlashEnabled(true),
        screenShakeEnabled(true),
        soundEffectEnabled(true) {
        
        // Default blood color (red)
        bloodColor[0] = 0.8f;  // R
        bloodColor[1] = 0.0f;  // G
        bloodColor[2] = 0.0f;  // B
        bloodColor[3] = 1.0f;  // A
        
        // Default damage flash color (red with transparency)
        damageFlashColor[0] = 1.0f;  // R
        damageFlashColor[1] = 0.0f;  // G
        damageFlashColor[2] = 0.0f;  // B
        damageFlashColor[3] = 0.5f;  // A
        
        SPARKY_LOG_DEBUG("DamageFeedbackComponent created");
    }

    DamageFeedbackComponent::~DamageFeedbackComponent() {
        SPARKY_LOG_DEBUG("DamageFeedbackComponent destroyed");
    }

    void DamageFeedbackComponent::update(float deltaTime) {
        // Update damage flash effect timer
        if (damageFlashTimer > 0.0f) {
            damageFlashTimer -= deltaTime;
            if (damageFlashTimer < 0.0f) {
                damageFlashTimer = 0.0f;
            }
        }
        
        // Update screen shake timer
        if (screenShakeTimer > 0.0f) {
            screenShakeTimer -= deltaTime;
            if (screenShakeTimer < 0.0f) {
                screenShakeTimer = 0.0f;
            }
        }
    }

    void DamageFeedbackComponent::render() {
        // Damage feedback effects are typically handled at the rendering system level
        // This component mainly manages the state and triggers effects
    }

    void DamageFeedbackComponent::onDamageTaken(float damage, float directionX, float directionY, float directionZ) {
        SPARKY_LOG_DEBUG("DamageFeedbackComponent: Damage taken - " + std::to_string(damage));
        
        // Blood splatter effect
        if (bloodEffectEnabled && particleComponent) {
            // Emit blood particles in the direction of the damage
            int bloodCount = static_cast<int>(5 * bloodEffectIntensity * (damage / 10.0f));
            particleComponent->emitBlood(directionX, directionY, directionZ, std::max(1, bloodCount));
        }
        
        // Screen flash effect
        if (damageFlashEnabled) {
            // Start the damage flash timer
            damageFlashTimer = damageFlashDuration;
            
            // The actual screen flash would be handled by the rendering system
            // For now, we just log it
            SPARKY_LOG_DEBUG("Damage flash effect triggered");
        }
        
        // Screen shake effect
        if (screenShakeEnabled) {
            // Start the screen shake timer
            screenShakeTimer = screenShakeDuration * screenShakeIntensity * (damage / 20.0f);
            
            // The actual screen shake would be handled by the camera system
            // For now, we just log it
            SPARKY_LOG_DEBUG("Screen shake effect triggered");
        }
        
        // Sound effect
        if (soundEffectEnabled) {
            // Play damage sound
            // In a full implementation, this would interface with the audio system
            SPARKY_LOG_DEBUG("Damage sound effect triggered");
        }
    }

    void DamageFeedbackComponent::onDeath() {
        SPARKY_LOG_DEBUG("DamageFeedbackComponent: Entity died");
        
        // Special death effects
        if (particleComponent) {
            // Create a more intense blood splatter
            particleComponent->emitBlood(0.0f, 1.0f, 0.0f, 20);
            
            // Create explosion effect for more dramatic death
            particleComponent->emitExplosion();
        }
        
        // Intense screen flash
        if (damageFlashEnabled) {
            damageFlashTimer = damageFlashDuration * 2.0f; // Longer flash
        }
        
        // Strong screen shake
        if (screenShakeEnabled) {
            screenShakeTimer = screenShakeDuration * 3.0f; // Longer shake
        }
    }

    void DamageFeedbackComponent::onHeal(float amount) {
        SPARKY_LOG_DEBUG("DamageFeedbackComponent: Healed - " + std::to_string(amount));
        
        // Healing effects
        if (particleComponent) {
            // Create a healing particle effect (green particles floating upward)
            // This would be a custom effect in the particle system
            SPARKY_LOG_DEBUG("Healing particle effect triggered");
        }
    }

    void DamageFeedbackComponent::setBloodColor(float r, float g, float b, float a) {
        bloodColor[0] = r;
        bloodColor[1] = g;
        bloodColor[2] = b;
        bloodColor[3] = a;
    }

    void DamageFeedbackComponent::getBloodColor(float* color) const {
        color[0] = bloodColor[0];
        color[1] = bloodColor[1];
        color[2] = bloodColor[2];
        color[3] = bloodColor[3];
    }

    void DamageFeedbackComponent::setDamageFlashColor(float r, float g, float b, float a) {
        damageFlashColor[0] = r;
        damageFlashColor[1] = g;
        damageFlashColor[2] = b;
        damageFlashColor[3] = a;
    }

    void DamageFeedbackComponent::getDamageFlashColor(float* color) const {
        color[0] = damageFlashColor[0];
        color[1] = damageFlashColor[1];
        color[2] = damageFlashColor[2];
        color[3] = damageFlashColor[3];
    }

    void DamageFeedbackComponent::enableEffect(DamageEffectType effect, bool enable) {
        switch (effect) {
            case DamageEffectType::BLOOD_SPLATTER:
                bloodEffectEnabled = enable;
                break;
            case DamageEffectType::DAMAGE_FLASH:
                damageFlashEnabled = enable;
                break;
            case DamageEffectType::SCREEN_SHAKE:
                screenShakeEnabled = enable;
                break;
            case DamageEffectType::SOUND_EFFECT:
                soundEffectEnabled = enable;
                break;
        }
    }

    bool DamageFeedbackComponent::isEffectEnabled(DamageEffectType effect) const {
        switch (effect) {
            case DamageEffectType::BLOOD_SPLATTER:
                return bloodEffectEnabled;
            case DamageEffectType::DAMAGE_FLASH:
                return damageFlashEnabled;
            case DamageEffectType::SCREEN_SHAKE:
                return screenShakeEnabled;
            case DamageEffectType::SOUND_EFFECT:
                return soundEffectEnabled;
            default:
                return false;
        }
    }
}