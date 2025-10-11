#pragma once

#include "AnimationComponent.h"
#include <string>
#include <unordered_map>
#include <memory>
#include <functional>
#include <vector>

namespace Sparky {
    
    // Forward declaration
    class AnimationComponent;
    
    // Animation state
    struct AnimationState {
        std::string name;
        std::string animationName;
        float playbackSpeed;
        bool looping;
    };
    
    // Animation transition
    struct AnimationTransition {
        std::string fromState;
        std::string toState;
        float transitionTime;
        std::function<bool()> condition; // Condition for transition
    };
    
    class AnimationController {
    public:
        AnimationController(AnimationComponent* animationComponent);
        ~AnimationController();
        
        // State management
        void addState(const AnimationState& state);
        void addTransition(const AnimationTransition& transition);
        void setState(const std::string& stateName);
        const std::string& getCurrentState() const { return currentState; }
        
        // Update
        void update(float deltaTime);
        
        // Getters
        AnimationComponent* getAnimationComponent() const { return animationComponent; }
        
        // Animation blending
        void setBlendWeight(const std::string& animationName, float weight);
        void crossfade(const std::string& fromState, const std::string& toState, float duration);

    private:
        AnimationComponent* animationComponent;
        std::unordered_map<std::string, AnimationState> states;
        std::vector<AnimationTransition> transitions;
        std::string currentState;
        float stateTime;
        std::string targetState;
        float transitionTime;
        float currentTransitionTime;
        
        // Private helper methods
        void applyState(const AnimationState& state);
    };
}