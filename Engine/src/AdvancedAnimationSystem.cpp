#include "../include/AdvancedAnimationSystem.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <algorithm>
#include <cmath>
#include <limits>

namespace Sparky {
    // AnimationClip implementation
    AnimationClip::AnimationClip(const std::string& name)
        : m_name(name)
        , m_duration(0.0f) {
    }
    
    void AnimationClip::addKeyframe(int boneId, const Keyframe& keyframe) {
        m_keyframes[boneId].push_back(keyframe);
        
        // Update duration if this keyframe is later
        if (keyframe.time > m_duration) {
            m_duration = keyframe.time;
        }
        
        // Keep keyframes sorted by time
        std::sort(m_keyframes[boneId].begin(), m_keyframes[boneId].end(),
            [](const Keyframe& a, const Keyframe& b) {
                return a.time < b.time;
            });
    }
    
    const std::vector<Keyframe>& AnimationClip::getKeyframes(int boneId) const {
        static std::vector<Keyframe> empty;
        auto it = m_keyframes.find(boneId);
        if (it != m_keyframes.end()) {
            return it->second;
        }
        return empty;
    }
    
    void AnimationClip::evaluate(float time, std::unordered_map<int, glm::mat4>& boneTransforms) const {
        for (const auto& pair : m_keyframes) {
            int boneId = pair.first;
            const std::vector<Keyframe>& keyframes = pair.second;
            
            if (keyframes.empty()) continue;
            
            // Handle time wrapping for looping animations
            float evalTime = time;
            if (m_duration > 0.0f) {
                evalTime = fmod(time, m_duration);
            }
            
            // Find the keyframes to interpolate between
            const Keyframe* prev = nullptr;
            const Keyframe* next = nullptr;
            
            for (const auto& keyframe : keyframes) {
                if (keyframe.time <= evalTime) {
                    prev = &keyframe;
                } else {
                    next = &keyframe;
                    break;
                }
            }
            
            // If we're past the last keyframe, use it
            if (!next && !keyframes.empty()) {
                next = &keyframes.back();
                prev = next;
            }
            
            // If we're before the first keyframe, use it
            if (!prev && !keyframes.empty()) {
                prev = &keyframes.front();
                next = prev;
            }
            
            // Interpolate between keyframes
            if (prev && next) {
                float t = 0.0f;
                if (prev != next) {
                    t = (evalTime - prev->time) / (next->time - prev->time);
                }
                
                Keyframe interpolated = interpolateKeyframes(*prev, *next, t);
                
                // Create transformation matrix
                glm::vec3 posVec(interpolated.position[0], interpolated.position[1], interpolated.position[2]);
                glm::quat rotQuat(interpolated.rotation[3], interpolated.rotation[0], interpolated.rotation[1], interpolated.rotation[2]);
                glm::vec3 scaleVec(interpolated.scale[0], interpolated.scale[1], interpolated.scale[2]);
                
                glm::mat4 translation = glm::translate(glm::mat4(1.0f), posVec);
                glm::mat4 rotation = glm::mat4_cast(rotQuat);
                glm::mat4 scale = glm::scale(glm::mat4(1.0f), scaleVec);
                
                boneTransforms[boneId] = translation * rotation * scale;
            }
        }
    }
    
    void AnimationClip::addEvent(const AnimationEvent& event) {
        m_events.push_back(event);
        
        // Keep events sorted by time
        std::sort(m_events.begin(), m_events.end(),
            [](const AnimationEvent& a, const AnimationEvent& b) {
                return a.time < b.time;
            });
    }
    
    Keyframe AnimationClip::interpolateKeyframes(const Keyframe& a, const Keyframe& b, float t) const {
        Keyframe result;
        result.time = a.time + (b.time - a.time) * t;
        
        // Interpolate position
        result.position[0] = a.position[0] + (b.position[0] - a.position[0]) * t;
        result.position[1] = a.position[1] + (b.position[1] - a.position[1]) * t;
        result.position[2] = a.position[2] + (b.position[2] - a.position[2]) * t;
        
        // Interpolate rotation (spherical linear interpolation for quaternions)
        glm::quat quatA(a.rotation[3], a.rotation[0], a.rotation[1], a.rotation[2]);
        glm::quat quatB(b.rotation[3], b.rotation[0], b.rotation[1], b.rotation[2]);
        glm::quat quatResult = glm::slerp(quatA, quatB, t);
        result.rotation[0] = quatResult.x;
        result.rotation[1] = quatResult.y;
        result.rotation[2] = quatResult.z;
        result.rotation[3] = quatResult.w;
        
        // Interpolate scale
        result.scale[0] = a.scale[0] + (b.scale[0] - a.scale[0]) * t;
        result.scale[1] = a.scale[1] + (b.scale[1] - a.scale[1]) * t;
        result.scale[2] = a.scale[2] + (b.scale[2] - a.scale[2]) * t;
        
        return result;
    }
    
    // SkeletalAnimation implementation
    SkeletalAnimation::SkeletalAnimation()
        : m_currentTime(0.0f)
        , m_isPlaying(false)
        , m_isPaused(false)
        , m_isLooping(true)
        , m_blendTime(0.2f)
        , m_blendProgress(0.0f)
        , m_blendMode(BlendMode::LINEAR)
        , m_isBlending(false) {
    }
    
    void SkeletalAnimation::initialize() {
        // Initialize skeletal animation system
    }
    
    void SkeletalAnimation::update(float deltaTime) {
        if (!m_isPlaying || m_isPaused) return;
        
        updateAnimation(deltaTime);
        
        if (m_isBlending) {
            updateBlending(deltaTime);
        }
    }
    
    void SkeletalAnimation::destroy() {
        // Cleanup skeletal animation system
    }
    
    void SkeletalAnimation::render() {
        // Skeletal animation doesn't render directly
    }
    
    void SkeletalAnimation::addBone(const Bone& bone) {
        m_bones.push_back(bone);
        m_boneNameToId[bone.name] = bone.id;
    }
    
    SkeletalAnimation::Bone* SkeletalAnimation::getBone(int id) {
        if (id >= 0 && id < static_cast<int>(m_bones.size())) {
            return &m_bones[id];
        }
        return nullptr;
    }
    
    SkeletalAnimation::Bone* SkeletalAnimation::getBone(const std::string& name) {
        auto it = m_boneNameToId.find(name);
        if (it != m_boneNameToId.end()) {
            return getBone(it->second);
        }
        return nullptr;
    }
    
    void SkeletalAnimation::addAnimationClip(std::unique_ptr<AnimationClip> clip) {
        if (clip) {
            m_clipNameToPtr[clip->getName()] = clip.get();
            m_animationClips.push_back(std::move(clip));
        }
    }
    
    AnimationClip* SkeletalAnimation::getAnimationClip(const std::string& name) const {
        auto it = m_clipNameToPtr.find(name);
        if (it != m_clipNameToPtr.end()) {
            return it->second;
        }
        return nullptr;
    }
    
    void SkeletalAnimation::playAnimation(const std::string& name, bool loop, float blendTime) {
        if (m_currentAnimation.empty()) {
            // First animation, play immediately
            m_currentAnimation = name;
            m_currentTime = 0.0f;
            m_isPlaying = true;
            m_isPaused = false;
            m_isLooping = loop;
        } else {
            // Blend to new animation
            blendToAnimation(name, blendTime);
        }
    }
    
    void SkeletalAnimation::stopAnimation() {
        m_isPlaying = false;
        m_isPaused = false;
        m_currentAnimation.clear();
        m_currentTime = 0.0f;
        m_isBlending = false;
        m_blendingToAnimation.clear();
    }
    
    void SkeletalAnimation::pauseAnimation() {
        m_isPaused = true;
    }
    
    void SkeletalAnimation::resumeAnimation() {
        m_isPaused = false;
    }
    
    void SkeletalAnimation::blendToAnimation(const std::string& name, float blendTime, BlendMode mode) {
        if (name == m_currentAnimation) return;
        
        AnimationClip* clip = getAnimationClip(name);
        if (!clip) return;
        
        m_blendingToAnimation = name;
        m_blendTime = blendTime;
        m_blendProgress = 0.0f;
        m_blendMode = mode;
        m_isBlending = true;
    }
    
    const glm::mat4& SkeletalAnimation::getBoneTransform(int boneId) const {
        static glm::mat4 identity(1.0f);
        if (boneId >= 0 && boneId < static_cast<int>(m_bones.size())) {
            return m_bones[boneId].finalTransformation;
        }
        return identity;
    }
    
    std::vector<glm::mat4> SkeletalAnimation::getBoneTransforms() const {
        std::vector<glm::mat4> transforms;
        transforms.reserve(m_bones.size());
        
        for (const auto& bone : m_bones) {
            transforms.push_back(bone.finalTransformation);
        }
        
        return transforms;
    }
    
    void SkeletalAnimation::setAnimationEventCallback(const std::string& eventName, std::function<void()> callback) {
        m_eventCallbacks[eventName] = callback;
    }
    
    void SkeletalAnimation::updateAnimation(float deltaTime) {
        AnimationClip* clip = getAnimationClip(m_currentAnimation);
        if (!clip) return;
        
        // Update time
        m_currentTime += deltaTime;
        
        // Handle looping
        if (m_isLooping && m_currentTime > clip->getDuration()) {
            m_currentTime = fmod(m_currentTime, clip->getDuration());
        }
        
        // Evaluate animation
        std::unordered_map<int, glm::mat4> boneTransforms;
        clip->evaluate(m_currentTime, boneTransforms);
        
        // Update bone transformations
        for (auto& bone : m_bones) {
            auto it = boneTransforms.find(bone.id);
            if (it != boneTransforms.end()) {
                bone.finalTransformation = it->second;
            } else {
                // Use identity if no keyframe
                bone.finalTransformation = glm::mat4(1.0f);
            }
        }
        
        // Handle events
        const std::vector<AnimationEvent>& events = clip->getEvents();
        for (const auto& event : events) {
            if (fabs(m_currentTime - event.time) < deltaTime) {
                // Trigger event
                auto callbackIt = m_eventCallbacks.find(event.name);
                if (callbackIt != m_eventCallbacks.end() && callbackIt->second) {
                    callbackIt->second();
                }
            }
        }
    }
    
    void SkeletalAnimation::updateBlending(float deltaTime) {
        m_blendProgress += deltaTime / m_blendTime;
        
        if (m_blendProgress >= 1.0f) {
            // Blending complete
            m_currentAnimation = m_blendingToAnimation;
            m_currentTime = 0.0f;
            m_isBlending = false;
            m_blendingToAnimation.clear();
        }
    }
    
    glm::mat4 SkeletalAnimation::calculateBoneTransform(int boneId, float time, AnimationClip* clip) const {
        if (!clip) return glm::mat4(1.0f);
        
        std::unordered_map<int, glm::mat4> boneTransforms;
        clip->evaluate(time, boneTransforms);
        
        auto it = boneTransforms.find(boneId);
        if (it != boneTransforms.end()) {
            return it->second;
        }
        
        return glm::mat4(1.0f);
    }
    
    float SkeletalAnimation::getBlendFactor(float progress, BlendMode mode) const {
        switch (mode) {
            case BlendMode::EASE_IN:
                return progress * progress;
            case BlendMode::EASE_OUT:
                return 1.0f - (1.0f - progress) * (1.0f - progress);
            case BlendMode::EASE_IN_OUT:
                return 0.5f * (1.0f - cos(progress * glm::pi<float>()));
            case BlendMode::OVERWRITE:
                return 1.0f;
            case BlendMode::LINEAR:
            default:
                return progress;
        }
    }
    
    // InverseKinematics implementation
    InverseKinematics::InverseKinematics()
        : m_skeletalAnimation(nullptr) {
    }
    
    void InverseKinematics::initialize() {
        // Get skeletal animation component from owner
        if (owner) {
            m_skeletalAnimation = owner->getComponent<SkeletalAnimation>();
        }
    }
    
    void InverseKinematics::update(float deltaTime) {
        // Solve all IK chains
        for (size_t i = 0; i < m_chains.size(); ++i) {
            solveIK(static_cast<int>(i));
        }
    }
    
    void InverseKinematics::destroy() {
        // Cleanup IK system
    }
    
    void InverseKinematics::render() {
        // IK system doesn't render directly
    }
    
    void InverseKinematics::addIKChain(const IKChain& chain) {
        m_chains.push_back(chain);
    }
    
    void InverseKinematics::removeIKChain(int chainId) {
        if (chainId >= 0 && chainId < static_cast<int>(m_chains.size())) {
            m_chains.erase(m_chains.begin() + chainId);
        }
    }
    
    InverseKinematics::IKChain* InverseKinematics::getIKChain(int chainId) {
        if (chainId >= 0 && chainId < static_cast<int>(m_chains.size())) {
            return &m_chains[chainId];
        }
        return nullptr;
    }
    
    void InverseKinematics::setTargetPosition(int chainId, const glm::vec3& target) {
        m_targets[chainId] = target;
    }
    
    void InverseKinematics::setPolePosition(int chainId, const glm::vec3& pole) {
        m_poles[chainId] = pole;
    }
    
    void InverseKinematics::solveIK(int chainId) {
        solveCCD(chainId);
    }
    
    void InverseKinematics::setJointConstraints(int chainId, int jointIndex, float minAngle, float maxAngle) {
        if (chainId >= 0 && chainId < static_cast<int>(m_chains.size())) {
            IKChain& chain = m_chains[chainId];
            if (jointIndex >= 0 && jointIndex < static_cast<int>(chain.joints.size())) {
                chain.joints[jointIndex].minAngle = minAngle;
                chain.joints[jointIndex].maxAngle = maxAngle;
            }
        }
    }
    
    void InverseKinematics::solveCCD(int chainId) {
        if (!m_skeletalAnimation) return;
        
        IKChain* chain = getIKChain(chainId);
        if (!chain) return;
        
        auto targetIt = m_targets.find(chainId);
        if (targetIt == m_targets.end()) return;
        
        glm::vec3 target = targetIt->second;
        
        // CCD (Cyclic Coordinate Descent) algorithm
        const int maxIterations = 10;
        const float epsilon = 0.01f;
        
        for (int iteration = 0; iteration < maxIterations; ++iteration) {
            glm::vec3 effectorPos = getBonePosition(chain->effectorBoneId);
            float distance = glm::distance(effectorPos, target);
            
            if (distance < epsilon) break;
            
            // Iterate backwards through the chain
            for (int i = static_cast<int>(chain->joints.size()) - 1; i >= 0; --i) {
                int boneId = chain->joints[i].boneId;
                
                glm::vec3 jointPos = getBonePosition(boneId);
                glm::vec3 toEffector = glm::normalize(effectorPos - jointPos);
                glm::vec3 toTarget = glm::normalize(target - jointPos);
                
                float angle = acos(glm::clamp(glm::dot(toEffector, toTarget), -1.0f, 1.0f));
                
                // Apply angle constraints
                angle = glm::clamp(angle, chain->joints[i].minAngle, chain->joints[i].maxAngle);
                
                if (angle > epsilon) {
                    glm::vec3 axis = glm::normalize(glm::cross(toEffector, toTarget));
                    glm::quat rotation = glm::angleAxis(angle, axis);
                    setBoneRotation(boneId, rotation);
                }
            }
        }
    }
    
    glm::vec3 InverseKinematics::getBonePosition(int boneId) const {
        if (m_skeletalAnimation) {
            SkeletalAnimation::Bone* bone = m_skeletalAnimation->getBone(boneId);
            if (bone) {
                glm::vec4 pos = bone->finalTransformation * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
                return glm::vec3(pos);
            }
        }
        return glm::vec3(0.0f);
    }
    
    void InverseKinematics::setBoneRotation(int boneId, const glm::quat& rotation) {
        // In a real implementation, this would update the bone's rotation
        // For now, we'll just acknowledge the call
    }
    
    // AdvancedAnimationController implementation
    AdvancedAnimationController::AdvancedAnimationController()
        : m_currentState("Default")
        , m_previousState("Default")
        , m_transitionTimer(0.0f)
        , m_transitionDuration(0.0f)
        , m_isTransitioning(false)
        , m_skeletalAnimation(nullptr) {
    }
    
    void AdvancedAnimationController::initialize() {
        // Get skeletal animation component from owner
        if (owner) {
            m_skeletalAnimation = owner->getComponent<SkeletalAnimation>();
        }
        
        // Add default state
        AnimationState defaultState;
        defaultState.name = "Default";
        defaultState.animationClip = "";
        defaultState.loop = true;
        defaultState.speed = 1.0f;
        m_states.push_back(defaultState);
    }
    
    void AdvancedAnimationController::update(float deltaTime) {
        updateState(deltaTime);
        updateTransitions(deltaTime);
        updateBlendTree(deltaTime);
    }
    
    void AdvancedAnimationController::destroy() {
        // Cleanup animation controller
    }
    
    void AdvancedAnimationController::render() {
        // Animation controller doesn't render directly
    }
    
    void AdvancedAnimationController::addState(const AnimationState& state) {
        // Check if state already exists
        for (auto& existingState : m_states) {
            if (existingState.name == state.name) {
                // Update existing state
                existingState = state;
                return;
            }
        }
        
        // Add new state
        m_states.push_back(state);
    }
    
    void AdvancedAnimationController::addTransition(const AnimationTransition& transition) {
        m_transitions.push_back(transition);
    }
    
    void AdvancedAnimationController::setState(const std::string& stateName) {
        // Check if state exists
        AnimationState* state = getState(stateName);
        if (!state) return;
        
        // Check if we can transition to this state
        if (canTransition(m_currentState, stateName)) {
            // Find transition
            for (const auto& transition : m_transitions) {
                if (transition.fromState == m_currentState && transition.toState == stateName) {
                    startTransition(stateName, transition.transitionTime);
                    return;
                }
            }
            
            // No transition defined, switch immediately
            m_previousState = m_currentState;
            m_currentState = stateName;
            
            // Play the animation for this state
            if (m_skeletalAnimation && !state->animationClip.empty()) {
                m_skeletalAnimation->playAnimation(state->animationClip, state->loop);
            }
        }
    }
    
    void AdvancedAnimationController::setFloatParameter(const std::string& name, float value) {
        m_floatParameters[name] = value;
    }
    
    void AdvancedAnimationController::setBoolParameter(const std::string& name, bool value) {
        m_boolParameters[name] = value;
    }
    
    void AdvancedAnimationController::setIntParameter(const std::string& name, int value) {
        m_intParameters[name] = value;
    }
    
    float AdvancedAnimationController::getFloatParameter(const std::string& name) const {
        auto it = m_floatParameters.find(name);
        if (it != m_floatParameters.end()) {
            return it->second;
        }
        return 0.0f;
    }
    
    bool AdvancedAnimationController::getBoolParameter(const std::string& name) const {
        auto it = m_boolParameters.find(name);
        if (it != m_boolParameters.end()) {
            return it->second;
        }
        return false;
    }
    
    int AdvancedAnimationController::getIntParameter(const std::string& name) const {
        auto it = m_intParameters.find(name);
        if (it != m_intParameters.end()) {
            return it->second;
        }
        return 0;
    }
    
    void AdvancedAnimationController::setBlendTree(const std::string& name, const BlendTreeNode& root) {
        m_blendTrees[name] = root;
    }
    
    void AdvancedAnimationController::useBlendTree(const std::string& name) {
        auto it = m_blendTrees.find(name);
        if (it != m_blendTrees.end()) {
            m_currentBlendTree = name;
        }
    }
    
    void AdvancedAnimationController::updateState(float deltaTime) {
        AnimationState* state = getState(m_currentState);
        if (!state || !m_skeletalAnimation) return;
        
        // Update animation speed based on state
        // In a real implementation, this would affect the playback rate
    }
    
    void AdvancedAnimationController::updateTransitions(float deltaTime) {
        if (!m_isTransitioning) return;
        
        m_transitionTimer += deltaTime;
        
        if (m_transitionTimer >= m_transitionDuration) {
            // Transition complete
            m_isTransitioning = false;
            m_previousState = m_currentState;
            m_currentState = m_blendingToAnimation;
            
            // Play the animation for the new state
            AnimationState* state = getState(m_currentState);
            if (state && m_skeletalAnimation && !state->animationClip.empty()) {
                m_skeletalAnimation->playAnimation(state->animationClip, state->loop);
            }
        }
    }
    
    void AdvancedAnimationController::updateBlendTree(float deltaTime) {
        if (m_currentBlendTree.empty() || !m_skeletalAnimation) return;
        
        auto it = m_blendTrees.find(m_currentBlendTree);
        if (it == m_blendTrees.end()) return;
        
        // In a real implementation, this would evaluate the blend tree
        // and blend between multiple animations based on parameters
    }
    
    AdvancedAnimationController::AnimationState* AdvancedAnimationController::getState(const std::string& name) {
        for (auto& state : m_states) {
            if (state.name == name) {
                return &state;
            }
        }
        return nullptr;
    }
    
    bool AdvancedAnimationController::canTransition(const std::string& from, const std::string& to) const {
        // Allow transition if a transition is defined or if it's the same state
        if (from == to) return true;
        
        for (const auto& transition : m_transitions) {
            if (transition.fromState == from && transition.toState == to) {
                // Check transition condition if it exists
                if (transition.condition) {
                    return transition.condition();
                }
                return true;
            }
        }
        
        return false;
    }
    
    void AdvancedAnimationController::startTransition(const std::string& toState, float duration) {
        m_blendingToAnimation = toState;
        m_transitionTimer = 0.0f;
        m_transitionDuration = duration;
        m_isTransitioning = true;
        
        // Start blending in the skeletal animation system
        AnimationState* state = getState(toState);
        if (state && m_skeletalAnimation && !state->animationClip.empty()) {
            m_skeletalAnimation->blendToAnimation(state->animationClip, duration);
        }
    }
}
