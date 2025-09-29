#pragma once

#include <memory>
#include <vector>
#include <functional>

namespace Sparky {
    
    // Forward declarations
    class GameObject;
    
    // Behavior tree node types
    enum class NodeType {
        ACTION,
        CONDITION,
        SELECTOR,
        SEQUENCE,
        PARALLEL
    };
    
    // Behavior tree node status
    enum class NodeStatus {
        SUCCESS,
        FAILURE,
        RUNNING
    };
    
    // Base class for behavior tree nodes
    class BehaviorNode {
    public:
        BehaviorNode(NodeType type);
        virtual ~BehaviorNode();
        
        virtual NodeStatus update(float deltaTime) = 0;
        virtual void reset();
        
        NodeType getType() const { return type; }
        NodeStatus getStatus() const { return status; }
        
        // Child management
        void addChild(std::unique_ptr<BehaviorNode> child);
        const std::vector<std::unique_ptr<BehaviorNode>>& getChildren() const { return children; }
        
    protected:
        NodeType type;
        NodeStatus status;
        std::vector<std::unique_ptr<BehaviorNode>> children;
    };
    
    // Action node - performs an action
    class ActionNode : public BehaviorNode {
    public:
        ActionNode(std::function<NodeStatus(float)> actionFunc);
        virtual ~ActionNode();
        
        NodeStatus update(float deltaTime) override;
        
    private:
        std::function<NodeStatus(float)> actionFunction;
    };
    
    // Condition node - checks a condition
    class ConditionNode : public BehaviorNode {
    public:
        ConditionNode(std::function<bool()> conditionFunc);
        virtual ~ConditionNode();
        
        NodeStatus update(float deltaTime) override;
        
    private:
        std::function<bool()> conditionFunction;
    };
    
    // Selector node - tries each child until one succeeds
    class SelectorNode : public BehaviorNode {
    public:
        SelectorNode();
        virtual ~SelectorNode();
        
        NodeStatus update(float deltaTime) override;
        
    private:
        size_t currentChildIndex;
    };
    
    // Sequence node - executes children in order until one fails
    class SequenceNode : public BehaviorNode {
    public:
        SequenceNode();
        virtual ~SequenceNode();
        
        NodeStatus update(float deltaTime) override;
        
    private:
        size_t currentChildIndex;
    };
    
    // Behavior tree class
    class BehaviorTree {
    public:
        BehaviorTree();
        ~BehaviorTree();
        
        void setRootNode(std::unique_ptr<BehaviorNode> root);
        void update(float deltaTime);
        void reset();
        
    private:
        std::unique_ptr<BehaviorNode> rootNode;
    };
}