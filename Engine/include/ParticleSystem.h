#pragma once

#include <vector>
#include <memory>

namespace Sparky {
    
    // Particle types for different visual effects
    enum class ParticleType {
        DEFAULT,
        EXPLOSION,
        SMOKE,
        FIRE,
        SPARK,
        BLOOD,
        MIST,
        TRAIL
    };
    
    // Advanced particle properties
    struct ParticleProperties {
        float position[3];        // x, y, z
        float velocity[3];        // x, y, z
        float acceleration[3];    // x, y, z
        float startColor[4];      // r, g, b, a
        float endColor[4];        // r, g, b, a
        float startSize;
        float endSize;
        float life;
        float maxLife;
        float rotation;
        float rotationSpeed;
        float mass;
        ParticleType type;
        
        ParticleProperties();
    };
    
    // Emitter properties for different effects
    struct EmitterProperties {
        float position[3];           // x, y, z
        float direction[3];          // x, y, z
        float spread;
        float speed;
        float speedVariation;
        float emissionRate;
        float lifetime;
        float lifetimeVariation;
        float startColor[4];         // r, g, b, a
        float endColor[4];           // r, g, b, a
        float startSize;
        float endSize;
        float sizeVariation;
        float gravity[3];            // x, y, z
        float acceleration[3];       // x, y, z
        float rotationSpeed;
        float rotationSpeedVariation;
        float mass;
        float massVariation;
        ParticleType type;
        
        EmitterProperties();
    };
    
    // Particle texture (for future implementation)
    class ParticleTexture {
    public:
        ParticleTexture();
        ~ParticleTexture();
        
        // In a full implementation, this would contain texture data
        // For now, we'll just use it as a placeholder
        int getTextureID() const { return textureID; }
        
    private:
        int textureID;
    };
    
    class ParticleSystem {
    public:
        ParticleSystem();
        ~ParticleSystem();
        
        virtual void update(float deltaTime);
        virtual void render();
        
        // Configuration methods
        void setMaxParticles(int max) { maxParticles = max; particles.resize(max); }
        void setEmitterProperties(const EmitterProperties& props) { emitterProps = props; }
        void setTexture(std::shared_ptr<ParticleTexture> tex) { texture = tex; }
        void setActive(bool active);
        
        // Getters
        bool isActive() const;
        int getParticleCount() const;
        int getMaxParticles() const { return maxParticles; }
        const EmitterProperties& getEmitterProperties() const { return emitterProps; }
        
        // Particle emission methods
        void emitParticles(float deltaTime);
        void emitBurst(int count);
        void emitParticle();
        
        // Advanced preset particle effects
        void createExplosion(float x, float y, float z, float intensity = 1.0f);
        void createSmoke(float x, float y, float z, float density = 1.0f);
        void createFire(float x, float y, float z, float intensity = 1.0f);
        void createSparks(float x, float y, float z, int count = 10);
        void createBlood(float x, float y, float z, float dirX, float dirY, float dirZ, int count = 5);
        void createMist(float x, float y, float z, float density = 1.0f);
        void createTrail(float startX, float startY, float startZ, float endX, float endY, float endZ, float duration = 1.0f);
        void createShockwave(float x, float y, float z, float radius = 5.0f, float intensity = 1.0f);
        
        // Effect combinations
        void createExplosionWithSmoke(float x, float y, float z, float intensity = 1.0f);
        void createFireWithSmoke(float x, float y, float z, float intensity = 1.0f);

    protected:
        // Protected helper methods that can be accessed by derived classes
        virtual void updateParticle(ParticleProperties& particle, float deltaTime);
        virtual void applyPhysics(ParticleProperties& particle, float deltaTime);
        void interpolateColor(float* result, const float* start, const float* end, float ratio);
        float interpolateFloat(float start, float end, float ratio);

    private:
        std::vector<ParticleProperties> particles;
        int maxParticles;
        int particleCount;
        
        // Emitter properties
        EmitterProperties emitterProps;
        float lastEmissionTime;
        
        // Texture
        std::shared_ptr<ParticleTexture> texture;
        
        bool systemActive;
    };
}