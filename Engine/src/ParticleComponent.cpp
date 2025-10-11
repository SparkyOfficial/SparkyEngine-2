#include "../include/ParticleComponent.h"
#include "../include/GameObject.h"

namespace Sparky {
    
    ParticleComponent::ParticleComponent() : Component(), followOwner(true) {
        // Create a default particle system
        particleSystem = std::make_unique<ParticleSystem>();
    }
    
    ParticleComponent::~ParticleComponent() {
    }
    
    void ParticleComponent::update(float deltaTime) {
        if (!particleSystem) return;
        
        // Update the emission point to follow the owner if needed
        if (followOwner && owner) {
            // In a full implementation, we would update the emitter position
            // For now, we'll just update the particle system
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
    
    void ParticleComponent::emitMuzzleFlash(float dirX, float dirY, float dirZ) {
        if (!particleSystem || !owner) return;
        
        // Set the emission point to the owner's position
        // In a full implementation, we would get the actual position
        particleSystem->createExplosion(0.0f, 0.0f, 0.0f, 0.1f); // Small explosion for muzzle flash
    }
    
    void ParticleComponent::emitExplosion() {
        if (!particleSystem || !owner) return;
        
        // Set the emission point to the owner's position
        // In a full implementation, we would get the actual position
        particleSystem->createExplosion(0.0f, 0.0f, 0.0f);
    }
    
    void ParticleComponent::emitSmoke() {
        if (!particleSystem || !owner) return;
        
        // Set the emission point to the owner's position
        // In a full implementation, we would get the actual position
        particleSystem->createSmoke(0.0f, 0.0f, 0.0f);
    }
    
    void ParticleComponent::emitFire() {
        if (!particleSystem || !owner) return;
        
        // Set the emission point to the owner's position
        // In a full implementation, we would get the actual position
        particleSystem->createFire(0.0f, 0.0f, 0.0f);
    }
    
    void ParticleComponent::emitSparks(int count) {
        if (!particleSystem || !owner) return;
        
        // Set the emission point to the owner's position
        // In a full implementation, we would get the actual position
        particleSystem->createSparks(0.0f, 0.0f, 0.0f, count);
    }
    
    void ParticleComponent::emitBlood(float dirX, float dirY, float dirZ, int count) {
        if (!particleSystem || !owner) return;
        
        // Set the emission point to the owner's position
        // In a full implementation, we would get the actual position
        particleSystem->createBlood(0.0f, 0.0f, 0.0f, dirX, dirY, dirZ, count);
    }
    
    void ParticleComponent::emitMist() {
        if (!particleSystem || !owner) return;
        
        // Set the emission point to the owner's position
        // In a full implementation, we would get the actual position
        particleSystem->createMist(0.0f, 0.0f, 0.0f);
    }
    
    void ParticleComponent::emitTrail(float endX, float endY, float endZ, float duration) {
        if (!particleSystem || !owner) return;
        
        // Set the emission point to the owner's position
        // In a full implementation, we would get the actual position
        particleSystem->createTrail(0.0f, 0.0f, 0.0f, endX, endY, endZ, duration);
    }
    
    void ParticleComponent::emitShockwave(float radius, float intensity) {
        if (!particleSystem || !owner) return;
        
        // Set the emission point to the owner's position
        // In a full implementation, we would get the actual position
        particleSystem->createShockwave(0.0f, 0.0f, 0.0f, radius, intensity);
    }
    
    void ParticleComponent::emitExplosionWithSmoke(float intensity) {
        if (!particleSystem || !owner) return;
        
        // Set the emission point to the owner's position
        // In a full implementation, we would get the actual position
        particleSystem->createExplosionWithSmoke(0.0f, 0.0f, 0.0f, intensity);
    }
    
    void ParticleComponent::emitFireWithSmoke(float intensity) {
        if (!particleSystem || !owner) return;
        
        // Set the emission point to the owner's position
        // In a full implementation, we would get the actual position
        particleSystem->createFireWithSmoke(0.0f, 0.0f, 0.0f, intensity);
    }
}