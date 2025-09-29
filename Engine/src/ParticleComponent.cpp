#include "../include/ParticleComponent.h"
#include "../include/GameObject.h"
#include "../include/Logger.h"

namespace Sparky {
    
    ParticleComponent::ParticleComponent() : Component(), followOwner(true) {
        // Create a default particle system
        particleSystem = std::make_unique<ParticleSystem>();
        SPARKY_LOG_DEBUG("ParticleComponent created");
    }
    
    ParticleComponent::~ParticleComponent() {
        SPARKY_LOG_DEBUG("ParticleComponent destroyed");
    }
    
    void ParticleComponent::update(float deltaTime) {
        if (!particleSystem) return;
        
        // Update the emission point to follow the owner if needed
        if (followOwner && owner) {
            particleSystem->setEmissionPoint(owner->getPosition());
        }
        
        // Update the particle system
        particleSystem->update(deltaTime);
    }
    
    void ParticleComponent::render() {
        if (!particleSystem) return;
        
        // Render the particle system
        particleSystem->render();
    }
    
    void ParticleComponent::setParticleSystem(std::unique_ptr<ParticleSystem> system) {
        particleSystem = std::move(system);
    }
    
    void ParticleComponent::emitMuzzleFlash(const glm::vec3& direction) {
        if (!particleSystem || !owner) return;
        
        // Set the emission point to the owner's position
        particleSystem->setEmissionPoint(owner->getPosition());
        
        // Create the muzzle flash effect
        particleSystem->createMuzzleFlash(owner->getPosition(), direction);
    }
    
    void ParticleComponent::emitExplosion() {
        if (!particleSystem || !owner) return;
        
        // Set the emission point to the owner's position
        particleSystem->setEmissionPoint(owner->getPosition());
        
        // Create the explosion effect
        particleSystem->createExplosion(owner->getPosition());
    }
    
    void ParticleComponent::emitSmoke() {
        if (!particleSystem || !owner) return;
        
        // Set the emission point to the owner's position
        particleSystem->setEmissionPoint(owner->getPosition());
        
        // Create the smoke effect
        particleSystem->createSmoke(owner->getPosition());
    }
}