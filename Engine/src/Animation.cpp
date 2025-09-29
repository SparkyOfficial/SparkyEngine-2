#include "../include/Animation.h"
#include "../include/Logger.h"
#include <algorithm>
#include <cmath>

namespace Sparky {
    
    // AnimationTrack implementation
    AnimationTrack::AnimationTrack(const std::string& name) : name(name) {
        // Add a default keyframe at time 0
        Keyframe defaultKeyframe;
        defaultKeyframe.time = 0.0f;
        defaultKeyframe.position = glm::vec3(0.0f);
        defaultKeyframe.rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f); // Identity quaternion
        defaultKeyframe.scale = glm::vec3(1.0f);
        keyframes.push_back(defaultKeyframe);
    }
    
    AnimationTrack::~AnimationTrack() {
    }
    
    void AnimationTrack::addKeyframe(const Keyframe& keyframe) {
        // Insert the keyframe in the correct time order
        auto it = std::upper_bound(keyframes.begin(), keyframes.end(), keyframe,
                                  [](const Keyframe& a, const Keyframe& b) {
                                      return a.time < b.time;
                                  });
        keyframes.insert(it, keyframe);
    }
    
    Keyframe AnimationTrack::getKeyframeAtTime(float time) const {
        if (keyframes.empty()) {
            Keyframe emptyKeyframe;
            emptyKeyframe.time = 0.0f;
            emptyKeyframe.position = glm::vec3(0.0f);
            emptyKeyframe.rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
            emptyKeyframe.scale = glm::vec3(1.0f);
            return emptyKeyframe;
        }
        
        // If time is before the first keyframe, return the first keyframe
        if (time <= keyframes.front().time) {
            return keyframes.front();
        }
        
        // If time is after the last keyframe, return the last keyframe
        if (time >= keyframes.back().time) {
            return keyframes.back();
        }
        
        // Find the two keyframes that bracket the time
        for (size_t i = 0; i < keyframes.size() - 1; ++i) {
            if (time >= keyframes[i].time && time <= keyframes[i+1].time) {
                // Interpolate between these two keyframes
                float t = (time - keyframes[i].time) / (keyframes[i+1].time - keyframes[i].time);
                return interpolateKeyframes(keyframes[i], keyframes[i+1], t);
            }
        }
        
        // Fallback - should not reach here
        return keyframes.back();
    }
    
    Keyframe AnimationTrack::interpolateKeyframes(const Keyframe& a, const Keyframe& b, float t) const {
        Keyframe result;
        result.time = a.time + t * (b.time - a.time);
        result.position = glm::mix(a.position, b.position, t);
        result.rotation = glm::slerp(a.rotation, b.rotation, t);
        result.scale = glm::mix(a.scale, b.scale, t);
        return result;
    }
    
    float AnimationTrack::getDuration() const {
        if (keyframes.empty()) return 0.0f;
        return keyframes.back().time;
    }
    
    // Animation implementation
    Animation::Animation(const std::string& name) : name(name), playing(false), looping(false), 
                                                   currentTime(0.0f), playbackSpeed(1.0f) {
        SPARKY_LOG_DEBUG("Animation created: " + name);
    }
    
    Animation::~Animation() {
        SPARKY_LOG_DEBUG("Animation destroyed: " + name);
    }
    
    void Animation::addTrack(std::unique_ptr<AnimationTrack> track) {
        tracks.push_back(std::move(track));
        SPARKY_LOG_DEBUG("Track added to animation: " + name);
    }
    
    AnimationTrack* Animation::getTrack(const std::string& name) const {
        for (const auto& track : tracks) {
            if (track->getName() == name) {
                return track.get();
            }
        }
        return nullptr;
    }
    
    AnimationTrack* Animation::getTrack(size_t index) const {
        if (index >= tracks.size()) {
            return nullptr;
        }
        return tracks[index].get();
    }
    
    float Animation::getDuration() const {
        float maxDuration = 0.0f;
        for (const auto& track : tracks) {
            maxDuration = std::max(maxDuration, track->getDuration());
        }
        return maxDuration;
    }
    
    void Animation::update(float deltaTime) {
        if (!playing) return;
        
        currentTime += deltaTime * playbackSpeed;
        
        float duration = getDuration();
        if (duration > 0.0f) {
            if (currentTime > duration) {
                if (looping) {
                    currentTime = std::fmod(currentTime, duration);
                } else {
                    currentTime = duration;
                    playing = false;
                }
            }
        }
    }
    
    void Animation::play(bool loop) {
        playing = true;
        looping = loop;
        SPARKY_LOG_DEBUG("Animation playing: " + name);
    }
    
    void Animation::pause() {
        playing = false;
        SPARKY_LOG_DEBUG("Animation paused: " + name);
    }
    
    void Animation::stop() {
        playing = false;
        currentTime = 0.0f;
        SPARKY_LOG_DEBUG("Animation stopped: " + name);
    }
}