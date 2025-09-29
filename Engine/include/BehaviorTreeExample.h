#pragma once

#include <memory>

namespace Sparky {
    class BehaviorTree;
    class GameObject;
    
    // Example behavior trees for different AI types
    
    // Patrol behavior tree - Enemies patrol until player is detected, then attack
    std::unique_ptr<BehaviorTree> createPatrolBehaviorTree(GameObject* enemy, GameObject* player);
    
    // Guard behavior tree - Enemies guard an area, investigate noises, and attack intruders
    std::unique_ptr<BehaviorTree> createGuardBehaviorTree(GameObject* enemy, GameObject* player);
    
    // Flee behavior tree - Enemies flee when health is low
    std::unique_ptr<BehaviorTree> createFleeBehaviorTree(GameObject* enemy, GameObject* player);
    
    // Hunt behavior tree - Enemies actively hunt for the player
    std::unique_ptr<BehaviorTree> createHuntBehaviorTree(GameObject* enemy, GameObject* player);
}