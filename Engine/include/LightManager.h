#pragma once

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>

namespace Sparky {
    class Light;
    class DirectionalLight;
    class PointLight;
    class SpotLight;

    class LightManager {
    public:
        static LightManager& getInstance();

        // Light management
        void addLight(std::unique_ptr<Light> light);
        void removeLight(const std::string& name);
        Light* getLight(const std::string& name);

        // Specific light creation
        DirectionalLight* createDirectionalLight(const std::string& name);
        PointLight* createPointLight(const std::string& name);
        SpotLight* createSpotLight(const std::string& name);

        // Get lights by type
        std::vector<DirectionalLight*> getDirectionalLights();
        std::vector<PointLight*> getPointLights();
        std::vector<SpotLight*> getSpotLights();

        // Update all lights
        void update(float deltaTime);

        // Apply lighting to shader
        void applyLighting(class ShaderProgram* shader, const glm::vec3& cameraPosition);

    private:
        LightManager();
        ~LightManager();

        std::unordered_map<std::string, std::unique_ptr<Light>> lights;
    };
}