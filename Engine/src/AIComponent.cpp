#include "../include/AIComponent.h"
#include "../include/GameObject.h"
#include "../include/PhysicsComponent.h"
#include "../include/HealthComponent.h"
#include "../include/BehaviorTree.h"

#ifdef HAS_GLFW
#include <GLFW/glfw3.h>
#endif

#include <algorithm>
#include <random>
#include <limits>
#include <cmath>

namespace Sparky {

    AIComponent::AIComponent() : Component(), currentState(AIState::IDLE), target(nullptr),
                               currentPatrolIndex(0), moveSpeed(2.0f), detectionRange(10.0f),
                               attackRange(2.0f), attackDamage(10.0f), attackRate(1.0f),
                               lastAttackTime(0.0f), searchTime(0.0f), maxSearchTime(10.0f) {
        // Initialize default AI properties
        aiProperties.aggression = 0.5f;
        aiProperties.intelligence = 0.5f;
        aiProperties.perception = 0.5f;
        aiProperties.healthThreshold = 0.3f;
        aiProperties.fieldOfView = 90.0f;
        aiProperties.hearingRange = 15.0f;
        aiProperties.canCoordinate = false;
        aiProperties.groupID = -1;
        
        // Initialize last known target position
        lastKnownTargetPosition[0] = 0.0f;
        lastKnownTargetPosition[1] = 0.0f;
        lastKnownTargetPosition[2] = 0.0f;
    }

    AIComponent::~AIComponent() {
    }

    void AIComponent::update(float deltaTime) {
        // Update based on current state
        switch (currentState) {
            case AIState::IDLE:
                updateIdle(deltaTime);
                break;
            case AIState::PATROL:
                updatePatrol(deltaTime);
                break;
            case AIState::CHASE:
                updateChase(deltaTime);
                break;
            case AIState::ATTACK:
                updateAttack(deltaTime);
                break;
            case AIState::FLEE:
                updateFlee(deltaTime);
                break;
            case AIState::SEARCH:
                updateSearch(deltaTime);
                break;
            case AIState::HUNT:
                updateHunt(deltaTime);
                break;
            case AIState::TACTICAL:
                updateTactical(deltaTime);
                break;
            case AIState::COORDINATE:
                updateCoordinate(deltaTime);
                break;
            case AIState::DEAD:
                // Do nothing
                break;
        }
        
        // If we have a behavior tree, update it
        if (behaviorTree) {
            behaviorTree->update(deltaTime);
        }
        
        // Check health threshold for fleeing
        if (checkHealthThreshold() && currentState != AIState::FLEE && currentState != AIState::DEAD) {
            fleeFromTarget();
        }
    }

    void AIComponent::render() {
        // AI components don't render anything directly
    }

    void AIComponent::setState(AIState state) {
        if (currentState != state) {
            currentState = state;
            
            // Special handling for certain state transitions
            if (state == AIState::SEARCH && target) {
                // Store the last known target position
                // In a full implementation, we would get the actual target position
                lastKnownTargetPosition[0] = 0.0f;
                lastKnownTargetPosition[1] = 0.0f;
                lastKnownTargetPosition[2] = 0.0f;
            }
        }
    }

    void AIComponent::setTarget(GameObject* target) {
        this->target = target;
    }

    void AIComponent::addPatrolPoint(float x, float y, float z) {
        patrolPoints.push_back(x);
        patrolPoints.push_back(y);
        patrolPoints.push_back(z);
    }

    void AIComponent::clearPatrolPoints() {
        patrolPoints.clear();
        currentPatrolIndex = 0;
    }

    size_t AIComponent::getPatrolPointCount() const {
        return patrolPoints.size() / 3;
    }

    void AIComponent::setBehaviorTree(std::unique_ptr<BehaviorTree> tree) {
        behaviorTree = std::move(tree);
    }

    // Advanced AI methods
    void AIComponent::addTacticalPosition(float x, float y, float z, float coverQuality, float visibility, float strategicValue) {
        tacticalPositions.push_back(x);
        tacticalPositions.push_back(y);
        tacticalPositions.push_back(z);
        tacticalPositions.push_back(coverQuality);
        tacticalPositions.push_back(visibility);
        tacticalPositions.push_back(strategicValue);
    }

    void AIComponent::clearTacticalPositions() {
        tacticalPositions.clear();
    }

    size_t AIComponent::getTacticalPositionCount() const {
        return tacticalPositions.size() / 6;
    }

    void AIComponent::addGroupMember(AIComponent* member) {
        if (member && member != this) {
            // Check if member is already in the group
            auto it = std::find(groupMembers.begin(), groupMembers.end(), member);
            if (it == groupMembers.end()) {
                groupMembers.push_back(member);
            }
        }
    }

    void AIComponent::removeGroupMember(AIComponent* member) {
        if (member) {
            auto it = std::find(groupMembers.begin(), groupMembers.end(), member);
            if (it != groupMembers.end()) {
                groupMembers.erase(it);
            }
        }
    }

    size_t AIComponent::getGroupMemberCount() const {
        return groupMembers.size();
    }

    bool AIComponent::canSeeTarget() {
        if (!target || !owner) return false;
        
        // In a full implementation, we would check field of view and distance
        // For now, we'll just check distance
        return distanceToTarget() <= detectionRange;
    }

    bool AIComponent::canHearTarget() {
        if (!target || !owner) return false;
        
        // Simple hearing check based on distance
        return distanceToTarget() <= aiProperties.hearingRange;
    }

    float AIComponent::getDistanceToTarget() const {
        return distanceToTarget();
    }

    void AIComponent::updateIdle(float deltaTime) {
        // Check if we should transition to another state
        if (target && distanceToTarget() <= detectionRange) {
            setState(AIState::CHASE);
        } else if (!patrolPoints.empty()) {
            setState(AIState::PATROL);
        }
    }

    void AIComponent::updatePatrol(float deltaTime) {
        if (patrolPoints.empty()) {
            setState(AIState::IDLE);
            return;
        }
        
        // Check if we should chase the target
        if (target && distanceToTarget() <= detectionRange) {
            setState(AIState::CHASE);
            return;
        }
        
        // In a full implementation, we would move towards patrol points
        // For now, we'll just cycle through them
        currentPatrolIndex = (currentPatrolIndex + 1) % getPatrolPointCount();
    }

    void AIComponent::updateChase(float deltaTime) {
        if (!target) {
            setState(AIState::IDLE);
            return;
        }
        
        float distance = distanceToTarget();
        
        // Check if target is in attack range
        if (distance <= attackRange) {
            setState(AIState::ATTACK);
            return;
        }
        
        // Check if target is out of detection range
        if (distance > detectionRange) {
            if (!patrolPoints.empty()) {
                setState(AIState::PATROL);
            } else {
                setState(AIState::IDLE);
            }
            return;
        }
        
        // In a full implementation, we would move towards the target
    }

    void AIComponent::updateAttack(float deltaTime) {
        if (!target) {
            setState(AIState::IDLE);
            return;
        }
        
        float distance = distanceToTarget();
        
        // Check if target is out of attack range
        if (distance > attackRange + 1.0f) {
            setState(AIState::CHASE);
            return;
        }
        
        // Check if we can attack
        if (canAttack()) {
            performAttack();
        }
    }

    void AIComponent::updateFlee(float deltaTime) {
        if (!target) {
            setState(AIState::IDLE);
            return;
        }
        
        // Check if we're far enough away
        if (distanceToTarget() > detectionRange * 2.0f) {
            if (!patrolPoints.empty()) {
                setState(AIState::PATROL);
            } else {
                setState(AIState::IDLE);
            }
        }
    }

    void AIComponent::updateSearch(float deltaTime) {
        if (!target) {
            setState(AIState::IDLE);
            return;
        }
        
        // Increment search time
        searchTime += deltaTime;
        
        // If we've searched long enough, give up
        if (searchTime > maxSearchTime) {
            searchTime = 0.0f;
            if (!patrolPoints.empty()) {
                setState(AIState::PATROL);
            } else {
                setState(AIState::IDLE);
            }
            return;
        }
        
        // Occasionally check if we can see the target
        if (canSeeTarget()) {
            searchTime = 0.0f;
            setState(AIState::CHASE);
        }
    }

    void AIComponent::updateHunt(float deltaTime) {
        if (!target) {
            setState(AIState::IDLE);
            return;
        }
        
        // Hunt behavior is more aggressive and persistent
        float distance = distanceToTarget();
        
        // If target is in attack range, attack
        if (distance <= attackRange) {
            setState(AIState::ATTACK);
            return;
        }
    }

    void AIComponent::updateTactical(float deltaTime) {
        // Move to the best tactical position
        if (!tacticalPositions.empty()) {
            // In a full implementation, we would move towards the tactical position
            // For now, we'll just transition to another state
            if (target && distanceToTarget() <= detectionRange) {
                setState(AIState::CHASE);
            } else if (!patrolPoints.empty()) {
                setState(AIState::PATROL);
            } else {
                setState(AIState::IDLE);
            }
        }
    }

    void AIComponent::updateCoordinate(float deltaTime) {
        // Coordinate with group members
        if (groupMembers.empty()) {
            // No group members, return to normal behavior
            if (target && distanceToTarget() <= detectionRange) {
                setState(AIState::CHASE);
            } else if (!patrolPoints.empty()) {
                setState(AIState::PATROL);
            } else {
                setState(AIState::IDLE);
            }
            return;
        }
        
        // In a full implementation, we would coordinate with group members
        // For now, we'll just transition to another state
        if (target && distanceToTarget() <= detectionRange) {
            setState(AIState::CHASE);
        } else if (!patrolPoints.empty()) {
            setState(AIState::PATROL);
        } else {
            setState(AIState::IDLE);
        }
    }

    float AIComponent::distanceToTarget() const {
        if (!target || !owner) {
            return std::numeric_limits<float>::max();
        }
        
        // In a full implementation, we would calculate the actual distance
        // For now, we'll return a placeholder value
        return 5.0f;
    }

    bool AIComponent::canAttack() const {
#ifdef HAS_GLFW
        float currentTime = static_cast<float>(glfwGetTime());
#else
        // Fallback implementation
        static float fallbackTime = 0.0f;
        fallbackTime += 0.016f; // Assume 60 FPS
        float currentTime = fallbackTime;
#endif
        return (currentTime - lastAttackTime) >= (1.0f / attackRate);
    }

    void AIComponent::performAttack() {
        if (!target) {
            return;
        }
        
#ifdef HAS_GLFW
        lastAttackTime = static_cast<float>(glfwGetTime());
#else
        // Fallback implementation
        static float fallbackTime = 0.0f;
        fallbackTime += 0.016f; // Assume 60 FPS
        lastAttackTime = fallbackTime;
#endif
        
        // Find the target's health component and apply damage
        HealthComponent* targetHealth = target->getComponent<HealthComponent>();
        if (targetHealth) {
            targetHealth->takeDamage(attackDamage);
        }
    }
    
    bool AIComponent::checkHealthThreshold() {
        // Get our own health component
        HealthComponent* ourHealth = owner->getComponent<HealthComponent>();
        if (!ourHealth) return false;
        
        float healthPercentage = ourHealth->getHealth() / ourHealth->getMaxHealth();
        return healthPercentage <= aiProperties.healthThreshold;
    }
    
    // Additional AI methods for more advanced behavior
    void AIComponent::fleeFromTarget() {
        setState(AIState::FLEE);
    }
    
    void AIComponent::setAggressive(bool aggressive) {
        aiProperties.aggression = aggressive ? 1.0f : 0.0f;
    }
    
    bool AIComponent::isAggressive() const {
        return aiProperties.aggression > 0.5f;
    }
    
    void AIComponent::wander() {
        // Simple wandering behavior
        // In a full implementation, we would apply random movement
    }
    
    void AIComponent::searchLastKnownPosition() {
        if (target) {
            setState(AIState::SEARCH);
        }
    }
    
    void AIComponent::huntTarget() {
        if (target) {
            setState(AIState::HUNT);
        }
    }
    
    void AIComponent::moveToTacticalPosition() {
        if (!tacticalPositions.empty()) {
            setState(AIState::TACTICAL);
        }
    }
    
    void AIComponent::coordinateWithGroup() {
        if (!groupMembers.empty()) {
            setState(AIState::COORDINATE);
        }
    }
}