#pragma once

#include "Component.h"
#include "Animation.h"
#include <unordered_map>
#include <memory>

namespace Sparky {
    
    class AnimationComponent : public Component {
    public:
        AnimationComponent();
        virtual ~AnimationComponent();
        
        void update(float deltaTime) override;
        void render() override;
        
        // Animation management
        void addAnimation(std::unique_ptr<Animation> animation);
        bool hasAnimation(const std::string& name) const;
        Animation* getAnimation(const std::string& name) const;
        void playAnimation(const std::string& name, bool loop = true);
        void stopAnimation();
        void pauseAnimation();
        
        // Animation blending
        void setBlendWeight(const std::string& animationName, float weight);
        void crossfade(const std::string& fromAnimation, const std::string& toAnimation, float duration);
        BlendedKeyframe getBlendedKeyframe(const std::string& trackName, float time) const;
        
        // Bone manipulation (for skeletal animation)
        void setBoneTransform(const std::string& boneName, float transform[16]); // 4x4 matrix as array
        void getBoneTransform(const std::string& boneName, float transform[16]) const; // 4x4 matrix as array
        
        // Getters
        Animation* getCurrentAnimation() const { return currentAnimation; }
        bool isPlaying() const { return currentAnimation && currentAnimation->isPlaying(); }
        
        // Animation blending manager
        AnimationBlender* getAnimationBlender() { return &blender; }

    private:
        std::unordered_map<std::string, std::unique_ptr<Animation>> animations;
        Animation* currentAnimation;
        std::unordered_map<std::string, float[16]> boneTransforms; // 4x4 matrices stored as arrays
        AnimationBlender blender;
    };
}