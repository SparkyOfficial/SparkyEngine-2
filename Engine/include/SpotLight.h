#pragma once

#include "Light.h"

namespace Sparky {
    class SpotLight : public Light {
    public:
        SpotLight(const std::string& name = "SpotLight");
        ~SpotLight();
    };
}