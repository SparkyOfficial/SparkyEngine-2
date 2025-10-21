#include "../include/DirectionalLight.h"

namespace Sparky {

    DirectionalLight::DirectionalLight(const std::string& name) : Light(name) {
        type = LightType::DIRECTIONAL;
        // Directional lights don't use position or attenuation
        position = glm::vec3(0.0f);
        constant = 1.0f;
        linear = 0.0f;
        quadratic = 0.0f;
    }

    DirectionalLight::~DirectionalLight() {
    }

    void DirectionalLight::setDirection(const glm::vec3& direction) {
        this->direction = direction;
    }
    
    void DirectionalLight::setColor(const glm::vec3& color) {
        setDiffuse(color);
        setSpecular(color * 0.3f); // Set specular to 30% of diffuse by default
    }
    
    void DirectionalLight::setIntensity(float intensity) {
        // Scale the diffuse and specular colors by the intensity
        glm::vec3 currentDiffuse = getDiffuse();
        glm::vec3 currentSpecular = getSpecular();
        
        setDiffuse(currentDiffuse * intensity);
        setSpecular(currentSpecular * intensity);
    }
}