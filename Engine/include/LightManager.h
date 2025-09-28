#pragma once

#include <vector>
#include <memory>
#include "Light.h"

namespace Sparky {

    const int MAX_POINT_LIGHTS = 32;
    const int MAX_SPOT_LIGHTS = 32;

    class ShaderProgram;

    class LightManager {
    private:
        std::unique_ptr<Light> directionalLight;
        std::vector<std::unique_ptr<Light>> pointLights;
        std::vector<std::unique_ptr<Light>> spotLights;

    public:
        LightManager();
        ~LightManager();

        void setDirectionalLight(std::unique_ptr<Light> light);
        void addPointLight(std::unique_ptr<Light> light);
        void addSpotLight(std::unique_ptr<Light> light);

        void updateShader(ShaderProgram* shaderProgram);
    };

}