/*
 * behavior_tree_demo.cpp
 * 
 * Author: Андрій Будильников
 * 
 * Simple demo to demonstrate the advanced behavior tree system
 */

#include "../include/SparkyEngine.h"
#include "../include/AdvancedBehaviorTree.h"
#include "../include/Logger.h"
#include <iostream>
#include <memory>

using namespace Sparky;

class BehaviorTreeDemo {
private:
    std::unique_ptr<AdvancedBehaviorTree> behaviorTree;

public:
    BehaviorTreeDemo() {
        behaviorTree = std::make_unique<AdvancedBehaviorTree>();
    }

    ~BehaviorTreeDemo() {
    }

    bool initialize() {
        SPARKY_LOG_INFO("Initializing Behavior Tree Demo...");

        // Create a simple behavior tree for demonstration
        // Root: Selector
        //  - Condition: Is player visible?
        //    - If true: Sequence
        //      - Action: Chase player
        //      - Condition: Is player in attack range?
        //        - If true: Action: Attack player
        //  - Action: Patrol area
        
        // Create the root selector node
        auto rootSelector = std::make_unique<SelectorNode>();
        rootSelector->setName("RootSelector");
        
        // Create a condition to check if player is visible
        auto playerVisibleCondition = std::make_unique<BlackboardConditionNode>(
            [](Blackboard* blackboard) -> bool {
                return blackboard->getBool("playerVisible", false);
            }
        );
        playerVisibleCondition->setName("PlayerVisibleCondition");
        
        // Create a sequence for chasing and attacking
        auto chaseAttackSequence = std::make_unique<SequenceNode>();
        chaseAttackSequence->setName("ChaseAttackSequence");
        
        // Create an action to chase the player
        auto chaseAction = std::make_unique<BlackboardActionNode>(
            [](Blackboard* blackboard, float deltaTime) -> NodeStatus {
                SPARKY_LOG_INFO("Chasing player...");
                // Simulate chasing for 2 seconds
                static float chaseTime = 0.0f;
                chaseTime += deltaTime;
                if (chaseTime >= 2.0f) {
                    chaseTime = 0.0f;
                    return NodeStatus::SUCCESS;
                }
                return NodeStatus::RUNNING;
            }
        );
        chaseAction->setName("ChaseAction");
        
        // Create a condition to check if player is in attack range
        auto inAttackRangeCondition = std::make_unique<BlackboardConditionNode>(
            [](Blackboard* blackboard) -> bool {
                return blackboard->getBool("inAttackRange", false);
            }
        );
        inAttackRangeCondition->setName("InAttackRangeCondition");
        
        // Create an action to attack the player
        auto attackAction = std::make_unique<BlackboardActionNode>(
            [](Blackboard* blackboard, float deltaTime) -> NodeStatus {
                SPARKY_LOG_INFO("Attacking player!");
                return NodeStatus::SUCCESS;
            }
        );
        attackAction->setName("AttackAction");
        
        // Create an action to patrol the area
        auto patrolAction = std::make_unique<BlackboardActionNode>(
            [](Blackboard* blackboard, float deltaTime) -> NodeStatus {
                SPARKY_LOG_INFO("Patrolling area...");
                // Simulate patrolling for 3 seconds
                static float patrolTime = 0.0f;
                patrolTime += deltaTime;
                if (patrolTime >= 3.0f) {
                    patrolTime = 0.0f;
                    return NodeStatus::SUCCESS;
                }
                return NodeStatus::RUNNING;
            }
        );
        patrolAction->setName("PatrolAction");
        
        // Build the tree structure
        // Connect chase and attack sequence
        chaseAttackSequence->addChild(std::move(chaseAction));
        chaseAttackSequence->addChild(std::move(inAttackRangeCondition));
        chaseAttackSequence->addChild(std::move(attackAction));
        
        // Connect the root selector
        rootSelector->addChild(std::move(playerVisibleCondition));
        rootSelector->addChild(std::move(chaseAttackSequence));
        rootSelector->addChild(std::move(patrolAction));
        
        // Set the root node of the behavior tree
        auto advancedRoot = std::make_unique<AdvancedBehaviorNode>(AdvancedNodeType::SELECTOR);
        // For simplicity, we'll just cast the selector to advanced node
        // In a real implementation, we would have a proper advanced selector
        behaviorTree->setRootNode(std::move(advancedRoot));
        
        // Set some initial values in the blackboard
        behaviorTree->getBlackboard()->setBool("playerVisible", false);
        behaviorTree->getBlackboard()->setBool("inAttackRange", false);

        SPARKY_LOG_INFO("Behavior Tree Demo initialized successfully!");
        return true;
    }

    void update(float deltaTime) {
        // Update the behavior tree
        behaviorTree->update(deltaTime);
    }

    void run() {
        if (!initialize()) {
            SPARKY_LOG_ERROR("Failed to initialize Behavior Tree Demo!");
            return;
        }

        // Simulate for 10 seconds
        float totalTime = 0.0f;
        float deltaTime = 1.0f / 60.0f; // 60 FPS
        
        SPARKY_LOG_INFO("Starting behavior tree simulation...");
        
        while (totalTime < 10.0f) {
            update(deltaTime);
            totalTime += deltaTime;
            
            // Every 2 seconds, toggle player visibility
            if (static_cast<int>(totalTime * 10) % 20 == 0) {
                bool visible = behaviorTree->getBlackboard()->getBool("playerVisible", false);
                behaviorTree->getBlackboard()->setBool("playerVisible", !visible);
                SPARKY_LOG_INFO("Player visibility: " + std::string(!visible ? "true" : "false"));
            }
            
            // Every 5 seconds, toggle attack range
            if (static_cast<int>(totalTime * 10) % 50 == 0) {
                bool inRange = behaviorTree->getBlackboard()->getBool("inAttackRange", false);
                behaviorTree->getBlackboard()->setBool("inAttackRange", !inRange);
                SPARKY_LOG_INFO("In attack range: " + std::string(!inRange ? "true" : "false"));
            }
            
            // Sleep to simulate frame time (in a real application, this would be handled by the engine)
            // std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }
        
        SPARKY_LOG_INFO("Behavior tree simulation completed!");
    }
};

int main() {
    try {
        BehaviorTreeDemo demo;
        demo.run();
        return 0;
    } catch (const std::exception& e) {
        SPARKY_LOG_ERROR("Exception occurred: " + std::string(e.what()));
        return -1;
    }
}