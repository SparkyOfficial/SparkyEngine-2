/*
 * AdvancedBehaviorTree.cpp
 * 
 * Author: Андрій Будильников
 * 
 * Advanced behavior tree system implementation
 */

#include "../include/AdvancedBehaviorTree.h"
#include "../include/Logger.h"
#include <algorithm>
#include <cmath>

namespace Sparky {
    
    // Blackboard implementation
    Blackboard::Blackboard() {
    }
    
    Blackboard::~Blackboard() {
    }
    
    void Blackboard::setFloat(const std::string& key, float value) {
        floatValues[key] = value;
    }
    
    void Blackboard::setInt(const std::string& key, int value) {
        intValues[key] = value;
    }
    
    void Blackboard::setBool(const std::string& key, bool value) {
        boolValues[key] = value;
    }
    
    void Blackboard::setVector3(const std::string& key, const glm::vec3& value) {
        vector3Values[key] = value;
    }
    
    void Blackboard::setString(const std::string& key, const std::string& value) {
        stringValues[key] = value;
    }
    
    float Blackboard::getFloat(const std::string& key, float defaultValue) const {
        auto it = floatValues.find(key);
        return (it != floatValues.end()) ? it->second : defaultValue;
    }
    
    int Blackboard::getInt(const std::string& key, int defaultValue) const {
        auto it = intValues.find(key);
        return (it != intValues.end()) ? it->second : defaultValue;
    }
    
    bool Blackboard::getBool(const std::string& key, bool defaultValue) const {
        auto it = boolValues.find(key);
        return (it != boolValues.end()) ? it->second : defaultValue;
    }
    
    glm::vec3 Blackboard::getVector3(const std::string& key, const glm::vec3& defaultValue) const {
        auto it = vector3Values.find(key);
        return (it != vector3Values.end()) ? it->second : defaultValue;
    }
    
    std::string Blackboard::getString(const std::string& key, const std::string& defaultValue) const {
        auto it = stringValues.find(key);
        return (it != stringValues.end()) ? it->second : defaultValue;
    }
    
    bool Blackboard::hasKey(const std::string& key) const {
        return (floatValues.find(key) != floatValues.end()) ||
               (intValues.find(key) != intValues.end()) ||
               (boolValues.find(key) != boolValues.end()) ||
               (vector3Values.find(key) != vector3Values.end()) ||
               (stringValues.find(key) != stringValues.end());
    }
    
    void Blackboard::removeKey(const std::string& key) {
        floatValues.erase(key);
        intValues.erase(key);
        boolValues.erase(key);
        vector3Values.erase(key);
        stringValues.erase(key);
    }
    
    void Blackboard::clear() {
        floatValues.clear();
        intValues.clear();
        boolValues.clear();
        vector3Values.clear();
        stringValues.clear();
    }
    
    // AdvancedBehaviorNode implementation
    AdvancedBehaviorNode::AdvancedBehaviorNode(AdvancedNodeType type) 
        : BehaviorNode(static_cast<NodeType>(type)), advancedType(type), blackboard(nullptr) {
    }
    
    AdvancedBehaviorNode::~AdvancedBehaviorNode() {
    }
    
    void AdvancedBehaviorNode::reset() {
        BehaviorNode::reset();
    }
    
    // DecoratorNode implementation
    DecoratorNode::DecoratorNode() : AdvancedBehaviorNode(AdvancedNodeType::DECORATOR) {
    }
    
    DecoratorNode::~DecoratorNode() {
    }
    
    void DecoratorNode::setChild(std::unique_ptr<AdvancedBehaviorNode> child) {
        this->child = std::move(child);
        if (this->child && blackboard) {
            this->child->setBlackboard(blackboard);
        }
    }
    
    // InverterNode implementation
    InverterNode::InverterNode() : DecoratorNode() {
        setName("Inverter");
    }
    
    InverterNode::~InverterNode() {
    }
    
    NodeStatus InverterNode::update(float deltaTime) {
        if (!child) {
            status = NodeStatus::FAILURE;
            return status;
        }
        
        NodeStatus childStatus = child->update(deltaTime);
        
        switch (childStatus) {
            case NodeStatus::SUCCESS:
                status = NodeStatus::FAILURE;
                break;
            case NodeStatus::FAILURE:
                status = NodeStatus::SUCCESS;
                break;
            case NodeStatus::RUNNING:
                status = NodeStatus::RUNNING;
                break;
        }
        
        return status;
    }
    
    // RepeaterNode implementation
    RepeaterNode::RepeaterNode(int limit) 
        : DecoratorNode(), limit(limit), count(0) {
        setName("Repeater");
    }
    
    RepeaterNode::~RepeaterNode() {
    }
    
    NodeStatus RepeaterNode::update(float deltaTime) {
        if (!child) {
            status = NodeStatus::FAILURE;
            return status;
        }
        
        // If we've reached the limit, succeed
        if (limit >= 0 && count >= limit) {
            count = 0;
            status = NodeStatus::SUCCESS;
            return status;
        }
        
        NodeStatus childStatus = child->update(deltaTime);
        
        switch (childStatus) {
            case NodeStatus::SUCCESS:
                count++;
                if (limit >= 0 && count >= limit) {
                    count = 0;
                    status = NodeStatus::SUCCESS;
                } else {
                    // Reset child and run again
                    child->reset();
                    status = NodeStatus::RUNNING;
                }
                break;
            case NodeStatus::FAILURE:
                count = 0;
                status = NodeStatus::FAILURE;
                break;
            case NodeStatus::RUNNING:
                status = NodeStatus::RUNNING;
                break;
        }
        
        return status;
    }
    
    // SucceederNode implementation
    SucceederNode::SucceederNode() : DecoratorNode() {
        setName("Succeeder");
    }
    
    SucceederNode::~SucceederNode() {
    }
    
    NodeStatus SucceederNode::update(float deltaTime) {
        if (!child) {
            status = NodeStatus::FAILURE;
            return status;
        }
        
        NodeStatus childStatus = child->update(deltaTime);
        
        switch (childStatus) {
            case NodeStatus::SUCCESS:
            case NodeStatus::FAILURE:
                status = NodeStatus::SUCCESS;
                break;
            case NodeStatus::RUNNING:
                status = NodeStatus::RUNNING;
                break;
        }
        
        return status;
    }
    
    // TimerNode implementation
    TimerNode::TimerNode(float duration) 
        : DecoratorNode(), duration(duration), elapsed(0.0f) {
        setName("Timer");
    }
    
    TimerNode::~TimerNode() {
    }
    
    NodeStatus TimerNode::update(float deltaTime) {
        if (!child) {
            status = NodeStatus::FAILURE;
            return status;
        }
        
        // If time is up, succeed
        if (elapsed >= duration) {
            elapsed = 0.0f;
            status = NodeStatus::SUCCESS;
            return status;
        }
        
        NodeStatus childStatus = child->update(deltaTime);
        elapsed += deltaTime;
        
        // If time is up now, succeed
        if (elapsed >= duration) {
            elapsed = 0.0f;
            status = NodeStatus::SUCCESS;
        } else {
            status = NodeStatus::RUNNING;
        }
        
        return status;
    }
    
    void TimerNode::reset() {
        DecoratorNode::reset();
        elapsed = 0.0f;
    }
    
    // BlackboardConditionNode implementation
    BlackboardConditionNode::BlackboardConditionNode(std::function<bool(Blackboard*)> conditionFunc) 
        : AdvancedBehaviorNode(AdvancedNodeType::BLACKBOARD_CONDITION), conditionFunction(conditionFunc) {
        setName("BlackboardCondition");
    }
    
    BlackboardConditionNode::~BlackboardConditionNode() {
    }
    
    NodeStatus BlackboardConditionNode::update(float deltaTime) {
        if (conditionFunction && blackboard) {
            status = conditionFunction(blackboard) ? NodeStatus::SUCCESS : NodeStatus::FAILURE;
        } else {
            status = NodeStatus::FAILURE;
        }
        return status;
    }
    
    // BlackboardActionNode implementation
    BlackboardActionNode::BlackboardActionNode(std::function<NodeStatus(Blackboard*, float)> actionFunc) 
        : AdvancedBehaviorNode(AdvancedNodeType::BLACKBOARD_ACTION), actionFunction(actionFunc) {
        setName("BlackboardAction");
    }
    
    BlackboardActionNode::~BlackboardActionNode() {
    }
    
    NodeStatus BlackboardActionNode::update(float deltaTime) {
        if (actionFunction && blackboard) {
            status = actionFunction(blackboard, deltaTime);
        } else {
            status = NodeStatus::FAILURE;
        }
        return status;
    }
    
    // SubtreeNode implementation
    SubtreeNode::SubtreeNode(std::unique_ptr<BehaviorTree> subtree) 
        : AdvancedBehaviorNode(AdvancedNodeType::SUBTREE), subtree(std::move(subtree)) {
        setName("Subtree");
    }
    
    SubtreeNode::~SubtreeNode() {
    }
    
    NodeStatus SubtreeNode::update(float deltaTime) {
        if (subtree) {
            // Update the subtree
            subtree->update(deltaTime);
            // For simplicity, we'll always return SUCCESS
            // In a more advanced implementation, we might want to track the subtree's status
            status = NodeStatus::SUCCESS;
        } else {
            status = NodeStatus::FAILURE;
        }
        return status;
    }
    
    // ParallelNode implementation
    ParallelNode::ParallelNode(Policy successPolicy, Policy failurePolicy) 
        : AdvancedBehaviorNode(AdvancedNodeType::PARALLEL), 
          successPolicy(successPolicy), failurePolicy(failurePolicy) {
        setName("Parallel");
    }
    
    ParallelNode::~ParallelNode() {
    }
    
    NodeStatus ParallelNode::update(float deltaTime) {
        if (children.empty()) {
            status = NodeStatus::SUCCESS;
            return status;
        }
        
        // Resize status array if needed
        if (childStatuses.size() != children.size()) {
            childStatuses.resize(children.size(), NodeStatus::RUNNING);
        }
        
        int successCount = 0;
        int failureCount = 0;
        
        // Update all children
        for (size_t i = 0; i < children.size(); ++i) {
            if (childStatuses[i] == NodeStatus::RUNNING) {
                childStatuses[i] = children[i]->update(deltaTime);
            }
            
            switch (childStatuses[i]) {
                case NodeStatus::SUCCESS:
                    successCount++;
                    break;
                case NodeStatus::FAILURE:
                    failureCount++;
                    break;
                case NodeStatus::RUNNING:
                    // Still running
                    break;
            }
        }
        
        // Check success policy
        bool success = false;
        if (successPolicy == Policy::REQUIRE_ALL) {
            success = (successCount == static_cast<int>(children.size()));
        } else { // REQUIRE_ONE
            success = (successCount > 0);
        }
        
        // Check failure policy
        bool failure = false;
        if (failurePolicy == Policy::REQUIRE_ALL) {
            failure = (failureCount == static_cast<int>(children.size()));
        } else { // REQUIRE_ONE
            failure = (failureCount > 0);
        }
        
        if (success) {
            // Reset all children for next run
            for (auto& child : children) {
                child->reset();
            }
            std::fill(childStatuses.begin(), childStatuses.end(), NodeStatus::RUNNING);
            status = NodeStatus::SUCCESS;
        } else if (failure) {
            // Reset all children for next run
            for (auto& child : children) {
                child->reset();
            }
            std::fill(childStatuses.begin(), childStatuses.end(), NodeStatus::RUNNING);
            status = NodeStatus::FAILURE;
        } else {
            status = NodeStatus::RUNNING;
        }
        
        return status;
    }
    
    void ParallelNode::addChild(std::unique_ptr<AdvancedBehaviorNode> child) {
        children.push_back(std::move(child));
        if (children.back() && blackboard) {
            children.back()->setBlackboard(blackboard);
        }
        childStatuses.push_back(NodeStatus::RUNNING);
    }
    
    // AdvancedBehaviorTree implementation
    AdvancedBehaviorTree::AdvancedBehaviorTree() : BehaviorTree() {
    }
    
    AdvancedBehaviorTree::~AdvancedBehaviorTree() {
    }
    
    void AdvancedBehaviorTree::setRootNode(std::unique_ptr<AdvancedBehaviorNode> root) {
        advancedRootNode = std::move(root);
        if (advancedRootNode) {
            advancedRootNode->setBlackboard(&blackboard);
        }
    }
    
    void AdvancedBehaviorTree::update(float deltaTime) {
        if (advancedRootNode) {
            NodeStatus rootStatus = advancedRootNode->update(deltaTime);
            
            // If the root node finished (success or failure), reset it for the next run
            if (rootStatus != NodeStatus::RUNNING) {
                advancedRootNode->reset();
            }
        }
    }
    
    void AdvancedBehaviorTree::reset() {
        if (advancedRootNode) {
            advancedRootNode->reset();
        }
        blackboard.clear();
    }
}