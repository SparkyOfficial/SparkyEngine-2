#include "../include/AnimationComponent.h"
#include "../include/GameObject.h"
#include "../include/Logger.h"

namespace Sparky {
    
    AnimationComponent::AnimationComponent() : Component(), currentAnimation(nullptr) {
        SPARKY_LOG_DEBUG("AnimationComponent created");
    }
    
    AnimationComponent::~AnimationComponent() {
        SPARKY_LOG_DEBUG("AnimationComponent destroyed");
    }
    
    void AnimationComponent::update(float deltaTime) {
        // Update all animations
        for (auto& pair : animations) {
            pair.second->update(deltaTime);
        }
        
        // Update the animation blender
        blender.update(deltaTime);
        
        // Apply animation to the owner object
        if (owner && currentAnimation && currentAnimation->isPlaying()) {
            // For now, we'll just apply the first track's transform to the owner
            // In a full skeletal animation system, we would apply all bone transforms
            if (currentAnimation->getTrackCount() > 0) {
                AnimationTrack* track = currentAnimation->getTrack(0);
                if (track) {
                    Keyframe keyframe = track->getKeyframeAtTime(currentAnimation->getCurrentTime());
                    // In a full implementation, we would set the position using glm
                    // For now, we'll just log that we're applying the animation
                    SPARKY_LOG_DEBUG("Applying animation frame to owner object");
                }
            }
        }
    }
    
    void AnimationComponent::render() {
        // Animation components don't render anything directly
    }
    
    void AnimationComponent::addAnimation(std::unique_ptr<Animation> animation) {
        if (animation) {
            std::string name = animation->getName();
            animations[name] = std::move(animation);
            blender.addAnimation(animations[name].get());
            SPARKY_LOG_DEBUG("Animation added: " + name);
        }
    }
    
    bool AnimationComponent::hasAnimation(const std::string& name) const {
        return animations.find(name) != animations.end();
    }
    
    Animation* AnimationComponent::getAnimation(const std::string& name) const {
        auto it = animations.find(name);
        if (it != animations.end()) {
            return it->second.get();
        }
        return nullptr;
    }
    
    void AnimationComponent::playAnimation(const std::string& name, bool loop) {
        Animation* animation = getAnimation(name);
        if (animation) {
            currentAnimation = animation;
            currentAnimation->play(loop);
            SPARKY_LOG_DEBUG("Playing animation: " + name);
        } else {
            SPARKY_LOG_WARNING("Animation not found: " + name);
        }
    }
    
    void AnimationComponent::stopAnimation() {
        if (currentAnimation) {
            currentAnimation->stop();
            currentAnimation = nullptr;
            SPARKY_LOG_DEBUG("Animation stopped");
        }
    }
    
    void AnimationComponent::pauseAnimation() {
        if (currentAnimation) {
            currentAnimation->pause();
            SPARKY_LOG_DEBUG("Animation paused");
        }
    }
    
    void AnimationComponent::setBlendWeight(const std::string& animationName, float weight) {
        blender.setBlendWeight(animationName, weight);
    }
    
    void AnimationComponent::crossfade(const std::string& fromAnimation, const std::string& toAnimation, float duration) {
        blender.crossfade(fromAnimation, toAnimation, duration);
    }
    
    BlendedKeyframe AnimationComponent::getBlendedKeyframe(const std::string& trackName, float time) const {
        return blender.getBlendedKeyframe(trackName, time);
    }
    
    void AnimationComponent::setBoneTransform(const std::string& boneName, float transform[16]) {
        // Copy the transform array
        for (int i = 0; i < 16; ++i) {
            boneTransforms[boneName][i] = transform[i];
        }
        SPARKY_LOG_DEBUG("Setting bone transform for " + boneName);
    }
    
    void AnimationComponent::getBoneTransform(const std::string& boneName, float transform[16]) const {
        auto it = boneTransforms.find(boneName);
        if (it != boneTransforms.end()) {
            // Copy the transform array
            for (int i = 0; i < 16; ++i) {
                transform[i] = it->second[i];
            }
        } else {
            // Return identity matrix
            for (int i = 0; i < 16; ++i) {
                transform[i] = (i % 5 == 0) ? 1.0f : 0.0f; // Identity matrix
            }
        }
        SPARKY_LOG_DEBUG("Getting bone transform for " + boneName);
    }
}