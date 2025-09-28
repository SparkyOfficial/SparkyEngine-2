#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>

namespace Sparky {
    struct Keyframe {
        float time;
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;
    };

    class Animation {
    public:
        Animation(const std::string& name, float duration);
        ~Animation();

        // Keyframe management
        void addKeyframe(const Keyframe& keyframe);
        void removeKeyframe(int index);
        Keyframe getKeyframe(int index) const;
        int getKeyframeCount() const { return static_cast<int>(keyframes.size()); }

        // Animation properties
        const std::string& getName() const { return name; }
        float getDuration() const { return duration; }

        // Animation evaluation
        void evaluate(float time, glm::vec3& position, glm::vec3& rotation, glm::vec3& scale) const;

        // Interpolation methods
        static glm::vec3 interpolateLinear(const glm::vec3& a, const glm::vec3& b, float t);
        static glm::vec3 interpolateSmooth(const glm::vec3& a, const glm::vec3& b, float t);

    private:
        std::string name;
        float duration;
        std::vector<Keyframe> keyframes;

        // Helper methods
        int findKeyframeIndex(float time) const;
    };
}