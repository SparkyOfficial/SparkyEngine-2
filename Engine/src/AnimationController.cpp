#include "../include/AnimationController.h"
#include "../include/Animation.h"
#include "../include/GameObject.h"
#include "../include/Logger.h"
#include <glm/glm.hpp>

namespace Sparky {

    AnimationController::AnimationController(GameObject* owner) : 
        owner(owner), blendMode(AnimationBlendMode::OVERRIDE) {
        SPARKY_LOG_DEBUG("Created animation controller");
    }

    AnimationController::~AnimationController() {
    }

    void AnimationController::addAnimation(std::unique_ptr<Animation> animation) {
        if (animation) {
            const std::string& name = animation->getName();
            animations[name] = std::move(animation);
            SPARKY_LOG_DEBUG("Added animation: " + name);
        }
    }

    void AnimationController::removeAnimation(const std::string& name) {
        auto it = animations.find(name);
        if (it != animations.end()) {
            // Also remove from active states
            activeStates.erase(
                std::remove_if(activeStates.begin(), activeStates.end(),
                    [&name](const AnimationState& state) { return state.name == name; }),
                activeStates.end()
            );
            
            animations.erase(it);
            SPARKY_LOG_DEBUG("Removed animation: " + name);
        }
    }

    Animation* AnimationController::getAnimation(const std::string& name) {
        auto it = animations.find(name);
        if (it != animations.end()) {
            return it->second.get();
        }
        return nullptr;
    }

    void AnimationController::playAnimation(const std::string& name, bool loop, float speed, float weight) {
        Animation* animation = getAnimation(name);
        if (!animation) {
            SPARKY_LOG_WARNING("Animation not found: " + name);
            return;
        }

        // Check if already playing
        AnimationState* state = findState(name);
        if (state) {
            // Update existing state
            state->time = 0.0f;
            state->speed = speed;
            state->weight = weight;
            state->looping = loop;
            state->playing = true;
        } else {
            // Create new state
            AnimationState newState;
            newState.name = name;
            newState.animation = animation;
            newState.time = 0.0f;
            newState.speed = speed;
            newState.weight = weight;
            newState.looping = loop;
            newState.playing = true;
            
            activeStates.push_back(newState);
        }
        
        SPARKY_LOG_DEBUG("Playing animation: " + name);
    }

    void AnimationController::stopAnimation(const std::string& name) {
        AnimationState* state = findState(name);
        if (state) {
            state->playing = false;
            SPARKY_LOG_DEBUG("Stopped animation: " + name);
        }
    }

    void AnimationController::stopAllAnimations() {
        for (auto& state : activeStates) {
            state.playing = false;
        }
        SPARKY_LOG_DEBUG("Stopped all animations");
    }

    void AnimationController::setAnimationSpeed(const std::string& name, float speed) {
        AnimationState* state = findState(name);
        if (state) {
            state->speed = speed;
            SPARKY_LOG_DEBUG("Set animation speed: " + name + " to " + std::to_string(speed));
        }
    }

    void AnimationController::setAnimationWeight(const std::string& name, float weight) {
        AnimationState* state = findState(name);
        if (state) {
            state->weight = weight;
            SPARKY_LOG_DEBUG("Set animation weight: " + name + " to " + std::to_string(weight));
        }
    }

    void AnimationController::setBlendMode(AnimationBlendMode mode) {
        blendMode = mode;
        SPARKY_LOG_DEBUG("Set animation blend mode");
    }

    void AnimationController::update(float deltaTime) {
        // Update active animations
        for (auto& state : activeStates) {
            if (state.playing && state.animation) {
                state.time += deltaTime * state.speed;
                
                // Handle looping
                if (state.time > state.animation->getDuration()) {
                    if (state.looping) {
                        state.time = fmod(state.time, state.animation->getDuration());
                    } else {
                        state.playing = false;
                    }
                }
            }
        }

        // Remove finished non-looping animations
        activeStates.erase(
            std::remove_if(activeStates.begin(), activeStates.end(),
                [](const AnimationState& state) { return !state.playing && !state.looping; }),
            activeStates.end()
        );

        SPARKY_LOG_DEBUG("Updated animation controller with " + std::to_string(activeStates.size()) + " active animations");
    }

    void AnimationController::getCurrentTransform(glm::vec3& position, glm::vec3& rotation, glm::vec3& scale) {
        if (activeStates.empty()) {
            // Return default transform
            position = glm::vec3(0.0f);
            rotation = glm::vec3(0.0f);
            scale = glm::vec3(1.0f);
            return;
        }

        // Handle different blend modes
        switch (blendMode) {
            case AnimationBlendMode::OVERRIDE:
                {
                    // Use the first active animation
                    for (const auto& state : activeStates) {
                        if (state.playing && state.animation) {
                            state.animation->evaluate(state.time, position, rotation, scale);
                            return;
                        }
                    }
                    // Fallback to default
                    position = glm::vec3(0.0f);
                    rotation = glm::vec3(0.0f);
                    scale = glm::vec3(1.0f);
                }
                break;

            case AnimationBlendMode::ADDITIVE:
                {
                    // Add all active animations
                    position = glm::vec3(0.0f);
                    rotation = glm::vec3(0.0f);
                    scale = glm::vec3(0.0f);
                    
                    float totalWeight = 0.0f;
                    for (const auto& state : activeStates) {
                        if (state.playing && state.animation) {
                            glm::vec3 pos, rot, scl;
                            state.animation->evaluate(state.time, pos, rot, scl);
                            
                            position += pos * state.weight;
                            rotation += rot * state.weight;
                            scale += scl * state.weight;
                            totalWeight += state.weight;
                        }
                    }
                    
                    if (totalWeight > 0.0f) {
                        position /= totalWeight;
                        rotation /= totalWeight;
                        scale /= totalWeight;
                    }
                }
                break;

            case AnimationBlendMode::MIX:
                {
                    // Mix animations based on weights
                    position = glm::vec3(0.0f);
                    rotation = glm::vec3(0.0f);
                    scale = glm::vec3(0.0f);
                    
                    float totalWeight = 0.0f;
                    for (const auto& state : activeStates) {
                        if (state.playing && state.animation) {
                            glm::vec3 pos, rot, scl;
                            state.animation->evaluate(state.time, pos, rot, scl);
                            
                            position += pos * state.weight;
                            rotation += rot * state.weight;
                            scale += (scl - glm::vec3(1.0f)) * state.weight + glm::vec3(1.0f);
                            totalWeight += state.weight;
                        }
                    }
                    
                    if (totalWeight > 0.0f) {
                        position /= totalWeight;
                        rotation /= totalWeight;
                    }
                }
                break;
        }
    }

    AnimationState* AnimationController::findState(const std::string& name) {
        for (auto& state : activeStates) {
            if (state.name == name) {
                return &state;
            }
        }
        return nullptr;
    }
}