#include "../include/LightManager.h"
#include "../include/DirectionalLight.h"
#include "../include/PointLight.h"
#include "../include/SpotLight.h"
#include "../include/ShaderProgram.h"
#include "../include/Logger.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Sparky {

    LightManager::LightManager() {
    }

    LightManager::~LightManager() {
    }

    LightManager& LightManager::getInstance() {
        static LightManager instance;
        return instance;
    }

    void LightManager::addLight(std::unique_ptr<Light> light) {
        if (light) {
            const std::string& name = light->getName();
            lights[name] = std::move(light);
            SPARKY_LOG_DEBUG("Added light: " + name);
        }
    }

    void LightManager::removeLight(const std::string& name) {
        auto it = lights.find(name);
        if (it != lights.end()) {
            lights.erase(it);
            SPARKY_LOG_DEBUG("Removed light: " + name);
        }
    }

    Light* LightManager::getLight(const std::string& name) {
        auto it = lights.find(name);
        if (it != lights.end()) {
            return it->second.get();
        }
        return nullptr;
    }

    DirectionalLight* LightManager::createDirectionalLight(const std::string& name) {
        auto light = std::make_unique<DirectionalLight>(name);
        DirectionalLight* lightPtr = light.get();
        addLight(std::move(light));
        return lightPtr;
    }

    PointLight* LightManager::createPointLight(const std::string& name) {
        auto light = std::make_unique<PointLight>(name);
        PointLight* lightPtr = light.get();
        addLight(std::move(light));
        return lightPtr;
    }

    SpotLight* LightManager::createSpotLight(const std::string& name) {
        auto light = std::make_unique<SpotLight>(name);
        SpotLight* lightPtr = light.get();
        addLight(std::move(light));
        return lightPtr;
    }

    std::vector<DirectionalLight*> LightManager::getDirectionalLights() {
        std::vector<DirectionalLight*> result;
        for (auto& pair : lights) {
            if (pair.second->getType() == LightType::DIRECTIONAL) {
                result.push_back(static_cast<DirectionalLight*>(pair.second.get()));
            }
        }
        return result;
    }

    std::vector<PointLight*> LightManager::getPointLights() {
        std::vector<PointLight*> result;
        for (auto& pair : lights) {
            if (pair.second->getType() == LightType::POINT) {
                result.push_back(static_cast<PointLight*>(pair.second.get()));
            }
        }
        return result;
    }

    std::vector<SpotLight*> LightManager::getSpotLights() {
        std::vector<SpotLight*> result;
        for (auto& pair : lights) {
            if (pair.second->getType() == LightType::SPOT) {
                result.push_back(static_cast<SpotLight*>(pair.second.get()));
            }
        }
        return result;
    }

    void LightManager::update(float deltaTime) {
        // Update light animations, flickering, etc.
        // For now, just log that we're updating
        SPARKY_LOG_DEBUG("Updating lights");
    }

    void LightManager::applyLighting(ShaderProgram* shader, const glm::vec3& cameraPosition) {
        if (!shader) {
            return;
        }

        // Use the shader
        shader->use();

        // Apply directional lights
        auto dirLights = getDirectionalLights();
        shader->setInt("directionalLightCount", static_cast<int>(dirLights.size()));
        for (size_t i = 0; i < dirLights.size(); i++) {
            std::string prefix = "directionalLights[" + std::to_string(i) + "].";
            
            if (dirLights[i]->isEnabled()) {
                shader->setVec3(prefix + "direction", dirLights[i]->getDirection());
                shader->setVec3(prefix + "color", dirLights[i]->getColor());
                shader->setFloat(prefix + "intensity", dirLights[i]->getIntensity());
            }
        }

        // Apply point lights
        auto pointLights = getPointLights();
        shader->setInt("pointLightCount", static_cast<int>(pointLights.size()));
        for (size_t i = 0; i < pointLights.size(); i++) {
            std::string prefix = "pointLights[" + std::to_string(i) + "].";
            
            if (pointLights[i]->isEnabled()) {
                shader->setVec3(prefix + "position", pointLights[i]->getPosition());
                shader->setVec3(prefix + "color", pointLights[i]->getColor());
                shader->setFloat(prefix + "intensity", pointLights[i]->getIntensity());
                
                // Attenuation
                shader->setFloat(prefix + "constant", pointLights[i]->getConstant());
                shader->setFloat(prefix + "linear", pointLights[i]->getLinear());
                shader->setFloat(prefix + "quadratic", pointLights[i]->getQuadratic());
            }
        }

        // Apply spot lights
        auto spotLights = getSpotLights();
        shader->setInt("spotLightCount", static_cast<int>(spotLights.size()));
        for (size_t i = 0; i < spotLights.size(); i++) {
            std::string prefix = "spotLights[" + std::to_string(i) + "].";
            
            if (spotLights[i]->isEnabled()) {
                shader->setVec3(prefix + "position", spotLights[i]->getPosition());
                shader->setVec3(prefix + "direction", spotLights[i]->getDirection());
                shader->setVec3(prefix + "color", spotLights[i]->getColor());
                shader->setFloat(prefix + "intensity", spotLights[i]->getIntensity());
                
                // Attenuation
                shader->setFloat(prefix + "constant", spotLights[i]->getConstant());
                shader->setFloat(prefix + "linear", spotLights[i]->getLinear());
                shader->setFloat(prefix + "quadratic", spotLights[i]->getQuadratic());
                
                // Spot light specific
                shader->setFloat(prefix + "cutOff", spotLights[i]->getCutOff());
                shader->setFloat(prefix + "outerCutOff", spotLights[i]->getOuterCutOff());
            }
        }

        // Set camera position for specular lighting
        shader->setVec3("viewPos", cameraPosition);
    }
}