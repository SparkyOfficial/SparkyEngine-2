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
}