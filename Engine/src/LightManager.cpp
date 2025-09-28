#include "../include/LightManager.h"
#include "../include/Logger.h"
#include "../include/ShaderProgram.h"
#include <glm/gtc/type_ptr.hpp>

namespace Sparky {

    LightManager::LightManager() {
        SPARKY_LOG_DEBUG("Creating LightManager");
    }

    LightManager::~LightManager() {
        SPARKY_LOG_DEBUG("Destroying LightManager");
    }

    void LightManager::setDirectionalLight(std::unique_ptr<Light> light) {
        directionalLight = std::move(light);
        SPARKY_LOG_DEBUG("Set directional light");
    }

    void LightManager::addPointLight(std::unique_ptr<Light> light) {
        pointLights.push_back(std::move(light));
        SPARKY_LOG_DEBUG("Added point light. Total point lights: " + std::to_string(pointLights.size()));
    }

    void LightManager::addSpotLight(std::unique_ptr<Light> light) {
        spotLights.push_back(std::move(light));
        SPARKY_LOG_DEBUG("Added spot light. Total spot lights: " + std::to_string(spotLights.size()));
    }

    void LightManager::updateShader(ShaderProgram* shaderProgram) {
        if (!shaderProgram) {
            SPARKY_LOG_WARNING("ShaderProgram is null");
            return;
        }

        // Update directional light
        if (directionalLight) {
            // In a real implementation, we would set the actual uniform values
            // For now, we'll just call the methods to avoid compilation errors
            shaderProgram->setVec3("dirLight.direction", directionalLight->getDirection());
            shaderProgram->setVec3("dirLight.ambient", directionalLight->getAmbient());
            shaderProgram->setVec3("dirLight.diffuse", directionalLight->getDiffuse());
            shaderProgram->setVec3("dirLight.specular", directionalLight->getSpecular());
        }

        // Update point lights
        for (size_t i = 0; i < pointLights.size() && i < MAX_POINT_LIGHTS; ++i) {
            // In a real implementation, we would set the actual uniform values
            // For now, we'll just call the methods to avoid compilation errors
            shaderProgram->setVec3("pointLights[" + std::to_string(i) + "].position", pointLights[i]->getPosition());
            shaderProgram->setVec3("pointLights[" + std::to_string(i) + "].ambient", pointLights[i]->getAmbient());
            shaderProgram->setVec3("pointLights[" + std::to_string(i) + "].diffuse", pointLights[i]->getDiffuse());
            shaderProgram->setVec3("pointLights[" + std::to_string(i) + "].specular", pointLights[i]->getSpecular());
            shaderProgram->setFloat("pointLights[" + std::to_string(i) + "].constant", pointLights[i]->getConstant());
            shaderProgram->setFloat("pointLights[" + std::to_string(i) + "].linear", pointLights[i]->getLinear());
            shaderProgram->setFloat("pointLights[" + std::to_string(i) + "].quadratic", pointLights[i]->getQuadratic());
        }

        // Set the number of point lights
        shaderProgram->setInt("pointLightCount", static_cast<int>(std::min(pointLights.size(), static_cast<size_t>(MAX_POINT_LIGHTS))));

        // Update spot lights
        for (size_t i = 0; i < spotLights.size() && i < MAX_SPOT_LIGHTS; ++i) {
            // In a real implementation, we would set the actual uniform values
            // For now, we'll just call the methods to avoid compilation errors
            shaderProgram->setVec3("spotLights[" + std::to_string(i) + "].position", spotLights[i]->getPosition());
            shaderProgram->setVec3("spotLights[" + std::to_string(i) + "].direction", spotLights[i]->getDirection());
            shaderProgram->setVec3("spotLights[" + std::to_string(i) + "].ambient", spotLights[i]->getAmbient());
            shaderProgram->setVec3("spotLights[" + std::to_string(i) + "].diffuse", spotLights[i]->getDiffuse());
            shaderProgram->setVec3("spotLights[" + std::to_string(i) + "].specular", spotLights[i]->getSpecular());
            shaderProgram->setFloat("spotLights[" + std::to_string(i) + "].constant", spotLights[i]->getConstant());
            shaderProgram->setFloat("spotLights[" + std::to_string(i) + "].linear", spotLights[i]->getLinear());
            shaderProgram->setFloat("spotLights[" + std::to_string(i) + "].quadratic", spotLights[i]->getQuadratic());
            shaderProgram->setFloat("spotLights[" + std::to_string(i) + "].cutOff", spotLights[i]->getCutOff());
            shaderProgram->setFloat("spotLights[" + std::to_string(i) + "].outerCutOff", spotLights[i]->getOuterCutOff());
        }

        // Set the number of spot lights
        shaderProgram->setInt("spotLightCount", static_cast<int>(std::min(spotLights.size(), static_cast<size_t>(MAX_SPOT_LIGHTS))));
    }

}