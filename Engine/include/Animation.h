#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>

namespace Sparky {
    struct KeyFrame {
        float time;
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;
    };

    class Animation {
    public:
        Animation(const std::string& name);
        ~Animation();

        void addKeyFrame(const KeyFrame& keyFrame);
        void setDuration(float duration);
        float getDuration() const { return duration; }
        
        const std::string& getName() const { return name; }
        const std::vector<KeyFrame>& getKeyFrames() const { return keyFrames; }

        // Animation evaluation
        void evaluate(float time, glm::vec3& position, glm::vec3& rotation, glm::vec3& scale) const;

    private:
        std::string name;
        std::vector<KeyFrame> keyFrames;
        float duration;
        
        // Helper methods
        KeyFrame interpolate(const KeyFrame& a, const KeyFrame& b, float t) const;
    };
}