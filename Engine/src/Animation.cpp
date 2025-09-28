#include "../include/Animation.h"
#include "../include/Logger.h"
#include <algorithm>
#include <cmath>

namespace Sparky {

    Animation::Animation(const std::string& name) 
        : name(name), duration(0.0f), currentTime(0.0f), isPlaying(false), loop(false) {
        SPARKY_LOG_DEBUG("Creating Animation: " + name);
    }

    Animation::~Animation() {
        SPARKY_LOG_DEBUG("Destroying Animation: " + name);
    }

    void Animation::addKeyFrame(const KeyFrame& keyFrame) {
        keyFrames.push_back(keyFrame);
        duration = std::max(duration, keyFrame.time);
        SPARKY_LOG_DEBUG("Added keyframe at time: " + std::to_string(keyFrame.time));
    }

    void Animation::play() {
        isPlaying = true;
        currentTime = 0.0f;
        SPARKY_LOG_DEBUG("Playing animation: " + name);
    }

    void Animation::stop() {
        isPlaying = false;
        currentTime = 0.0f;
        SPARKY_LOG_DEBUG("Stopping animation: " + name);
    }

    void Animation::update(float deltaTime) {
        if (!isPlaying) return;

        currentTime += deltaTime;

        if (currentTime >= duration) {
            if (loop) {
                currentTime = fmod(currentTime, duration);
                SPARKY_LOG_DEBUG("Looping animation: " + name);
            } else {
                currentTime = duration;
                isPlaying = false;
                SPARKY_LOG_DEBUG("Animation finished: " + name);
            }
        }
    }

    bool Animation::isFinished() const {
        return !isPlaying && currentTime >= duration;
    }

    glm::vec3 Animation::interpolatePosition(float time) const {
        if (keyFrames.empty()) {
            return glm::vec3(0.0f);
        }

        // Find the two keyframes to interpolate between
        for (size_t i = 0; i < keyFrames.size() - 1; ++i) {
            if (time >= keyFrames[i].time && time <= keyFrames[i + 1].time) {
                float t = (time - keyFrames[i].time) / (keyFrames[i + 1].time - keyFrames[i].time);
                return glm::mix(keyFrames[i].position, keyFrames[i + 1].position, t);
            }
        }

        // If time is before the first keyframe or after the last, return the closest one
        if (time <= keyFrames.front().time) {
            return keyFrames.front().position;
        } else {
            return keyFrames.back().position;
        }
    }

    glm::vec3 Animation::interpolateRotation(float time) const {
        if (keyFrames.empty()) {
            return glm::vec3(0.0f);
        }

        // Find the two keyframes to interpolate between
        for (size_t i = 0; i < keyFrames.size() - 1; ++i) {
            if (time >= keyFrames[i].time && time <= keyFrames[i + 1].time) {
                float t = (time - keyFrames[i].time) / (keyFrames[i + 1].time - keyFrames[i].time);
                return glm::mix(keyFrames[i].rotation, keyFrames[i + 1].rotation, t);
            }
        }

        // If time is before the first keyframe or after the last, return the closest one
        if (time <= keyFrames.front().time) {
            return keyFrames.front().rotation;
        } else {
            return keyFrames.back().rotation;
        }
    }

    glm::vec3 Animation::interpolateScale(float time) const {
        if (keyFrames.empty()) {
            return glm::vec3(1.0f);
        }

        // Find the two keyframes to interpolate between
        for (size_t i = 0; i < keyFrames.size() - 1; ++i) {
            if (time >= keyFrames[i].time && time <= keyFrames[i + 1].time) {
                float t = (time - keyFrames[i].time) / (keyFrames[i + 1].time - keyFrames[i].time);
                return glm::mix(keyFrames[i].scale, keyFrames[i + 1].scale, t);
            }
        }

        // If time is before the first keyframe or after the last, return the closest one
        if (time <= keyFrames.front().time) {
            return keyFrames.front().scale;
        } else {
            return keyFrames.back().scale;
        }
    }

}