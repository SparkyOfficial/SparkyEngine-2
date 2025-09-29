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
        
        // Bone manipulation (for skeletal animation)
        void setBoneTransform(const std::string& boneName, const glm::mat4& transform);
        glm::mat4 getBoneTransform(const std::string& boneName) const;
        
        // Getters
        Animation* getCurrentAnimation() const { return currentAnimation; }
        bool isPlaying() const { return currentAnimation && currentAnimation->isPlaying(); }

    private:
        std::unordered_map<std::string, std::unique_ptr<Animation>> animations;
        Animation* currentAnimation;
        std::unordered_map<std::string, glm::mat4> boneTransforms;
    };
}