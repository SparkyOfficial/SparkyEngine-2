#pragma once

#include "Light.h"

namespace Sparky {
    class PointLight : public Light {
    public:
        PointLight(const std::string& name = "PointLight");
        ~PointLight();
    };
}