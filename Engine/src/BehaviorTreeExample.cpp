#include "../include/BehaviorTreeExample.h"
#include "../include/GameObject.h"
#include "../include/Logger.h"
#include <iostream>

namespace Sparky {

    // MoveToTarget implementation
    MoveToTarget::MoveToTarget(GameObject* owner, const glm::vec3& target) : 
        LeafNode("MoveToTarget"), owner(owner), target(target), speed(5.0f) {
    }

    MoveToTarget::~MoveToTarget() {
    }

    BehaviorStatus MoveToTarget::update(float deltaTime) {
        if (!owner) {
            return BehaviorStatus::FAILURE;
        }

        glm::vec3 currentPosition = owner->getPosition();
        glm::vec3 direction = target - currentPosition;
        float distance = glm::length(direction);

        // If we're close enough, consider it a success
        if (distance < 0.1f) {
            return BehaviorStatus::SUCCESS;
        }

        // Move towards target
        direction = glm::normalize(direction);
        glm::vec3 newPosition = currentPosition + direction * speed * deltaTime;
        owner->setPosition(newPosition);

        return BehaviorStatus::RUNNING;
    }

    // Wait implementation
    Wait::Wait(float duration) : LeafNode("Wait"), duration(duration), elapsed(0.0f) {
    }

    Wait::~Wait() {
    }

    BehaviorStatus Wait::update(float deltaTime) {
        elapsed += deltaTime;

        if (elapsed >= duration) {
            elapsed = 0.0f;
            return BehaviorStatus::SUCCESS;
        }

        return BehaviorStatus::RUNNING;
    }

    // CheckDistance implementation
    CheckDistance::CheckDistance(GameObject* owner, const glm::vec3& target, float threshold) : 
        LeafNode("CheckDistance"), owner(owner), target(target), threshold(threshold) {
    }

    CheckDistance::~CheckDistance() {
    }

    BehaviorStatus CheckDistance::update(float deltaTime) {
        if (!owner) {
            return BehaviorStatus::FAILURE;
        }

        glm::vec3 currentPosition = owner->getPosition();
        glm::vec3 direction = target - currentPosition;
        float distance = glm::length(direction);

        if (distance <= threshold) {
            return BehaviorStatus::SUCCESS;
        }

        return BehaviorStatus::FAILURE;
    }

    // PrintMessage implementation
    PrintMessage::PrintMessage(const std::string& message) : 
        LeafNode("PrintMessage"), message(message) {
    }

    PrintMessage::~PrintMessage() {
    }

    BehaviorStatus PrintMessage::update(float deltaTime) {
        static float lastPrintTime = 0.0f;
        static const float printInterval = 1.0f; // Print at most once per second
        
        lastPrintTime += deltaTime;
        if (lastPrintTime >= printInterval) {
            SPARKY_LOG_INFO("Behavior Tree Message: " + message);
            lastPrintTime = 0.0f;
        }
        return BehaviorStatus::SUCCESS;
    }

    // ExampleAIBehavior implementation
    std::unique_ptr<BehaviorTree> ExampleAIBehavior::createPatrolBehavior(GameObject* owner) {
        auto tree = std::make_unique<BehaviorTree>();

        // Create a sequence: Print -> Move to point A -> Wait -> Print -> Move to point B -> Wait
        auto sequence = std::make_unique<Sequence>("PatrolSequence");

        // Add print message
        auto print1 = std::make_unique<PrintMessage>("Starting patrol...");
        sequence->addChild(std::move(print1));

        // Move to point A
        auto moveA = std::make_unique<MoveToTarget>(owner, glm::vec3(10.0f, 0.0f, 0.0f));
        sequence->addChild(std::move(moveA));

        // Wait
        auto wait1 = std::make_unique<Wait>(2.0f);
        sequence->addChild(std::move(wait1));

        // Add print message
        auto print2 = std::make_unique<PrintMessage>("Reached point A, moving to point B...");
        sequence->addChild(std::move(print2));

        // Move to point B
        auto moveB = std::make_unique<MoveToTarget>(owner, glm::vec3(-10.0f, 0.0f, 0.0f));
        sequence->addChild(std::move(moveB));

        // Wait
        auto wait2 = std::make_unique<Wait>(2.0f);
        sequence->addChild(std::move(wait2));

        // Add print message
        auto print3 = std::make_unique<PrintMessage>("Reached point B, restarting patrol...");
        sequence->addChild(std::move(print3));

        tree->setRoot(std::move(sequence));
        return tree;
    }

    std::unique_ptr<BehaviorTree> ExampleAIBehavior::createChaseBehavior(GameObject* owner, const glm::vec3& target) {
        auto tree = std::make_unique<BehaviorTree>();

        // Create a sequence: Check distance -> Move to target -> Print success
        auto sequence = std::make_unique<Sequence>("ChaseSequence");

        // Check if target is close
        auto checkDistance = std::make_unique<CheckDistance>(owner, target, 5.0f);
        sequence->addChild(std::move(checkDistance));

        // Move to target
        auto moveToTarget = std::make_unique<MoveToTarget>(owner, target);
        sequence->addChild(std::move(moveToTarget));

        // Print success message
        auto printSuccess = std::make_unique<PrintMessage>("Target reached!");
        sequence->addChild(std::move(printSuccess));

        tree->setRoot(std::move(sequence));
        return tree;
    }
}