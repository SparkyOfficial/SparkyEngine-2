#pragma once

#include <memory>
#include <vector>
#include <functional>
#include <string>

namespace Sparky {
    enum class BehaviorStatus {
        SUCCESS,
        FAILURE,
        RUNNING
    };

    class BehaviorNode {
    public:
        BehaviorNode(const std::string& name = "BehaviorNode");
        virtual ~BehaviorNode();

        virtual BehaviorStatus update(float deltaTime) = 0;
        virtual void initialize();
        virtual void terminate(BehaviorStatus status);

        const std::string& getName() const { return name; }
        BehaviorStatus getStatus() const { return status; }

    protected:
        std::string name;
        BehaviorStatus status;
    };

    class CompositeNode : public BehaviorNode {
    public:
        CompositeNode(const std::string& name = "CompositeNode");
        virtual ~CompositeNode();

        void addChild(std::unique_ptr<BehaviorNode> child);
        const std::vector<std::unique_ptr<BehaviorNode>>& getChildren() const { return children; }

    protected:
        std::vector<std::unique_ptr<BehaviorNode>> children;
    };

    class Selector : public CompositeNode {
    public:
        Selector(const std::string& name = "Selector");
        virtual ~Selector();

        BehaviorStatus update(float deltaTime) override;

    private:
        size_t currentChild;
    };

    class Sequence : public CompositeNode {
    public:
        Sequence(const std::string& name = "Sequence");
        virtual ~Sequence();

        BehaviorStatus update(float deltaTime) override;
    };

    class DecoratorNode : public BehaviorNode {
    public:
        DecoratorNode(const std::string& name = "DecoratorNode");
        virtual ~DecoratorNode();

        void setChild(std::unique_ptr<BehaviorNode> child);
        BehaviorNode* getChild() const { return child.get(); }

    protected:
        std::unique_ptr<BehaviorNode> child;
    };

    class Inverter : public DecoratorNode {
    public:
        Inverter(const std::string& name = "Inverter");
        virtual ~Inverter();

        BehaviorStatus update(float deltaTime) override;
    };

    class LeafNode : public BehaviorNode {
    public:
        LeafNode(const std::string& name = "LeafNode");
        virtual ~LeafNode();

        using TaskFunction = std::function<BehaviorStatus(float)>;

        void setTask(TaskFunction task);

    protected:
        TaskFunction task;
    };

    class BehaviorTree {
    public:
        BehaviorTree();
        ~BehaviorTree();

        void setRoot(std::unique_ptr<BehaviorNode> root);
        BehaviorStatus update(float deltaTime);

    private:
        std::unique_ptr<BehaviorNode> root;
    };
}