#pragma once

#include "Component.h"
#include <vector>
#include <memory>
#include <unordered_map>

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
        DEAD,
        SEARCH,      // New state for searching last known position
        HUNT,        // New state for actively hunting
        TACTICAL,    // New state for tactical positioning
        COORDINATE   // New state for group coordination
    };
    
    // Advanced AI properties
    struct AIProperties {
        float aggression = 0.5f;        // How aggressive the AI is (0.0 to 1.0)
        float intelligence = 0.5f;      // How intelligent the AI is (0.0 to 1.0)
        float perception = 0.5f;        // How well the AI perceives its environment (0.0 to 1.0)
        float healthThreshold = 0.3f;   // Health threshold for fleeing (0.0 to 1.0)
        float fieldOfView = 90.0f;      // Field of view in degrees
        float hearingRange = 15.0f;     // How far the AI can hear
        bool canCoordinate = false;     // Whether this AI can coordinate with others
        int groupID = -1;               // Group ID for coordination
    };
    
    // Tactical position for advanced positioning
    struct TacticalPosition {
        float x, y, z;
        float coverQuality;     // How good this position is for cover (0.0 to 1.0)
        float visibility;       // How visible this position is (0.0 to 1.0)
        float strategicValue;   // Strategic value of this position (0.0 to 1.0)
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
        void addPatrolPoint(float x, float y, float z);
        void clearPatrolPoints();
        size_t getPatrolPointCount() const;
        
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
        
        // Advanced AI properties
        void setAIProperties(const AIProperties& properties) { aiProperties = properties; }
        const AIProperties& getAIProperties() const { return aiProperties; }
        
        // Tactical positioning
        void addTacticalPosition(float x, float y, float z, float coverQuality, float visibility, float strategicValue);
        void clearTacticalPositions();
        size_t getTacticalPositionCount() const;
        
        // Group coordination
        void setGroupID(int id) { aiProperties.groupID = id; }
        int getGroupID() const { return aiProperties.groupID; }
        void addGroupMember(AIComponent* member);
        void removeGroupMember(AIComponent* member);
        size_t getGroupMemberCount() const;
        
        // Perception system
        bool canSeeTarget();
        bool canHearTarget();
        float getDistanceToTarget() const;
        
        // Additional AI methods for more advanced behavior
        void fleeFromTarget();
        void setAggressive(bool aggressive);
        bool isAggressive() const;
        void wander();
        void searchLastKnownPosition();
        void huntTarget();
        void moveToTacticalPosition();
        void coordinateWithGroup();

    private:
        AIState currentState;
        GameObject* target;
        std::vector<float> patrolPoints; // Store as x,y,z triplets
        size_t currentPatrolIndex;
        
        // Advanced AI properties
        AIProperties aiProperties;
        std::vector<float> tacticalPositions; // Store as x,y,z,cover,visibility,strategicValue sextuplets
        std::vector<AIComponent*> groupMembers;
        
        // Movement properties
        float moveSpeed;
        
        // Detection properties
        float detectionRange;
        float attackRange;
        
        // Combat properties
        float attackDamage;
        float attackRate;
        float lastAttackTime;
        
        // Search behavior
        float lastKnownTargetPosition[3]; // x, y, z
        float searchTime;
        float maxSearchTime;
        
        // Behavior tree
        std::unique_ptr<BehaviorTree> behaviorTree;
        
        // Private helper methods
        void updateIdle(float deltaTime);
        void updatePatrol(float deltaTime);
        void updateChase(float deltaTime);
        void updateAttack(float deltaTime);
        void updateFlee(float deltaTime);
        void updateSearch(float deltaTime);
        void updateHunt(float deltaTime);
        void updateTactical(float deltaTime);
        void updateCoordinate(float deltaTime);
        
        float distanceToTarget() const;
        bool canAttack() const;
        void performAttack();
        bool checkHealthThreshold();
    };
}