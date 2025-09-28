#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <memory>

namespace Sparky {
    struct Particle {
        glm::vec3 position;
        glm::vec3 velocity;
        glm::vec4 color;
        float life;
        float maxLife;
        float size;
        bool active;
    };

    class ParticleSystem {
    public:
        ParticleSystem(int maxParticles = 1000);
        ~ParticleSystem();

        // Particle emission
        void emitParticle(const glm::vec3& position, const glm::vec3& velocity, 
                         const glm::vec4& color, float life, float size);
        void emitParticles(int count, const glm::vec3& position, const glm::vec3& baseVelocity,
                          const glm::vec4& color, float life, float size, float spread = 1.0f);

        // System properties
        void setEmissionRate(float rate);
        void setGravity(const glm::vec3& gravity);
        void setLifetime(float minLife, float maxLife);
        void setStartSize(float minSize, float maxSize);
        void setStartColor(const glm::vec4& minColor, const glm::vec4& maxColor);

        float getEmissionRate() const { return emissionRate; }
        const glm::vec3& getGravity() const { return gravity; }

        // Update and render
        void update(float deltaTime);
        void render();

        // System control
        void start();
        void stop();
        void reset();

        bool isRunning() const { return running; }
        int getActiveParticleCount() const;

    private:
        std::vector<Particle> particles;
        int maxParticles;
        bool running;

        // Emission properties
        float emissionRate;
        float emissionAccumulator;
        glm::vec3 gravity;

        // Particle properties
        float minLife, maxLife;
        float minSize, maxSize;
        glm::vec4 minColor, maxColor;
    };
}