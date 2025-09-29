#pragma once

#include "Component.h"
#include <vector>
#include <string>
#include <memory>
#include <glm/glm.hpp>

namespace Sparky {
    class Animation;

    class AnimationController : public Component {
    public:
        AnimationController();
        virtual ~AnimationController();

        void update(float deltaTime) override;
        void render() override;

        // Animation management
        void addAnimation(std::unique_ptr<Animation> animation);
        bool setAnimation(const std::string& name);
        void setPlaybackSpeed(float speed);
        void setLooping(bool looping);
        
        // Playback control
        void play();
        void pause();
        void stop();
        void reset();

        // Getters
        bool isPlaying() const { return playing; }
        float getCurrentTime() const { return currentTime; }
        const std::string& getCurrentAnimationName() const { return currentAnimationName; }

    private:
        std::vector<std::unique_ptr<Animation>> animations;
        std::string currentAnimationName;
        Animation* currentAnimation;
        
        float currentTime;
        float playbackSpeed;
        bool playing;
        bool looping;
    };
}