#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <vector>

namespace Sparky {
    class Animation;
    class GameObject;

    enum class AnimationBlendMode {
        OVERRIDE,
        ADDITIVE,
        MIX
    };

    struct AnimationState {
        std::string name;
        Animation* animation;
        float time;
        float speed;
        float weight;
        bool looping;
        bool playing;
    };

    class AnimationController {
    public:
        AnimationController(GameObject* owner);
        ~AnimationController();

        // Animation management
        void addAnimation(std::unique_ptr<Animation> animation);
        void removeAnimation(const std::string& name);
        Animation* getAnimation(const std::string& name);

        // State management
        void playAnimation(const std::string& name, bool loop = true, float speed = 1.0f, float weight = 1.0f);
        void stopAnimation(const std::string& name);
        void stopAllAnimations();
        void setAnimationSpeed(const std::string& name, float speed);
        void setAnimationWeight(const std::string& name, float weight);

        // Blending
        void setBlendMode(AnimationBlendMode mode);
        AnimationBlendMode getBlendMode() const { return blendMode; }

        // Update
        void update(float deltaTime);

        // Get current transform
        void getCurrentTransform(glm::vec3& position, glm::vec3& rotation, glm::vec3& scale);

    private:
        GameObject* owner;
        std::unordered_map<std::string, std::unique_ptr<Animation>> animations;
        std::vector<AnimationState> activeStates;
        AnimationBlendMode blendMode;

        // Helper methods
        AnimationState* findState(const std::string& name);
    };
}