#pragma once

#include "Component.h"
#include "ParticleSystem.h"
#include "ParticleComponent.h"
#include <memory>

namespace Sparky {
    
    // Types of damage feedback effects
    enum class DamageEffectType {
        BLOOD_SPLATTER,
        DAMAGE_FLASH,
        SCREEN_SHAKE,
        SOUND_EFFECT
    };
    
    class DamageFeedbackComponent : public Component {
    public:
        DamageFeedbackComponent();
        virtual ~DamageFeedbackComponent();
        
        void update(float deltaTime) override;
        void render() override;
        
        // Damage feedback methods
        void onDamageTaken(float damage, float directionX = 0.0f, float directionY = 0.0f, float directionZ = 1.0f);
        void onDeath();
        void onHeal(float amount);
        
        // Configuration methods
        void setBloodEffectIntensity(float intensity) { bloodEffectIntensity = intensity; }
        float getBloodEffectIntensity() const { return bloodEffectIntensity; }
        
        void setDamageFlashIntensity(float intensity) { damageFlashIntensity = intensity; }
        float getDamageFlashIntensity() const { return damageFlashIntensity; }
        
        void setScreenShakeIntensity(float intensity) { screenShakeIntensity = intensity; }
        float getScreenShakeIntensity() const { return screenShakeIntensity; }
        
        void setParticleComponent(ParticleComponent* particles) { particleComponent = particles; }
        ParticleComponent* getParticleComponent() const { return particleComponent; }
        
        // Effect customization
        void setBloodColor(float r, float g, float b, float a = 1.0f);
        void getBloodColor(float* color) const;
        
        void setDamageFlashColor(float r, float g, float b, float a = 0.5f);
        void getDamageFlashColor(float* color) const;
        
        // Effect enabling/disabling
        void enableEffect(DamageEffectType effect, bool enable = true);
        bool isEffectEnabled(DamageEffectType effect) const;

    private:
        // Effect intensities
        float bloodEffectIntensity;
        float damageFlashIntensity;
        float screenShakeIntensity;
        
        // Effect colors
        float bloodColor[4];
        float damageFlashColor[4];
        
        // Effect enabled flags
        bool bloodEffectEnabled;
        bool damageFlashEnabled;
        bool screenShakeEnabled;
        bool soundEffectEnabled;
        
        // References to other components
        ParticleComponent* particleComponent;
        
        // Internal state
        float damageFlashTimer;
        float damageFlashDuration;
        float screenShakeTimer;
        float screenShakeDuration;
    };
}