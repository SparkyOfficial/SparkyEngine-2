#include "../include/AnimationController.h"
#include "../include/Logger.h"

namespace Sparky {
    
    AnimationController::AnimationController(AnimationComponent* animationComponent) 
        : animationComponent(animationComponent), stateTime(0.0f), 
          transitionTime(0.0f), currentTransitionTime(0.0f) {
        SPARKY_LOG_DEBUG("AnimationController created");
    }
    
    AnimationController::~AnimationController() {
        SPARKY_LOG_DEBUG("AnimationController destroyed");
    }
    
    void AnimationController::addState(const AnimationState& state) {
        states[state.name] = state;
        SPARKY_LOG_DEBUG("Animation state added: " + state.name);
    }
    
    void AnimationController::addTransition(const AnimationTransition& transition) {
        transitions.push_back(transition);
        SPARKY_LOG_DEBUG("Animation transition added: " + transition.fromState + " -> " + transition.toState);
    }
    
    void AnimationController::setState(const std::string& stateName) {
        auto it = states.find(stateName);
        if (it != states.end()) {
            if (currentState != stateName) {
                // Check if there's a transition defined
                for (const auto& transition : transitions) {
                    if (transition.fromState == currentState && transition.toState == stateName) {
                        // Start transition
                        targetState = stateName;
                        transitionTime = transition.transitionTime;
                        currentTransitionTime = 0.0f;
                        SPARKY_LOG_DEBUG("Starting transition: " + currentState + " -> " + stateName);
                        return;
                    }
                }
                
                // No transition defined, switch immediately
                currentState = stateName;
                stateTime = 0.0f;
                applyState(it->second);
                SPARKY_LOG_DEBUG("State changed to: " + stateName);
            }
        } else {
            SPARKY_LOG_WARNING("Animation state not found: " + stateName);
        }
    }
    
    void AnimationController::update(float deltaTime) {
        stateTime += deltaTime;
        
        // Handle ongoing transitions
        if (!targetState.empty()) {
            currentTransitionTime += deltaTime;
            if (currentTransitionTime >= transitionTime) {
                // Transition complete
                currentState = targetState;
                targetState.clear();
                
                auto it = states.find(currentState);
                if (it != states.end()) {
                    applyState(it->second);
                }
                SPARKY_LOG_DEBUG("Transition complete to state: " + currentState);
            }
        }
        
        // Check for automatic transitions based on conditions
        if (targetState.empty()) { // Only check if not already transitioning
            for (const auto& transition : transitions) {
                if (transition.fromState == currentState && transition.condition) {
                    if (transition.condition()) {
                        setState(transition.toState);
                        break;
                    }
                }
            }
        }
    }
    
    void AnimationController::applyState(const AnimationState& state) {
        if (animationComponent) {
            animationComponent->playAnimation(state.animationName, state.looping);
            if (auto anim = animationComponent->getAnimation(state.animationName)) {
                anim->setPlaybackSpeed(state.playbackSpeed);
            }
        }
    }
    
    void AnimationController::setBlendWeight(const std::string& animationName, float weight) {
        if (animationComponent) {
            // In a full implementation, we would set the blend weight for the animation
            // For now, we'll just log the action
            SPARKY_LOG_DEBUG("Setting blend weight for animation " + animationName + " to " + std::to_string(weight));
        }
    }
    
    void AnimationController::crossfade(const std::string& fromState, const std::string& toState, float duration) {
        // In a full implementation, we would perform a crossfade between states
        // For now, we'll just log the action
        SPARKY_LOG_DEBUG("Crossfading from state " + fromState + " to " + toState + " over " + std::to_string(duration) + " seconds");
    }
}