#pragma once

#include "BehaviorTree.h"
#include <glm/glm.hpp>

namespace Sparky {
    class GameObject;

    // Example leaf nodes for a simple AI
    class MoveToTarget : public LeafNode {
    public:
        MoveToTarget(GameObject* owner, const glm::vec3& target);
        virtual ~MoveToTarget();

        BehaviorStatus update(float deltaTime) override;

    private:
        GameObject* owner;
        glm::vec3 target;
        float speed;
    };

    class Wait : public LeafNode {
    public:
        Wait(float duration);
        virtual ~Wait();

        BehaviorStatus update(float deltaTime) override;

    private:
        float duration;
        float elapsed;
    };

    class CheckDistance : public LeafNode {
    public:
        CheckDistance(GameObject* owner, const glm::vec3& target, float threshold);
        virtual ~CheckDistance();

        BehaviorStatus update(float deltaTime) override;

    private:
        GameObject* owner;
        glm::vec3 target;
        float threshold;
    };

    class PrintMessage : public LeafNode {
    public:
        PrintMessage(const std::string& message);
        virtual ~PrintMessage();

        BehaviorStatus update(float deltaTime) override;

    private:
        std::string message;
    };

    // Example behavior tree setup
    class ExampleAIBehavior {
    public:
        static std::unique_ptr<BehaviorTree> createPatrolBehavior(GameObject* owner);
        static std::unique_ptr<BehaviorTree> createChaseBehavior(GameObject* owner, const glm::vec3& target);
    };
}