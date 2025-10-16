#pragma once

#include "Animation.h"
#include "Component.h"
#include "GameObject.h"
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include <functional>

namespace Sparky {
    // Forward declarations
    class SkeletalAnimation;
    class AnimationController;
    class InverseKinematics;
    
    // Animation blend modes
    enum class BlendMode {
        LINEAR,
        EASE_IN,
        EASE_OUT,
        EASE_IN_OUT,
        OVERWRITE
    };
    
    // Animation events
    struct AnimationEvent {
        float time;
        std::string name;
        std::function<void()> callback;
    };
    
    // Use Keyframe structure from Animation.h
    
    // Animation clip
    class AnimationClip {
    public:
        AnimationClip(const std::string& name);
        ~AnimationClip() = default;
        
        // Clip properties
        const std::string& getName() const { return m_name; }
        float getDuration() const { return m_duration; }
        
        // Keyframe management
        void addKeyframe(int boneId, const Keyframe& keyframe);
        const std::vector<Keyframe>& getKeyframes(int boneId) const;
        
        // Animation evaluation
        void evaluate(float time, std::unordered_map<int, glm::mat4>& boneTransforms) const;
        
        // Events
        void addEvent(const AnimationEvent& event);
        const std::vector<AnimationEvent>& getEvents() const { return m_events; }
        
    private:
        std::string m_name;
        float m_duration;
        std::unordered_map<int, std::vector<Keyframe>> m_keyframes;
        std::vector<AnimationEvent> m_events;
        
        // Helper methods
        Keyframe interpolateKeyframes(const Keyframe& a, const Keyframe& b, float t) const;
    };
    
    // Skeletal animation system
    class SkeletalAnimation : public Component {
    public:
        SkeletalAnimation();
        virtual ~SkeletalAnimation() = default;
        
        // Component interface
        virtual void initialize();
        virtual void update(float deltaTime) override;
        virtual void destroy();
        virtual void render() override;
        
        // Skeleton structure
        struct Bone {
            std::string name;
            int id;
            int parentId;
            glm::mat4 offsetMatrix;
            glm::mat4 finalTransformation;
        };
        
        // Bone management
        void addBone(const Bone& bone);
        Bone* getBone(int id);
        Bone* getBone(const std::string& name);
        const std::vector<Bone>& getBones() const { return m_bones; }
        
        // Animation clips
        void addAnimationClip(std::unique_ptr<AnimationClip> clip);
        AnimationClip* getAnimationClip(const std::string& name) const;
        
        // Animation playback
        void playAnimation(const std::string& name, bool loop = true, float blendTime = 0.2f);
        void stopAnimation();
        void pauseAnimation();
        void resumeAnimation();
        
        // Blending
        void blendToAnimation(const std::string& name, float blendTime = 0.2f, BlendMode mode = BlendMode::LINEAR);
        
        // Animation state
        bool isPlaying() const { return m_isPlaying; }
        bool isPaused() const { return m_isPaused; }
        const std::string& getCurrentAnimation() const { return m_currentAnimation; }
        float getCurrentTime() const { return m_currentTime; }
        
        // Bone transformations
        const glm::mat4& getBoneTransform(int boneId) const;
        std::vector<glm::mat4> getBoneTransforms() const;
        
        // Events
        void setAnimationEventCallback(const std::string& eventName, std::function<void()> callback);
        
    private:
        // Skeleton
        std::vector<Bone> m_bones;
        std::unordered_map<std::string, int> m_boneNameToId;
        
        // Animation clips
        std::vector<std::unique_ptr<AnimationClip>> m_animationClips;
        std::unordered_map<std::string, AnimationClip*> m_clipNameToPtr;
        
        // Playback state
        std::string m_currentAnimation;
        float m_currentTime;
        bool m_isPlaying;
        bool m_isPaused;
        bool m_isLooping;
        
        // Blending
        std::string m_blendingToAnimation;
        float m_blendTime;
        float m_blendProgress;
        BlendMode m_blendMode;
        bool m_isBlending;
        
        // Events
        std::unordered_map<std::string, std::function<void()>> m_eventCallbacks;
        
        // Internal methods
        void updateAnimation(float deltaTime);
        void updateBlending(float deltaTime);
        glm::mat4 calculateBoneTransform(int boneId, float time, AnimationClip* clip) const;
        float getBlendFactor(float progress, BlendMode mode) const;
    };
    
    // Inverse Kinematics system
    class InverseKinematics : public Component {
    public:
        InverseKinematics();
        virtual ~InverseKinematics() = default;
        
        // Component interface
        virtual void initialize();
        virtual void update(float deltaTime) override;
        virtual void destroy();
        virtual void render() override;
        
        // IK chain
        struct IKJoint {
            int boneId;
            float minAngle;
            float maxAngle;
            glm::vec3 axis;
        };
        
        struct IKChain {
            std::vector<IKJoint> joints;
            int effectorBoneId;
            int targetBoneId;
            int poleBoneId;
        };
        
        // Chain management
        void addIKChain(const IKChain& chain);
        void removeIKChain(int chainId);
        IKChain* getIKChain(int chainId);
        
        // IK solving
        void setTargetPosition(int chainId, const glm::vec3& target);
        void setPolePosition(int chainId, const glm::vec3& pole);
        void solveIK(int chainId);
        
        // Constraints
        void setJointConstraints(int chainId, int jointIndex, float minAngle, float maxAngle);
        
    private:
        std::vector<IKChain> m_chains;
        std::unordered_map<int, glm::vec3> m_targets;
        std::unordered_map<int, glm::vec3> m_poles;
        
        // Skeletal animation reference
        SkeletalAnimation* m_skeletalAnimation;
        
        // Internal methods
        void solveCCD(int chainId);
        glm::vec3 getBonePosition(int boneId) const;
        void setBoneRotation(int boneId, const glm::quat& rotation);
    };
    
    // Advanced animation controller with state machine
    class AdvancedAnimationController : public Component {
    public:
        AdvancedAnimationController();
        virtual ~AdvancedAnimationController() = default;
        
        // Component interface
        virtual void initialize();
        virtual void update(float deltaTime) override;
        virtual void destroy();
        virtual void render() override;
        
        // Animation states
        struct AnimationState {
            std::string name;
            std::string animationClip;
            bool loop;
            float speed;
        };
        
        // Transitions
        struct AnimationTransition {
            std::string fromState;
            std::string toState;
            float transitionTime;
            std::function<bool()> condition;
        };
        
        // State management
        void addState(const AnimationState& state);
        void addTransition(const AnimationTransition& transition);
        void setState(const std::string& stateName);
        const std::string& getCurrentState() const { return m_currentState; }
        
        // Parameters
        void setFloatParameter(const std::string& name, float value);
        void setBoolParameter(const std::string& name, bool value);
        void setIntParameter(const std::string& name, int value);
        
        float getFloatParameter(const std::string& name) const;
        bool getBoolParameter(const std::string& name) const;
        int getIntParameter(const std::string& name) const;
        
        // Blend trees
        struct BlendTreeNode {
            enum class NodeType {
                CLIP,
                BLEND_1D,
                BLEND_2D
            };
            
            NodeType type;
            std::string clipName;
            std::vector<BlendTreeNode> children;
            std::string parameterX;
            std::string parameterY;
            glm::vec2 position;
        };
        
        void setBlendTree(const std::string& name, const BlendTreeNode& root);
        void useBlendTree(const std::string& name);
        
    private:
        // State machine
        std::vector<AnimationState> m_states;
        std::vector<AnimationTransition> m_transitions;
        std::string m_currentState;
        std::string m_previousState;
        std::string m_blendingToAnimation;
        float m_transitionTimer;
        float m_transitionDuration;
        bool m_isTransitioning;
        
        // Parameters
        std::unordered_map<std::string, float> m_floatParameters;
        std::unordered_map<std::string, bool> m_boolParameters;
        std::unordered_map<std::string, int> m_intParameters;
        
        // Blend trees
        std::unordered_map<std::string, BlendTreeNode> m_blendTrees;
        std::string m_currentBlendTree;
        
        // Skeletal animation reference
        SkeletalAnimation* m_skeletalAnimation;
        
        // Internal methods
        void updateState(float deltaTime);
        void updateTransitions(float deltaTime);
        void updateBlendTree(float deltaTime);
        AnimationState* getState(const std::string& name);
        bool canTransition(const std::string& from, const std::string& to) const;
        void startTransition(const std::string& toState, float duration);
    };
}