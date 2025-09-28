#pragma once

#include "Light.h"

namespace Sparky {
    class DirectionalLight : public Light {
    public:
        DirectionalLight(const std::string& name = "DirectionalLight");
        ~DirectionalLight();

        // Directional lights don't have position, they have direction
        void setDirection(const glm::vec3& direction);
        const glm::vec3& getDirection() const { return direction; }
    };
}