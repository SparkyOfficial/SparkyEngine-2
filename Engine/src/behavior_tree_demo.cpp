#include "../include/SparkyEngine.h"
#include "../include/BehaviorTree.h"
#include "../include/Logger.h"
#include <iostream>
#include <memory>

using namespace Sparky;

class BehaviorTreeDemo {
private:
    std::unique_ptr<BehaviorTree> behaviorTree;

public:
    bool initialize() {
        behaviorTree = std::make_unique<BehaviorTree>();
        
        // Create the root selector node
        auto rootSelector = std::make_unique<SelectorNode>();
        
        // Create a condition to check if player is visible
        // For basic behavior tree, we can't access blackboard directly
        // We'll simulate this with a simple condition
        auto playerVisibleCondition = std::make_unique<ConditionNode>(
            []() -> bool {
                // In a real implementation, this would check the blackboard
                // For demo purposes, we'll return false
                return false;
            }
        );
        
        // Create a sequence for chasing and attacking
        auto chaseAttackSequence = std::make_unique<SequenceNode>();
        
        // Create an action to chase the player
        auto chaseAction = std::make_unique<ActionNode>(
            [](float deltaTime) -> NodeStatus {
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
        
        // Create a condition to check if player is in attack range
        auto inAttackRangeCondition = std::make_unique<ConditionNode>(
            []() -> bool {
                // In a real implementation, this would check the blackboard
                // For demo purposes, we'll return false
                return false;
            }
        );
        
        // Create an action to attack the player
        auto attackAction = std::make_unique<ActionNode>(
            [](float deltaTime) -> NodeStatus {
                SPARKY_LOG_INFO("Attacking player!");
                return NodeStatus::SUCCESS;
            }
        );
        
        // Create an action to patrol the area
        auto patrolAction = std::make_unique<ActionNode>(
            [](float deltaTime) -> NodeStatus {
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
        behaviorTree->setRootNode(std::move(rootSelector));
        
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