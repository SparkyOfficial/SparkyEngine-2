#pragma once

#include <glm/glm.hpp>
#include <string>

namespace Sparky {
    enum class LightType {
        DIRECTIONAL,
        POINT,
        SPOT
    };

    class Light {
    public:
        Light(const std::string& name = "Light");
        virtual ~Light();

        // Basic properties
        void setPosition(const glm::vec3& position);
        void setColor(const glm::vec3& color);
        void setIntensity(float intensity);
        void setEnabled(bool enabled);

        const glm::vec3& getPosition() const { return position; }
        const glm::vec3& getColor() const { return color; }
        float getIntensity() const { return intensity; }
        bool isEnabled() const { return enabled; }

        // Light type
        LightType getType() const { return type; }

        // Attenuation (for point and spot lights)
        void setConstant(float constant);
        void setLinear(float linear);
        void setQuadratic(float quadratic);

        float getConstant() const { return constant; }
        float getLinear() const { return linear; }
        float getQuadratic() const { return quadratic; }

        // Spot light properties
        void setDirection(const glm::vec3& direction);
        void setCutOff(float cutOff);
        void setOuterCutOff(float outerCutOff);

        const glm::vec3& getDirection() const { return direction; }
        float getCutOff() const { return cutOff; }
        float getOuterCutOff() const { return outerCutOff; }

        const std::string& getName() const { return name; }

    protected:
        std::string name;
        LightType type;

        glm::vec3 position;
        glm::vec3 color;
        float intensity;
        bool enabled;

        // Attenuation parameters
        float constant;
        float linear;
        float quadratic;

        // Spot light parameters
        glm::vec3 direction;
        float cutOff;
        float outerCutOff;
    };
}