#include "../include/BehaviorTree.h"
#include "../include/Logger.h"

namespace Sparky {
    
    // BehaviorNode implementation
    BehaviorNode::BehaviorNode(NodeType type) : type(type), status(NodeStatus::RUNNING) {
    }
    
    BehaviorNode::~BehaviorNode() {
    }
    
    void BehaviorNode::reset() {
        status = NodeStatus::RUNNING;
        for (auto& child : children) {
            child->reset();
        }
    }
    
    void BehaviorNode::addChild(std::unique_ptr<BehaviorNode> child) {
        children.push_back(std::move(child));
    }
    
    // ActionNode implementation
    ActionNode::ActionNode(std::function<NodeStatus(float)> actionFunc) 
        : BehaviorNode(NodeType::ACTION), actionFunction(actionFunc) {
    }
    
    ActionNode::~ActionNode() {
    }
    
    NodeStatus ActionNode::update(float deltaTime) {
        if (actionFunction) {
            status = actionFunction(deltaTime);
        } else {
            status = NodeStatus::FAILURE;
        }
        return status;
    }
    
    // ConditionNode implementation
    ConditionNode::ConditionNode(std::function<bool()> conditionFunc) 
        : BehaviorNode(NodeType::CONDITION), conditionFunction(conditionFunc) {
    }
    
    ConditionNode::~ConditionNode() {
    }
    
    NodeStatus ConditionNode::update(float deltaTime) {
        if (conditionFunction) {
            status = conditionFunction() ? NodeStatus::SUCCESS : NodeStatus::FAILURE;
        } else {
            status = NodeStatus::FAILURE;
        }
        return status;
    }
    
    // SelectorNode implementation
    SelectorNode::SelectorNode() : BehaviorNode(NodeType::SELECTOR), currentChildIndex(0) {
    }
    
    SelectorNode::~SelectorNode() {
    }
    
    NodeStatus SelectorNode::update(float deltaTime) {
        // Try each child until one succeeds
        for (size_t i = 0; i < children.size(); ++i) {
            NodeStatus childStatus = children[i]->update(deltaTime);
            
            if (childStatus == NodeStatus::SUCCESS) {
                // Reset all children for next run
                for (auto& child : children) {
                    child->reset();
                }
                status = NodeStatus::SUCCESS;
                return status;
            } else if (childStatus == NodeStatus::RUNNING) {
                // If a child is still running, we're still running
                status = NodeStatus::RUNNING;
                return status;
            }
            // If child failed, try the next one
        }
        
        // All children failed
        status = NodeStatus::FAILURE;
        return status;
    }
    
    // SequenceNode implementation
    SequenceNode::SequenceNode() : BehaviorNode(NodeType::SEQUENCE), currentChildIndex(0) {
    }
    
    SequenceNode::~SequenceNode() {
    }
    
    NodeStatus SequenceNode::update(float deltaTime) {
        // Execute children in order until one fails
        for (size_t i = 0; i < children.size(); ++i) {
            NodeStatus childStatus = children[i]->update(deltaTime);
            
            if (childStatus == NodeStatus::FAILURE) {
                // Reset all children for next run
                for (auto& child : children) {
                    child->reset();
                }
                status = NodeStatus::FAILURE;
                return status;
            } else if (childStatus == NodeStatus::RUNNING) {
                // If a child is still running, we're still running
                status = NodeStatus::RUNNING;
                return status;
            }
            // If child succeeded, continue to the next one
        }
        
        // All children succeeded
        status = NodeStatus::SUCCESS;
        return status;
    }
    
    // BehaviorTree implementation
    BehaviorTree::BehaviorTree() {
    }
    
    BehaviorTree::~BehaviorTree() {
    }
    
    void BehaviorTree::setRootNode(std::unique_ptr<BehaviorNode> root) {
        rootNode = std::move(root);
    }
    
    void BehaviorTree::update(float deltaTime) {
        if (rootNode) {
            NodeStatus rootStatus = rootNode->update(deltaTime);
            
            // If the root node finished (success or failure), reset it for the next run
            if (rootStatus != NodeStatus::RUNNING) {
                rootNode->reset();
            }
        }
    }
    
    void BehaviorTree::reset() {
        if (rootNode) {
            rootNode->reset();
        }
    }
}