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
        // Update the current animation
        if (currentAnimation) {
            currentAnimation->update(deltaTime);
            
            // Apply animation to the owner object
            if (owner) {
                // For now, we'll just apply the first track's transform to the owner
                // In a full skeletal animation system, we would apply all bone transforms
                if (currentAnimation->getTrackCount() > 0) {
                    AnimationTrack* track = currentAnimation->getTrack(0);
                    if (track) {
                        Keyframe keyframe = track->getKeyframeAtTime(currentAnimation->getCurrentTime());
                        owner->setPosition(keyframe.position);
                        // For rotation, we would convert the quaternion to Euler angles
                        // owner->setRotation(glm::eulerAngles(keyframe.rotation));
                        owner->setScale(keyframe.scale);
                    }
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
    
    void AnimationComponent::setBoneTransform(const std::string& boneName, const glm::mat4& transform) {
        boneTransforms[boneName] = transform;
    }
    
    glm::mat4 AnimationComponent::getBoneTransform(const std::string& boneName) const {
        auto it = boneTransforms.find(boneName);
        if (it != boneTransforms.end()) {
            return it->second;
        }
        return glm::mat4(1.0f); // Return identity matrix if not found
    }
}