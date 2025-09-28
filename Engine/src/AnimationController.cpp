#include "../include/AnimationController.h"
#include "../include/Logger.h"
#include <algorithm>

namespace Sparky {

    AnimationController::AnimationController() : isPlaying(false) {
        SPARKY_LOG_DEBUG("Creating AnimationController");
    }

    AnimationController::~AnimationController() {
        SPARKY_LOG_DEBUG("Destroying AnimationController");
    }

    void AnimationController::addAnimation(const std::string& name, std::unique_ptr<Animation> animation) {
        animations[name] = std::move(animation);
        SPARKY_LOG_DEBUG("Added animation: " + name);
    }

    void AnimationController::playAnimation(const std::string& name) {
        auto it = animations.find(name);
        if (it != animations.end()) {
            // Stop current animations
            for (auto& state : activeStates) {
                state.animation->stop();
            }
            activeStates.clear();

            // Play new animation
            AnimationState state;
            state.animation = it->second.get();
            state.animation->play();
            state.blendWeight = 1.0f;
            activeStates.push_back(state);

            isPlaying = true;
            SPARKY_LOG_DEBUG("Playing animation: " + name);
        } else {
            SPARKY_LOG_WARNING("Animation not found: " + name);
        }
    }

    void AnimationController::blendAnimation(const std::string& name, float blendWeight) {
        auto it = animations.find(name);
        if (it != animations.end()) {
            // Check if animation is already active
            auto activeIt = std::find_if(activeStates.begin(), activeStates.end(),
                [&it](const AnimationState& state) {
                    return state.animation == it->second.get();
                });

            if (activeIt != activeStates.end()) {
                // Update blend weight
                activeIt->blendWeight = blendWeight;
            } else {
                // Add new animation state
                AnimationState state;
                state.animation = it->second.get();
                state.animation->play();
                state.blendWeight = blendWeight;
                activeStates.push_back(state);
            }

            isPlaying = true;
            SPARKY_LOG_DEBUG("Blending animation: " + name + " with weight: " + std::to_string(blendWeight));
        } else {
            SPARKY_LOG_WARNING("Animation not found: " + name);
        }
    }

    void AnimationController::stopAnimation() {
        for (auto& state : activeStates) {
            state.animation->stop();
        }
        activeStates.clear();
        isPlaying = false;
        SPARKY_LOG_DEBUG("Stopped all animations");
    }

    void AnimationController::update(float deltaTime) {
        if (!isPlaying) return;

        // Update all active animations
        for (auto it = activeStates.begin(); it != activeStates.end();) {
            it->animation->update(deltaTime);
            
            // Remove finished animations
            if (it->animation->isFinished()) {
                it = activeStates.erase(it);
            } else {
                ++it;
            }
        }

        // If no active animations, stop playing
        if (activeStates.empty()) {
            isPlaying = false;
        }
    }

    bool AnimationController::getIsPlaying() const {
        return isPlaying;
    }

    Animation* AnimationController::getAnimation(const std::string& name) {
        auto it = animations.find(name);
        if (it != animations.end()) {
            return it->second.get();
        }
        return nullptr;
    }
}