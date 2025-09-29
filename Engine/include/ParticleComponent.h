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
        void emitMuzzleFlash(const glm::vec3& direction);
        void emitExplosion();
        void emitSmoke();
        
        // Configuration methods
        void setFollowOwner(bool follow) { followOwner = follow; }
        bool getFollowOwner() const { return followOwner; }
        
    private:
        std::unique_ptr<ParticleSystem> particleSystem;
        bool followOwner; // Whether to follow the owner's position
    };
}