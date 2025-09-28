#include "../include/Light.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Sparky {

    Light::Light(const std::string& name) : name(name), type(LightType::DIRECTIONAL),
                                           position(0.0f, 0.0f, 0.0f), 
                                           direction(0.0f, -1.0f, 0.0f),
                                           ambient(0.1f, 0.1f, 0.1f),
                                           diffuse(0.8f, 0.8f, 0.8f), 
                                           specular(1.0f, 1.0f, 1.0f),
                                           constant(1.0f), linear(0.09f), quadratic(0.032f),
                                           cutOff(glm::cos(glm::radians(12.5f))),
                                           outerCutOff(glm::cos(glm::radians(15.0f))) {
    }

    Light::~Light() {
    }

    // We can add any additional methods that aren't inline in the header here
    // But we'll remove the duplicate definitions that are already in the header
}