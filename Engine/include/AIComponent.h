#pragma once

#include "Component.h"
#include <glm/glm.hpp>
#include <vector>
#include <memory>

namespace Sparky {
    class GameObject;
    
    // Forward declarations for behavior tree
    class BehaviorNode;
    class BehaviorTree;
    
    enum class AIState {
        IDLE,
        PATROL,
        CHASE,
        ATTACK,
        FLEE,
        DEAD
    };
    
    class AIComponent : public Component {
    public:
        AIComponent();
        ~AIComponent();

        void update(float deltaTime) override;
        void render() override;

        // AI state management
        void setState(AIState state);
        AIState getState() const { return currentState; }
        
        // Target management
        void setTarget(GameObject* target);
        GameObject* getTarget() const { return target; }
        
        // Patrol points
        void addPatrolPoint(const glm::vec3& point);
        void clearPatrolPoints();
        const std::vector<glm::vec3>& getPatrolPoints() const { return patrolPoints; }
        
        // Movement properties
        void setMoveSpeed(float speed) { moveSpeed = speed; }
        float getMoveSpeed() const { return moveSpeed; }
        
        // Detection properties
        void setDetectionRange(float range) { detectionRange = range; }
        float getDetectionRange() const { return detectionRange; }
        
        void setAttackRange(float range) { attackRange = range; }
        float getAttackRange() const { return attackRange; }
        
        // Combat properties
        void setAttackDamage(float damage) { attackDamage = damage; }
        float getAttackDamage() const { return attackDamage; }
        
        void setAttackRate(float rate) { attackRate = rate; }
        float getAttackRate() const { return attackRate; }
        
        // Behavior tree support
        void setBehaviorTree(std::unique_ptr<BehaviorTree> tree);
        BehaviorTree* getBehaviorTree() const { return behaviorTree.get(); }
        
        // Additional AI methods for more advanced behavior
        void fleeFromTarget();
        void setAggressive(bool aggressive);
        bool isAggressive() const;
        void wander();

    private:
        AIState currentState;
        GameObject* target;
        std::vector<glm::vec3> patrolPoints;
        size_t currentPatrolIndex;
        
        // Movement properties
        float moveSpeed;
        
        // Detection properties
        float detectionRange;
        float attackRange;
        
        // Combat properties
        float attackDamage;
        float attackRate;
        float lastAttackTime;
        
        // Behavior tree
        std::unique_ptr<BehaviorTree> behaviorTree;
        
        // Private helper methods
        void updateIdle(float deltaTime);
        void updatePatrol(float deltaTime);
        void updateChase(float deltaTime);
        void updateAttack(float deltaTime);
        void updateFlee(float deltaTime);
        
        float distanceToTarget() const;
        bool canAttack() const;
        void performAttack();
    };
}