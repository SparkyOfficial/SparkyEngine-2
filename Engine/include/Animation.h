#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace Sparky {
    
    // Keyframe structure
    struct Keyframe {
        float time;           // Time in seconds
        float position[3];    // Position at this keyframe (x, y, z)
        float rotation[4];    // Rotation at this keyframe (x, y, z, w)
        float scale[3];       // Scale at this keyframe (x, y, z)
    };
    
    // Animation track for a single bone or object
    class AnimationTrack {
    public:
        AnimationTrack(const std::string& name);
        ~AnimationTrack();
        
        void addKeyframe(const Keyframe& keyframe);
        Keyframe getKeyframeAtTime(float time) const;
        Keyframe interpolateKeyframes(const Keyframe& a, const Keyframe& b, float t) const;
        
        const std::string& getName() const { return name; }
        size_t getKeyframeCount() const { return keyframes.size(); }
        const std::vector<Keyframe>& getKeyframes() const { return keyframes; }
        
        float getDuration() const;

    private:
        std::string name;
        std::vector<Keyframe> keyframes;
    };
    
    // Blending information for animation mixing
    struct AnimationBlendInfo {
        std::string animationName;
        float weight;
        float timeOffset;
        
        AnimationBlendInfo(const std::string& name, float w, float offset = 0.0f) 
            : animationName(name), weight(w), timeOffset(offset) {}
    };
    
    // Animation class
    class Animation {
    public:
        Animation(const std::string& name);
        ~Animation();
        
        void addTrack(std::unique_ptr<AnimationTrack> track);
        AnimationTrack* getTrack(const std::string& name) const;
        AnimationTrack* getTrack(size_t index) const;
        
        const std::string& getName() const { return name; }
        size_t getTrackCount() const { return tracks.size(); }
        float getDuration() const;
        
        // Animation playback methods
        void update(float deltaTime);
        void play(bool loop = true);
        void pause();
        void stop();
        void setPlaybackSpeed(float speed) { playbackSpeed = speed; }
        void setCurrentTime(float time) { currentTime = time; }
        
        // Blending methods
        void setBlendWeight(float weight) { blendWeight = weight; }
        float getBlendWeight() const { return blendWeight; }
        
        bool isPlaying() const { return playing; }
        float getCurrentTime() const { return currentTime; }
        float getPlaybackSpeed() const { return playbackSpeed; }
        
        // Cross-fade methods
        void startCrossfade(float fadeDuration);
        bool isCrossfading() const { return crossfadeTime >= 0.0f; }
        float getCrossfadeProgress() const { return crossfadeTime >= 0.0f ? crossfadeTime / crossfadeDuration : 0.0f; }

    private:
        std::string name;
        std::vector<std::unique_ptr<AnimationTrack>> tracks;
        bool playing;
        bool looping;
        float currentTime;
        float playbackSpeed;
        float blendWeight;
        
        // Cross-fade properties
        float crossfadeTime;
        float crossfadeDuration;
        bool crossfadeOut;
    };
    
    // Animation blending result
    struct BlendedKeyframe {
        float position[3];
        float rotation[4];
        float scale[3];
        
        BlendedKeyframe();
    };
    
    // Animation blending manager
    class AnimationBlender {
    public:
        AnimationBlender();
        ~AnimationBlender();
        
        // Add animation to blend
        void addAnimation(Animation* animation);
        
        // Set blend weights
        void setBlendWeight(const std::string& animationName, float weight);
        void setBlendWeights(const std::vector<AnimationBlendInfo>& blendInfos);
        
        // Get blended keyframe at time
        BlendedKeyframe getBlendedKeyframe(const std::string& trackName, float time) const;
        
        // Update blending
        void update(float deltaTime);
        
        // Cross-fade between animations
        void crossfade(const std::string& fromAnimation, const std::string& toAnimation, float duration);
        
        // Getters
        size_t getAnimationCount() const { return animations.size(); }
        Animation* getAnimation(const std::string& name) const;
        
    private:
        std::unordered_map<std::string, Animation*> animations;
        std::vector<AnimationBlendInfo> blendInfos;
        std::string currentAnimation;
        std::string targetAnimation;
        float crossfadeProgress;
        float crossfadeDuration;
        
        // Private helper methods
        BlendedKeyframe blendKeyframes(const std::vector<std::pair<Keyframe, float>>& keyframes) const;
    };
}