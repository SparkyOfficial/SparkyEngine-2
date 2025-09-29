#include "../include/Animation.h"
#include "../include/Logger.h"
#include <algorithm>
#include <cmath>

namespace Sparky {

    Animation::Animation(const std::string& name) : name(name), duration(0.0f) {
    }

    Animation::~Animation() {
    }

    void Animation::addKeyFrame(const KeyFrame& keyFrame) {
        keyFrames.push_back(keyFrame);
        
        // Update duration if this keyframe is later than current duration
        if (keyFrame.time > duration) {
            duration = keyFrame.time;
        }
        
        // Sort keyframes by time
        std::sort(keyFrames.begin(), keyFrames.end(), 
                  [](const KeyFrame& a, const KeyFrame& b) {
                      return a.time < b.time;
                  });
    }

    void Animation::setDuration(float duration) {
        this->duration = duration;
    }

    void Animation::evaluate(float time, glm::vec3& position, glm::vec3& rotation, glm::vec3& scale) const {
        if (keyFrames.empty()) {
            position = glm::vec3(0.0f);
            rotation = glm::vec3(0.0f);
            scale = glm::vec3(1.0f);
            return;
        }
        
        // Wrap time if it exceeds duration
        if (duration > 0.0f) {
            time = fmod(time, duration);
        }
        
        // Find the keyframes to interpolate between
        if (keyFrames.size() == 1) {
            // Only one keyframe, use its values
            position = keyFrames[0].position;
            rotation = keyFrames[0].rotation;
            scale = keyFrames[0].scale;
            return;
        }
        
        // Find the two keyframes to interpolate between
        size_t nextIndex = 0;
        while (nextIndex < keyFrames.size() && keyFrames[nextIndex].time <= time) {
            nextIndex++;
        }
        
        if (nextIndex == 0) {
            // Before first keyframe
            position = keyFrames[0].position;
            rotation = keyFrames[0].rotation;
            scale = keyFrames[0].scale;
            return;
        }
        
        if (nextIndex >= keyFrames.size()) {
            // After last keyframe
            position = keyFrames.back().position;
            rotation = keyFrames.back().rotation;
            scale = keyFrames.back().scale;
            return;
        }
        
        // Interpolate between keyframes
        const KeyFrame& prev = keyFrames[nextIndex - 1];
        const KeyFrame& next = keyFrames[nextIndex];
        
        float t = (time - prev.time) / (next.time - prev.time);
        KeyFrame interpolated = interpolate(prev, next, t);
        
        position = interpolated.position;
        rotation = interpolated.rotation;
        scale = interpolated.scale;
    }

    KeyFrame Animation::interpolate(const KeyFrame& a, const KeyFrame& b, float t) const {
        KeyFrame result;
        result.time = a.time + t * (b.time - a.time);
        result.position = glm::mix(a.position, b.position, t);
        result.rotation = glm::mix(a.rotation, b.rotation, t);
        result.scale = glm::mix(a.scale, b.scale, t);
        return result;
    }
}