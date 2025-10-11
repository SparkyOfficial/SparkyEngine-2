#pragma once

#include "Component.h"
#include "ParticleSystem.h"
#include <memory>

namespace Sparky {
    
    class ParticleComponent : public Component {
    public:
        ParticleComponent();
        virtual ~ParticleComponent();
        
        void update(float deltaTime) override;
        void render() override;
        
        // Particle system management
        ParticleSystem* getParticleSystem() { return particleSystem.get(); }
        const ParticleSystem* getParticleSystem() const { return particleSystem.get(); }
        
        void setParticleSystem(std::unique_ptr<ParticleSystem> system);
        
        // Convenience methods for common effects
        void emitMuzzleFlash(float dirX, float dirY, float dirZ);
        void emitExplosion();
        void emitSmoke();
        void emitFire();
        void emitSparks(int count = 10);
        void emitBlood(float dirX, float dirY, float dirZ, int count = 5);
        void emitMist();
        void emitTrail(float endX, float endY, float endZ, float duration = 1.0f);
        void emitShockwave(float radius = 5.0f, float intensity = 1.0f);
        
        // Combined effects
        void emitExplosionWithSmoke(float intensity = 1.0f);
        void emitFireWithSmoke(float intensity = 1.0f);
        
        // Configuration methods
        void setFollowOwner(bool follow) { followOwner = follow; }
        bool getFollowOwner() const { return followOwner; }
        
    private:
        std::unique_ptr<ParticleSystem> particleSystem;
        bool followOwner; // Whether to follow the owner's position
    };
}