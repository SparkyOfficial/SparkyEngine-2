#include "../include/Animation.h"
#include "../include/Logger.h"
#include <algorithm>
#include <cmath>

namespace Sparky {
    
    // BlendedKeyframe implementation
    BlendedKeyframe::BlendedKeyframe() {
        // Initialize arrays
        position[0] = position[1] = position[2] = 0.0f;
        rotation[0] = rotation[1] = rotation[2] = 0.0f; rotation[3] = 1.0f; // Identity quaternion
        scale[0] = scale[1] = scale[2] = 1.0f;
    }
    
    // AnimationTrack implementation
    AnimationTrack::AnimationTrack(const std::string& name) : name(name) {
        // Add a default keyframe at time 0
        Keyframe defaultKeyframe;
        defaultKeyframe.time = 0.0f;
        defaultKeyframe.position[0] = defaultKeyframe.position[1] = defaultKeyframe.position[2] = 0.0f;
        defaultKeyframe.rotation[0] = defaultKeyframe.rotation[1] = defaultKeyframe.rotation[2] = 0.0f;
        defaultKeyframe.rotation[3] = 1.0f; // Identity quaternion
        defaultKeyframe.scale[0] = defaultKeyframe.scale[1] = defaultKeyframe.scale[2] = 1.0f;
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
            emptyKeyframe.position[0] = emptyKeyframe.position[1] = emptyKeyframe.position[2] = 0.0f;
            emptyKeyframe.rotation[0] = emptyKeyframe.rotation[1] = emptyKeyframe.rotation[2] = 0.0f;
            emptyKeyframe.rotation[3] = 1.0f;
            emptyKeyframe.scale[0] = emptyKeyframe.scale[1] = emptyKeyframe.scale[2] = 1.0f;
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
        
        // Linear interpolation for position and scale
        for (int i = 0; i < 3; ++i) {
            result.position[i] = a.position[i] + t * (b.position[i] - a.position[i]);
            result.scale[i] = a.scale[i] + t * (b.scale[i] - a.scale[i]);
        }
        
        // Simple linear interpolation for rotation (quaternion) - not mathematically correct but works for small angles
        for (int i = 0; i < 4; ++i) {
            result.rotation[i] = a.rotation[i] + t * (b.rotation[i] - a.rotation[i]);
        }
        
        // Normalize the quaternion to maintain unit length
        float length = std::sqrt(result.rotation[0]*result.rotation[0] + 
                                result.rotation[1]*result.rotation[1] + 
                                result.rotation[2]*result.rotation[2] + 
                                result.rotation[3]*result.rotation[3]);
        if (length > 0.0f) {
            for (int i = 0; i < 4; ++i) {
                result.rotation[i] /= length;
            }
        }
        
        return result;
    }
    
    float AnimationTrack::getDuration() const {
        if (keyframes.empty()) return 0.0f;
        return keyframes.back().time;
    }
    
    // Animation implementation
    Animation::Animation(const std::string& name) : name(name), playing(false), looping(false), 
                                                   currentTime(0.0f), playbackSpeed(1.0f),
                                                   blendWeight(1.0f), crossfadeTime(-1.0f),
                                                   crossfadeDuration(0.0f), crossfadeOut(false) {
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
        
        // Update crossfade if active
        if (crossfadeTime >= 0.0f) {
            crossfadeTime += deltaTime;
            if (crossfadeTime >= crossfadeDuration) {
                crossfadeTime = -1.0f; // Crossfade complete
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
    
    void Animation::startCrossfade(float fadeDuration) {
        crossfadeTime = 0.0f;
        crossfadeDuration = fadeDuration;
        crossfadeOut = true;
    }
    
    // AnimationBlender implementation
    AnimationBlender::AnimationBlender() : crossfadeProgress(0.0f), crossfadeDuration(0.0f) {
    }
    
    AnimationBlender::~AnimationBlender() {
    }
    
    void AnimationBlender::addAnimation(Animation* animation) {
        if (animation) {
            animations[animation->getName()] = animation;
        }
    }
    
    void AnimationBlender::setBlendWeight(const std::string& animationName, float weight) {
        // Update or add blend info
        for (auto& info : blendInfos) {
            if (info.animationName == animationName) {
                info.weight = weight;
                return;
            }
        }
        // If not found, add new blend info
        blendInfos.emplace_back(animationName, weight);
    }
    
    void AnimationBlender::setBlendWeights(const std::vector<AnimationBlendInfo>& blendInfos) {
        this->blendInfos = blendInfos;
    }
    
    BlendedKeyframe AnimationBlender::getBlendedKeyframe(const std::string& trackName, float time) const {
        std::vector<std::pair<Keyframe, float>> keyframes;
        
        // Collect keyframes from all animations with their weights
        for (const auto& info : blendInfos) {
            auto it = animations.find(info.animationName);
            if (it != animations.end()) {
                Animation* anim = it->second;
                if (anim->isPlaying()) {
                    AnimationTrack* track = anim->getTrack(trackName);
                    if (track) {
                        float adjustedTime = time + info.timeOffset;
                        Keyframe keyframe = track->getKeyframeAtTime(adjustedTime);
                        keyframes.emplace_back(keyframe, info.weight * anim->getBlendWeight());
                    }
                }
            }
        }
        
        // Blend the keyframes
        return blendKeyframes(keyframes);
    }
    
    void AnimationBlender::update(float deltaTime) {
        // Handle crossfade if active
        if (!targetAnimation.empty()) {
            crossfadeProgress += deltaTime;
            if (crossfadeProgress >= crossfadeDuration) {
                // Crossfade complete
                currentAnimation = targetAnimation;
                targetAnimation.clear();
                crossfadeProgress = 0.0f;
                
                // Set the target animation to full weight and current to zero
                setBlendWeight(currentAnimation, 1.0f);
                for (auto& info : blendInfos) {
                    if (info.animationName != currentAnimation) {
                        info.weight = 0.0f;
                    }
                }
            } else {
                // Update blend weights during crossfade
                float progress = crossfadeProgress / crossfadeDuration;
                setBlendWeight(currentAnimation, 1.0f - progress);
                setBlendWeight(targetAnimation, progress);
            }
        }
    }
    
    void AnimationBlender::crossfade(const std::string& fromAnimation, const std::string& toAnimation, float duration) {
        // Check if both animations exist
        if (animations.find(fromAnimation) == animations.end() || 
            animations.find(toAnimation) == animations.end()) {
            return;
        }
        
        currentAnimation = fromAnimation;
        targetAnimation = toAnimation;
        crossfadeDuration = duration;
        crossfadeProgress = 0.0f;
        
        // Start both animations if they're not already playing
        Animation* fromAnim = animations[fromAnimation];
        Animation* toAnim = animations[toAnimation];
        
        if (!fromAnim->isPlaying()) {
            fromAnim->play(true);
        }
        if (!toAnim->isPlaying()) {
            toAnim->play(true);
        }
        
        // Initialize blend weights
        setBlendWeight(fromAnimation, 1.0f);
        setBlendWeight(toAnimation, 0.0f);
    }
    
    Animation* AnimationBlender::getAnimation(const std::string& name) const {
        auto it = animations.find(name);
        if (it != animations.end()) {
            return it->second;
        }
        return nullptr;
    }
    
    BlendedKeyframe AnimationBlender::blendKeyframes(const std::vector<std::pair<Keyframe, float>>& keyframes) const {
        BlendedKeyframe result;
        
        if (keyframes.empty()) {
            return result;
        }
        
        if (keyframes.size() == 1) {
            // Only one keyframe, no blending needed
            const Keyframe& kf = keyframes[0].first;
            float weight = keyframes[0].second;
            
            for (int i = 0; i < 3; ++i) {
                result.position[i] = kf.position[i] * weight;
                result.scale[i] = kf.scale[i] * weight;
            }
            
            // For rotation, we apply the weight directly
            result.rotation[0] = kf.rotation[0] * weight;
            result.rotation[1] = kf.rotation[1] * weight;
            result.rotation[2] = kf.rotation[2] * weight;
            result.rotation[3] = kf.rotation[3] * weight;
            
            return result;
        }
        
        // Multiple keyframes, blend them
        float totalWeight = 0.0f;
        for (const auto& pair : keyframes) {
            totalWeight += pair.second;
        }
        
        if (totalWeight <= 0.0f) {
            return result;
        }
        
        // Normalize weights
        std::vector<float> normalizedWeights;
        normalizedWeights.reserve(keyframes.size());
        for (const auto& pair : keyframes) {
            normalizedWeights.push_back(pair.second / totalWeight);
        }
        
        // Blend position and scale (linear interpolation)
        for (int i = 0; i < 3; ++i) {
            result.position[i] = 0.0f;
            result.scale[i] = 0.0f;
            
            for (size_t j = 0; j < keyframes.size(); ++j) {
                result.position[i] += keyframes[j].first.position[i] * normalizedWeights[j];
                result.scale[i] += keyframes[j].first.scale[i] * normalizedWeights[j];
            }
        }
        
        // Blend rotation (simple weighted average with normalization)
        for (int i = 0; i < 4; ++i) {
            result.rotation[i] = 0.0f;
            
            for (size_t j = 0; j < keyframes.size(); ++j) {
                result.rotation[i] += keyframes[j].first.rotation[i] * normalizedWeights[j];
            }
        }
        
        // Normalize the resulting quaternion
        float length = std::sqrt(result.rotation[0]*result.rotation[0] + 
                                result.rotation[1]*result.rotation[1] + 
                                result.rotation[2]*result.rotation[2] + 
                                result.rotation[3]*result.rotation[3]);
        if (length > 0.0f) {
            for (int i = 0; i < 4; ++i) {
                result.rotation[i] /= length;
            }
        }
        
        return result;
    }
}