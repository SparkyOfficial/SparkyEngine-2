#include "../include/Animation.h"
#include "../include/Logger.h"
#include <algorithm>
#include <cmath>

namespace Sparky {

    Animation::Animation(const std::string& name, float duration) : 
        name(name), duration(duration) {
        SPARKY_LOG_DEBUG("Created animation: " + name + " with duration: " + std::to_string(duration));
    }

    Animation::~Animation() {
    }

    void Animation::addKeyframe(const Keyframe& keyframe) {
        keyframes.push_back(keyframe);
        
        // Keep keyframes sorted by time
        std::sort(keyframes.begin(), keyframes.end(), 
                 [](const Keyframe& a, const Keyframe& b) { return a.time < b.time; });
        
        SPARKY_LOG_DEBUG("Added keyframe to animation: " + name);
    }

    void Animation::removeKeyframe(int index) {
        if (index >= 0 && index < static_cast<int>(keyframes.size())) {
            keyframes.erase(keyframes.begin() + index);
            SPARKY_LOG_DEBUG("Removed keyframe from animation: " + name);
        }
    }

    Keyframe Animation::getKeyframe(int index) const {
        if (index >= 0 && index < static_cast<int>(keyframes.size())) {
            return keyframes[index];
        }
        return Keyframe{0.0f, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f)};
    }

    void Animation::evaluate(float time, glm::vec3& position, glm::vec3& rotation, glm::vec3& scale) const {
        if (keyframes.empty()) {
            position = glm::vec3(0.0f);
            rotation = glm::vec3(0.0f);
            scale = glm::vec3(1.0f);
            return;
        }

        // Wrap time to animation duration
        time = fmod(time, duration);
        if (time < 0.0f) time += duration;

        // Find the two keyframes to interpolate between
        int index = findKeyframeIndex(time);
        
        if (index == static_cast<int>(keyframes.size()) - 1) {
            // Last keyframe
            const Keyframe& keyframe = keyframes[index];
            position = keyframe.position;
            rotation = keyframe.rotation;
            scale = keyframe.scale;
        } else {
            // Interpolate between two keyframes
            const Keyframe& keyframe1 = keyframes[index];
            const Keyframe& keyframe2 = keyframes[index + 1];
            
            float t = (time - keyframe1.time) / (keyframe2.time - keyframe1.time);
            
            // Use smooth interpolation
            position = interpolateSmooth(keyframe1.position, keyframe2.position, t);
            rotation = interpolateSmooth(keyframe1.rotation, keyframe2.rotation, t);
            scale = interpolateSmooth(keyframe1.scale, keyframe2.scale, t);
        }
    }

    glm::vec3 Animation::interpolateLinear(const glm::vec3& a, const glm::vec3& b, float t) {
        return a + (b - a) * t;
    }

    glm::vec3 Animation::interpolateSmooth(const glm::vec3& a, const glm::vec3& b, float t) {
        // Smooth interpolation using cubic easing
        t = t * t * (3.0f - 2.0f * t);
        return a + (b - a) * t;
    }

    int Animation::findKeyframeIndex(float time) const {
        // Binary search for the keyframe
        int left = 0;
        int right = static_cast<int>(keyframes.size()) - 1;
        
        while (left < right) {
            int mid = (left + right) / 2;
            if (keyframes[mid].time < time) {
                left = mid + 1;
            } else {
                right = mid;
            }
        }
        
        return left > 0 ? left - 1 : 0;
    }
}