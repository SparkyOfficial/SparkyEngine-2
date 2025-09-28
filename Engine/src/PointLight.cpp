#include "../include/PointLight.h"

namespace Sparky {

    PointLight::PointLight(const std::string& name) : Light(name) {
        type = LightType::POINT;
        // Point lights don't use direction
        direction = glm::vec3(0.0f);
    }

    PointLight::~PointLight() {
    }
}