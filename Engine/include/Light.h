#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace Sparky {

    enum class LightType {
        DIRECTIONAL,
        POINT,
        SPOT
    };

    // Structure for uniform buffer objects
    struct LightUniformBufferObject {
        glm::vec4 position;     // w component indicates light type (1.0 for point, 0.0 for directional)
        glm::vec4 direction;    // w component unused
        glm::vec4 ambient;
        glm::vec4 diffuse;
        glm::vec4 specular;
        float constant;
        float linear;
        float quadratic;
        float cutOff;
        float outerCutOff;
        float padding[3];       // For alignment to 16-byte boundaries
    };

    class Light {
    protected:
        std::string name;
        LightType type;
        glm::vec3 position;
        glm::vec3 direction;
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        float constant;
        float linear;
        float quadratic;
        float cutOff;
        float outerCutOff;

    public:
        Light(const std::string& name = "Light");
        virtual ~Light();

        // Getters
        const std::string& getName() const { return name; }
        LightType getType() const { return type; }
        const glm::vec3& getPosition() const { return position; }
        const glm::vec3& getDirection() const { return direction; }
        const glm::vec3& getAmbient() const { return ambient; }
        const glm::vec3& getDiffuse() const { return diffuse; }
        const glm::vec3& getSpecular() const { return specular; }
        float getConstant() const { return constant; }
        float getLinear() const { return linear; }
        float getQuadratic() const { return quadratic; }
        float getCutOff() const { return cutOff; }
        float getOuterCutOff() const { return outerCutOff; }

        // Setters
        void setName(const std::string& n) { name = n; }
        void setType(LightType t) { type = t; }
        void setPosition(const glm::vec3& pos) { position = pos; }
        void setDirection(const glm::vec3& dir) { direction = dir; }
        void setAmbient(const glm::vec3& amb) { ambient = amb; }
        void setDiffuse(const glm::vec3& diff) { diffuse = diff; }
        void setSpecular(const glm::vec3& spec) { specular = spec; }
        void setConstant(float c) { constant = c; }
        void setLinear(float l) { linear = l; }
        void setQuadratic(float q) { quadratic = q; }
        void setCutOff(float co) { cutOff = co; }
        void setOuterCutOff(float oco) { outerCutOff = oco; }

        // Vulkan-specific methods
        void fillUniformBufferObject(LightUniformBufferObject& ubo) const;
        static void fillUniformBufferObjects(std::vector<LightUniformBufferObject>& ubos, 
                                           const std::vector<std::unique_ptr<Light>>& lights);
    };

}