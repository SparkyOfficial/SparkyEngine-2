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

    void Light::fillUniformBufferObject(LightUniformBufferObject& ubo) const {
        // Set position with w component indicating light type
        ubo.position = glm::vec4(position, type == LightType::DIRECTIONAL ? 0.0f : 1.0f);
        ubo.direction = glm::vec4(direction, 0.0f);
        ubo.ambient = glm::vec4(ambient, 1.0f);
        ubo.diffuse = glm::vec4(diffuse, 1.0f);
        ubo.specular = glm::vec4(specular, 1.0f);
        ubo.constant = constant;
        ubo.linear = linear;
        ubo.quadratic = quadratic;
        ubo.cutOff = cutOff;
        ubo.outerCutOff = outerCutOff;
    }

    void Light::fillUniformBufferObjects(std::vector<LightUniformBufferObject>& ubos, 
                                      const std::vector<std::unique_ptr<Light>>& lights) {
        ubos.resize(lights.size());
        for (size_t i = 0; i < lights.size(); ++i) {
            lights[i]->fillUniformBufferObject(ubos[i]);
        }
    }
}