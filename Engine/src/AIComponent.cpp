#include "../include/AIComponent.h"
#include "../include/GameObject.h"
#include "../include/PhysicsComponent.h"
#include "../include/Logger.h"
#include "../include/HealthComponent.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <random>

namespace Sparky {

    AIComponent::AIComponent() : Component(), currentState(AIState::IDLE), target(nullptr),
                               currentPatrolIndex(0), moveSpeed(2.0f), detectionRange(10.0f),
                               attackRange(2.0f), attackDamage(10.0f), attackRate(1.0f),
                               lastAttackTime(0.0f) {
        SPARKY_LOG_DEBUG("AIComponent created");
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
            case AIState::DEAD:
                // Do nothing
                break;
        }
        
        // If we have a behavior tree, update it
        if (behaviorTree) {
            behaviorTree->update(deltaTime);
        }
    }

    void AIComponent::render() {
        // AI components don't render anything directly
    }

    void AIComponent::setState(AIState state) {
        if (currentState != state) {
            SPARKY_LOG_DEBUG("AIComponent state changed from " + std::to_string(static_cast<int>(currentState)) + 
                           " to " + std::to_string(static_cast<int>(state)));
            currentState = state;
        }
    }

    void AIComponent::setTarget(GameObject* target) {
        this->target = target;
    }

    void AIComponent::addPatrolPoint(const glm::vec3& point) {
        patrolPoints.push_back(point);
    }

    void AIComponent::clearPatrolPoints() {
        patrolPoints.clear();
        currentPatrolIndex = 0;
    }

    void AIComponent::setBehaviorTree(std::unique_ptr<BehaviorTree> tree) {
        behaviorTree = std::move(tree);
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
        
        // Move towards the current patrol point
        glm::vec3 targetPoint = patrolPoints[currentPatrolIndex];
        glm::vec3 currentPosition = owner->getPosition();
        glm::vec3 direction = glm::normalize(targetPoint - currentPosition);
        
        // Check if we've reached the patrol point
        if (glm::distance(currentPosition, targetPoint) < 0.5f) {
            // Move to the next patrol point
            currentPatrolIndex = (currentPatrolIndex + 1) % patrolPoints.size();
            return;
        }
        
        // Apply movement
        PhysicsComponent* physics = owner->getComponent<PhysicsComponent>();
        if (physics) {
            glm::vec3 velocity = direction * moveSpeed;
            velocity.y = physics->getVelocity().y; // Preserve vertical velocity
            physics->setVelocity(velocity);
        }
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
        
        // Move towards target
        glm::vec3 targetPos = target->getPosition();
        glm::vec3 currentPos = owner->getPosition();
        glm::vec3 direction = glm::normalize(targetPos - currentPos);
        
        // Apply movement
        PhysicsComponent* physics = owner->getComponent<PhysicsComponent>();
        if (physics) {
            glm::vec3 velocity = direction * moveSpeed;
            velocity.y = physics->getVelocity().y; // Preserve vertical velocity
            physics->setVelocity(velocity);
        }
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
        
        // Move away from target
        glm::vec3 targetPos = target->getPosition();
        glm::vec3 currentPos = owner->getPosition();
        glm::vec3 direction = glm::normalize(currentPos - targetPos);
        
        // Apply movement
        PhysicsComponent* physics = owner->getComponent<PhysicsComponent>();
        if (physics) {
            glm::vec3 velocity = direction * moveSpeed;
            velocity.y = physics->getVelocity().y; // Preserve vertical velocity
            physics->setVelocity(velocity);
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

    float AIComponent::distanceToTarget() const {
        if (!target || !owner) {
            return std::numeric_limits<float>::max();
        }
        
        glm::vec3 targetPos = target->getPosition();
        glm::vec3 currentPos = owner->getPosition();
        return glm::distance(targetPos, currentPos);
    }

    bool AIComponent::canAttack() const {
        float currentTime = static_cast<float>(glfwGetTime());
        return (currentTime - lastAttackTime) >= (1.0f / attackRate);
    }

    void AIComponent::performAttack() {
        if (!target) {
            return;
        }
        
        lastAttackTime = static_cast<float>(glfwGetTime());
        SPARKY_LOG_DEBUG("AIComponent performing attack for " + std::to_string(attackDamage) + " damage");
        
        // Find the target's health component and apply damage
        HealthComponent* targetHealth = target->getComponent<HealthComponent>();
        if (targetHealth) {
            targetHealth->takeDamage(attackDamage);
        }
    }
    
    // Additional AI methods for more advanced behavior
    void AIComponent::fleeFromTarget() {
        setState(AIState::FLEE);
    }
    
    void AIComponent::setAggressive(bool aggressive) {
        // In a full implementation, we would store this flag and use it
        // to determine AI behavior
        SPARKY_LOG_DEBUG("AIComponent aggression set to " + std::to_string(aggressive));
    }
    
    bool AIComponent::isAggressive() const {
        // In a full implementation, we would return the stored flag
        return true;
    }
    
    void AIComponent::wander() {
        // Simple wandering behavior
        if (!owner) return;
        
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<float> dis(-1.0f, 1.0f);
        
        // Generate a random direction
        glm::vec3 direction(dis(gen), 0.0f, dis(gen));
        direction = glm::normalize(direction);
        
        // Apply movement
        PhysicsComponent* physics = owner->getComponent<PhysicsComponent>();
        if (physics) {
            glm::vec3 velocity = direction * moveSpeed * 0.5f; // Wander at half speed
            velocity.y = physics->getVelocity().y; // Preserve vertical velocity
            physics->setVelocity(velocity);
        }
    }
}