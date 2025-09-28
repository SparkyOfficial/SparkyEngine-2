#include "../include/Light.h"

namespace Sparky {

    Light::Light(const std::string& name) : name(name), type(LightType::DIRECTIONAL),
                                           position(0.0f, 0.0f, 0.0f), color(1.0f, 1.0f, 1.0f), 
                                           intensity(1.0f), enabled(true),
                                           constant(1.0f), linear(0.09f), quadratic(0.032f),
                                           direction(0.0f, -1.0f, 0.0f), 
                                           cutOff(glm::cos(glm::radians(12.5f))),
                                           outerCutOff(glm::cos(glm::radians(15.0f))) {
    }

    Light::~Light() {
    }

    void Light::setPosition(const glm::vec3& position) {
        this->position = position;
    }

    void Light::setColor(const glm::vec3& color) {
        this->color = color;
    }

    void Light::setIntensity(float intensity) {
        this->intensity = intensity;
    }

    void Light::setEnabled(bool enabled) {
        this->enabled = enabled;
    }

    void Light::setConstant(float constant) {
        this->constant = constant;
    }

    void Light::setLinear(float linear) {
        this->linear = linear;
    }

    void Light::setQuadratic(float quadratic) {
        this->quadratic = quadratic;
    }

    void Light::setDirection(const glm::vec3& direction) {
        this->direction = direction;
    }

    void Light::setCutOff(float cutOff) {
        this->cutOff = cutOff;
    }

    void Light::setOuterCutOff(float outerCutOff) {
        this->outerCutOff = outerCutOff;
    }
}