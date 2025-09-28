#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include "Animation.h"

namespace Sparky {

    struct AnimationState {
        Animation* animation;
        float blendWeight;
    };

    class AnimationController {
    private:
        std::unordered_map<std::string, std::unique_ptr<Animation>> animations;
        std::vector<AnimationState> activeStates;
        bool isPlaying;

    public:
        AnimationController();
        ~AnimationController();

        void addAnimation(const std::string& name, std::unique_ptr<Animation> animation);
        void playAnimation(const std::string& name);
        void blendAnimation(const std::string& name, float blendWeight);
        void stopAnimation();
        void update(float deltaTime);

        bool getIsPlaying() const;
        Animation* getAnimation(const std::string& name);
    };

}