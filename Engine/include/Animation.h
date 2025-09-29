#pragma once

#include <string>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Sparky {
    
    // Keyframe structure
    struct Keyframe {
        float time;           // Time in seconds
        glm::vec3 position;   // Position at this keyframe
        glm::quat rotation;   // Rotation at this keyframe
        glm::vec3 scale;      // Scale at this keyframe
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
        
        bool isPlaying() const { return playing; }
        float getCurrentTime() const { return currentTime; }
        float getPlaybackSpeed() const { return playbackSpeed; }

    private:
        std::string name;
        std::vector<std::unique_ptr<AnimationTrack>> tracks;
        bool playing;
        bool looping;
        float currentTime;
        float playbackSpeed;
    };
}