#include "../include/BehaviorTreeExample.h"
#include "../include/BehaviorTree.h"
#include "../include/GameObject.h"
#include "../include/AIComponent.h"
#include "../include/Logger.h"
#include <glm/glm.hpp>

namespace Sparky {
    
    std::unique_ptr<BehaviorTree> createPatrolBehaviorTree(GameObject* enemy, GameObject* player) {
        auto tree = std::make_unique<BehaviorTree>();
        
        // Create root selector node
        auto rootSelector = std::make_unique<SelectorNode>();
        
        // Create sequence for attacking player
        auto attackSequence = std::make_unique<SequenceNode>();
        
        // Add condition: Is player in range?
        auto playerInRangeCondition = std::make_unique<ConditionNode>([enemy, player]() -> bool {
            if (!enemy || !player) return false;
            
            AIComponent* ai = enemy->getComponent<AIComponent>();
            if (!ai) return false;
            
            // Set player as target
            ai->setTarget(player);
            
            // Check if player is in detection range
            float distance = glm::distance(enemy->getPosition(), player->getPosition());
            return distance <= ai->getDetectionRange();
        });
        
        // Add action: Attack player
        auto attackAction = std::make_unique<ActionNode>([enemy](float deltaTime) -> NodeStatus {
            if (!enemy) return NodeStatus::FAILURE;
            
            AIComponent* ai = enemy->getComponent<AIComponent>();
            if (!ai) return NodeStatus::FAILURE;
            
            // Set AI state to attack
            ai->setState(AIState::ATTACK);
            
            // For simplicity, we'll just return SUCCESS
            // In a real implementation, this would depend on the attack outcome
            return NodeStatus::SUCCESS;
        });
        
        // Add condition and action to attack sequence
        attackSequence->addChild(std::move(playerInRangeCondition));
        attackSequence->addChild(std::move(attackAction));
        
        // Create sequence for patrolling
        auto patrolSequence = std::make_unique<SequenceNode>();
        
        // Add condition: Is player NOT in range?
        auto playerNotInRangeCondition = std::make_unique<ConditionNode>([enemy, player]() -> bool {
            if (!enemy || !player) return true; // If no player, just patrol
            
            AIComponent* ai = enemy->getComponent<AIComponent>();
            if (!ai) return true;
            
            // Check if player is NOT in detection range
            float distance = glm::distance(enemy->getPosition(), player->getPosition());
            return distance > ai->getDetectionRange();
        });
        
        // Add action: Patrol
        auto patrolAction = std::make_unique<ActionNode>([enemy](float deltaTime) -> NodeStatus {
            if (!enemy) return NodeStatus::FAILURE;
            
            AIComponent* ai = enemy->getComponent<AIComponent>();
            if (!ai) return NodeStatus::FAILURE;
            
            // Set AI state to patrol
            ai->setState(AIState::PATROL);
            
            // For simplicity, we'll just return SUCCESS
            return NodeStatus::SUCCESS;
        });
        
        // Add condition and action to patrol sequence
        patrolSequence->addChild(std::move(playerNotInRangeCondition));
        patrolSequence->addChild(std::move(patrolAction));
        
        // Add sequences to root selector
        rootSelector->addChild(std::move(attackSequence));
        rootSelector->addChild(std::move(patrolSequence));
        
        // Set root node of the tree
        tree->setRootNode(std::move(rootSelector));
        
        return tree;
    }
    
    std::unique_ptr<BehaviorTree> createGuardBehaviorTree(GameObject* enemy, GameObject* player) {
        auto tree = std::make_unique<BehaviorTree>();
        
        // Create root selector node
        auto rootSelector = std::make_unique<SelectorNode>();
        
        // Create sequence for investigating noise
        auto investigateSequence = std::make_unique<SequenceNode>();
        
        // Add condition: Is there a noise to investigate?
        auto noiseCondition = std::make_unique<ConditionNode>([enemy]() -> bool {
            // In a full implementation, we would check for noise events
            // For now, we'll just return false to skip this branch
            return false;
        });
        
        // Add action: Investigate noise
        auto investigateAction = std::make_unique<ActionNode>([enemy](float deltaTime) -> NodeStatus {
            if (!enemy) return NodeStatus::FAILURE;
            
            AIComponent* ai = enemy->getComponent<AIComponent>();
            if (!ai) return NodeStatus::FAILURE;
            
            // For simplicity, we'll just return FAILURE to move to next branch
            return NodeStatus::FAILURE;
        });
        
        // Add condition and action to investigate sequence
        investigateSequence->addChild(std::move(noiseCondition));
        investigateSequence->addChild(std::move(investigateAction));
        
        // Create sequence for attacking player
        auto attackSequence = std::make_unique<SequenceNode>();
        
        // Add condition: Can see player?
        auto canSeePlayerCondition = std::make_unique<ConditionNode>([enemy, player]() -> bool {
            if (!enemy || !player) return false;
            
            // Simple line-of-sight check
            glm::vec3 direction = player->getPosition() - enemy->getPosition();
            float distance = glm::length(direction);
            
            AIComponent* ai = enemy->getComponent<AIComponent>();
            if (!ai) return false;
            
            // Set player as target
            ai->setTarget(player);
            
            // Check if player is in sight range
            return distance <= ai->getDetectionRange();
        });
        
        // Add action: Attack player
        auto attackAction = std::make_unique<ActionNode>([enemy](float deltaTime) -> NodeStatus {
            if (!enemy) return NodeStatus::FAILURE;
            
            AIComponent* ai = enemy->getComponent<AIComponent>();
            if (!ai) return NodeStatus::FAILURE;
            
            // Set AI state to attack
            ai->setState(AIState::ATTACK);
            
            return NodeStatus::SUCCESS;
        });
        
        // Add condition and action to attack sequence
        attackSequence->addChild(std::move(canSeePlayerCondition));
        attackSequence->addChild(std::move(attackAction));
        
        // Create sequence for patrolling
        auto patrolSequence = std::make_unique<SequenceNode>();
        
        // Add condition: No player in sight and no noise
        auto idleCondition = std::make_unique<ConditionNode>([enemy, player]() -> bool {
            // Always true for this example
            return true;
        });
        
        // Add action: Patrol or idle
        auto idleAction = std::make_unique<ActionNode>([enemy](float deltaTime) -> NodeStatus {
            if (!enemy) return NodeStatus::FAILURE;
            
            AIComponent* ai = enemy->getComponent<AIComponent>();
            if (!ai) return NodeStatus::FAILURE;
            
            // Set AI state to patrol
            ai->setState(AIState::PATROL);
            
            return NodeStatus::SUCCESS;
        });
        
        // Add condition and action to patrol sequence
        patrolSequence->addChild(std::move(idleCondition));
        patrolSequence->addChild(std::move(idleAction));
        
        // Add sequences to root selector
        rootSelector->addChild(std::move(investigateSequence));
        rootSelector->addChild(std::move(attackSequence));
        rootSelector->addChild(std::move(patrolSequence));
        
        // Set root node of the tree
        tree->setRootNode(std::move(rootSelector));
        
        return tree;
    }
}