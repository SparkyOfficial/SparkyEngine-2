#include "../include/SpotLight.h"

namespace Sparky {

    SpotLight::SpotLight(const std::string& name) : Light(name) {
        type = LightType::SPOT;
    }

    SpotLight::~SpotLight() {
    }
}