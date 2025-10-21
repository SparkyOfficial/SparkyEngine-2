/*
 * AdvancedBehaviorTree.h
 * 
 * Author: Андрій Будильников
 * 
 * Advanced behavior tree system with additional node types and functionality
 */

#pragma once

#include "BehaviorTree.h"
#include <memory>
#include <vector>
#include <functional>
#include <unordered_map>
#include <string>

namespace Sparky {
    
    // Forward declarations
    class GameObject;
    class AIComponent;
    
    // Advanced behavior tree node types
    enum class AdvancedNodeType {
        ACTION,
        CONDITION,
        SELECTOR,
        SEQUENCE,
        PARALLEL,
        DECORATOR,
        INVERTER,
        REPEATER,
        SUCCEEDER,
        TIMER,
        BLACKBOARD_CONDITION,
        BLACKBOARD_ACTION,
        SUBTREE
    };
    
    // Blackboard for sharing data between nodes
    class Blackboard {
    public:
        Blackboard();
        ~Blackboard();
        
        // Set values
        void setFloat(const std::string& key, float value);
        void setInt(const std::string& key, int value);
        void setBool(const std::string& key, bool value);
        void setVector3(const std::string& key, const glm::vec3& value);
        void setString(const std::string& key, const std::string& value);
        
        // Get values
        float getFloat(const std::string& key, float defaultValue = 0.0f) const;
        int getInt(const std::string& key, int defaultValue = 0) const;
        bool getBool(const std::string& key, bool defaultValue = false) const;
        glm::vec3 getVector3(const std::string& key, const glm::vec3& defaultValue = glm::vec3(0.0f)) const;
        std::string getString(const std::string& key, const std::string& defaultValue = "") const;
        
        // Check if key exists
        bool hasKey(const std::string& key) const;
        
        // Remove key
        void removeKey(const std::string& key);
        
        // Clear all data
        void clear();
        
    private:
        std::unordered_map<std::string, float> floatValues;
        std::unordered_map<std::string, int> intValues;
        std::unordered_map<std::string, bool> boolValues;
        std::unordered_map<std::string, glm::vec3> vector3Values;
        std::unordered_map<std::string, std::string> stringValues;
    };
    
    // Base class for advanced behavior tree nodes
    class AdvancedBehaviorNode : public BehaviorNode {
    public:
        AdvancedBehaviorNode(AdvancedNodeType type);
        virtual ~AdvancedBehaviorNode();
        
        virtual NodeStatus update(float deltaTime) override = 0;
        virtual void reset() override;
        
        AdvancedNodeType getAdvancedType() const { return advancedType; }
        
        // Blackboard access
        void setBlackboard(Blackboard* bb) { blackboard = bb; }
        Blackboard* getBlackboard() const { return blackboard; }
        
        // Name for debugging
        void setName(const std::string& name) { nodeName = name; }
        const std::string& getName() const { return nodeName; }
        
    protected:
        AdvancedNodeType advancedType;
        Blackboard* blackboard;
        std::string nodeName;
    };
    
    // Decorator node - modifies the behavior of a single child
    class DecoratorNode : public AdvancedBehaviorNode {
    public:
        DecoratorNode();
        virtual ~DecoratorNode();
        
        void setChild(std::unique_ptr<AdvancedBehaviorNode> child);
        AdvancedBehaviorNode* getChild() const { return child.get(); }
        
    protected:
        std::unique_ptr<AdvancedBehaviorNode> child;
    };
    
    // Inverter node - inverts the result of its child
    class InverterNode : public DecoratorNode {
    public:
        InverterNode();
        virtual ~InverterNode();
        
        NodeStatus update(float deltaTime) override;
    };
    
    // Repeater node - repeats its child until it fails or a limit is reached
    class RepeaterNode : public DecoratorNode {
    public:
        RepeaterNode(int limit = -1); // -1 means infinite
        virtual ~RepeaterNode();
        
        NodeStatus update(float deltaTime) override;
        void setLimit(int limit) { this->limit = limit; }
        int getLimit() const { return limit; }
        
    private:
        int limit;
        int count;
    };
    
    // Succeeder node - always returns success after executing its child
    class SucceederNode : public DecoratorNode {
    public:
        SucceederNode();
        virtual ~SucceederNode();
        
        NodeStatus update(float deltaTime) override;
    };
    
    // Timer node - executes its child for a specific amount of time
    class TimerNode : public DecoratorNode {
    public:
        TimerNode(float duration);
        virtual ~TimerNode();
        
        NodeStatus update(float deltaTime) override;
        void reset() override;
        void setDuration(float duration) { this->duration = duration; }
        float getDuration() const { return duration; }
        
    private:
        float duration;
        float elapsed;
    };
    
    // Blackboard condition node - checks a condition based on blackboard values
    class BlackboardConditionNode : public AdvancedBehaviorNode {
    public:
        BlackboardConditionNode(std::function<bool(Blackboard*)> conditionFunc);
        virtual ~BlackboardConditionNode();
        
        NodeStatus update(float deltaTime) override;
        
    private:
        std::function<bool(Blackboard*)> conditionFunction;
    };
    
    // Blackboard action node - performs an action that modifies blackboard values
    class BlackboardActionNode : public AdvancedBehaviorNode {
    public:
        BlackboardActionNode(std::function<NodeStatus(Blackboard*, float)> actionFunc);
        virtual ~BlackboardActionNode();
        
        NodeStatus update(float deltaTime) override;
        
    private:
        std::function<NodeStatus(Blackboard*, float)> actionFunction;
    };
    
    // Subtree node - executes another behavior tree
    class SubtreeNode : public AdvancedBehaviorNode {
    public:
        SubtreeNode(std::unique_ptr<BehaviorTree> subtree);
        virtual ~SubtreeNode();
        
        NodeStatus update(float deltaTime) override;
        void setSubtree(std::unique_ptr<BehaviorTree> subtree) { this->subtree = std::move(subtree); }
        BehaviorTree* getSubtree() const { return subtree.get(); }
        
    private:
        std::unique_ptr<BehaviorTree> subtree;
    };
    
    // Parallel node with policy - executes all children simultaneously
    class ParallelNode : public AdvancedBehaviorNode {
    public:
        enum class Policy {
            REQUIRE_ONE,  // Succeeds if one child succeeds, fails if all fail
            REQUIRE_ALL   // Succeeds if all children succeed, fails if one fails
        };
        
        ParallelNode(Policy successPolicy = Policy::REQUIRE_ONE, Policy failurePolicy = Policy::REQUIRE_ONE);
        virtual ~ParallelNode();
        
        NodeStatus update(float deltaTime) override;
        void addChild(std::unique_ptr<AdvancedBehaviorNode> child);
        
        void setSuccessPolicy(Policy policy) { successPolicy = policy; }
        void setFailurePolicy(Policy policy) { failurePolicy = policy; }
        
    private:
        Policy successPolicy;
        Policy failurePolicy;
        std::vector<std::unique_ptr<AdvancedBehaviorNode>> children;
        std::vector<NodeStatus> childStatuses;
    };
    
    // Behavior tree class
    class AdvancedBehaviorTree : public BehaviorTree {
    public:
        AdvancedBehaviorTree();
        ~AdvancedBehaviorTree();
        
        void setRootNode(std::unique_ptr<AdvancedBehaviorNode> root);
        void update(float deltaTime);
        void reset();
        
        // Blackboard access
        Blackboard* getBlackboard() { return &blackboard; }
        
    private:
        std::unique_ptr<AdvancedBehaviorNode> advancedRootNode;
        Blackboard blackboard;
    };
}
