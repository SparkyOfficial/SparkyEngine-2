#include "../include/AnimationController.h"
#include "../include/Animation.h"
#include "../include/GameObject.h"
#include "../include/Logger.h"
#include <algorithm>

namespace Sparky {

    AnimationController::AnimationController() : 
        currentAnimation(nullptr),
        currentTime(0.0f),
        playbackSpeed(1.0f),
        playing(false),
        looping(true) {
    }

    AnimationController::~AnimationController() {
    }

    void AnimationController::update(float deltaTime) {
        if (!playing || !currentAnimation || !owner) {
            return;
        }
        
        // Update current time
        currentTime += deltaTime * playbackSpeed;
        
        // Handle animation end
        if (currentAnimation->getDuration() > 0.0f && 
            currentTime >= currentAnimation->getDuration()) {
            if (looping) {
                // Wrap around for looping
                currentTime = fmod(currentTime, currentAnimation->getDuration());
            } else {
                // Stop at the end for non-looping
                currentTime = currentAnimation->getDuration();
                playing = false;
            }
        }
        
        // Evaluate animation and apply to owner
        glm::vec3 position, rotation, scale;
        currentAnimation->evaluate(currentTime, position, rotation, scale);
        
        owner->setPosition(position);
        owner->setRotation(rotation);
        owner->setScale(scale);
    }

    void AnimationController::render() {
        // Animation controller doesn't need rendering
    }

    void AnimationController::addAnimation(std::unique_ptr<Animation> animation) {
        if (!animation) return;
        
        // Check if animation with this name already exists
        const std::string& name = animation->getName();
        auto it = std::find_if(animations.begin(), animations.end(),
            [&name](const std::unique_ptr<Animation>& anim) {
                return anim->getName() == name;
            });
        
        if (it != animations.end()) {
            // Replace existing animation
            *it = std::move(animation);
        } else {
            // Add new animation
            animations.push_back(std::move(animation));
        }
    }

    bool AnimationController::setAnimation(const std::string& name) {
        auto it = std::find_if(animations.begin(), animations.end(),
            [&name](const std::unique_ptr<Animation>& anim) {
                return anim->getName() == name;
            });
        
        if (it != animations.end()) {
            currentAnimation = it->get();
            currentAnimationName = name;
            currentTime = 0.0f;
            return true;
        }
        
        return false;
    }

    void AnimationController::setPlaybackSpeed(float speed) {
        playbackSpeed = speed;
    }

    void AnimationController::setLooping(bool looping) {
        this->looping = looping;
    }

    void AnimationController::play() {
        playing = true;
    }

    void AnimationController::pause() {
        playing = false;
    }

    void AnimationController::stop() {
        playing = false;
        currentTime = 0.0f;
    }

    void AnimationController::reset() {
        currentTime = 0.0f;
    }
}