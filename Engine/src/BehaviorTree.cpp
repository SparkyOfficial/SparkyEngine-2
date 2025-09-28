#include "../include/BehaviorTree.h"
#include "../include/Logger.h"

namespace Sparky {

    // BehaviorNode implementation
    BehaviorNode::BehaviorNode(const std::string& name) : 
        name(name), status(BehaviorStatus::SUCCESS) {
    }

    BehaviorNode::~BehaviorNode() {
    }

    void BehaviorNode::initialize() {
        status = BehaviorStatus::RUNNING;
    }

    void BehaviorNode::terminate(BehaviorStatus status) {
        this->status = status;
    }

    // CompositeNode implementation
    CompositeNode::CompositeNode(const std::string& name) : BehaviorNode(name) {
    }

    CompositeNode::~CompositeNode() {
    }

    void CompositeNode::addChild(std::unique_ptr<BehaviorNode> child) {
        if (child) {
            children.push_back(std::move(child));
            SPARKY_LOG_DEBUG("Added child to composite node: " + name);
        }
    }

    // Selector implementation
    Selector::Selector(const std::string& name) : CompositeNode(name), currentChild(0) {
    }

    Selector::~Selector() {
    }

    BehaviorStatus Selector::update(float deltaTime) {
        // If this is the first time, initialize
        if (status != BehaviorStatus::RUNNING) {
            initialize();
            currentChild = 0;
        }

        // Try each child until one succeeds
        while (currentChild < children.size()) {
            BehaviorStatus childStatus = children[currentChild]->update(deltaTime);

            if (childStatus == BehaviorStatus::RUNNING) {
                return BehaviorStatus::RUNNING;
            } else if (childStatus == BehaviorStatus::SUCCESS) {
                terminate(BehaviorStatus::SUCCESS);
                return BehaviorStatus::SUCCESS;
            } else {
                // Child failed, try next child
                currentChild++;
            }
        }

        // All children failed
        terminate(BehaviorStatus::FAILURE);
        return BehaviorStatus::FAILURE;
    }

    // Sequence implementation
    Sequence::Sequence(const std::string& name) : CompositeNode(name) {
    }

    Sequence::~Sequence() {
    }

    BehaviorStatus Sequence::update(float deltaTime) {
        // If this is the first time, initialize
        if (status != BehaviorStatus::RUNNING) {
            initialize();
        }

        // Execute all children in sequence
        for (auto& child : children) {
            BehaviorStatus childStatus = child->update(deltaTime);

            if (childStatus == BehaviorStatus::RUNNING) {
                return BehaviorStatus::RUNNING;
            } else if (childStatus == BehaviorStatus::FAILURE) {
                terminate(BehaviorStatus::FAILURE);
                return BehaviorStatus::FAILURE;
            }
            // If child succeeded, continue to next child
        }

        // All children succeeded
        terminate(BehaviorStatus::SUCCESS);
        return BehaviorStatus::SUCCESS;
    }

    // DecoratorNode implementation
    DecoratorNode::DecoratorNode(const std::string& name) : BehaviorNode(name) {
    }

    DecoratorNode::~DecoratorNode() {
    }

    void DecoratorNode::setChild(std::unique_ptr<BehaviorNode> child) {
        this->child = std::move(child);
        SPARKY_LOG_DEBUG("Set child for decorator node: " + name);
    }

    // Inverter implementation
    Inverter::Inverter(const std::string& name) : DecoratorNode(name) {
    }

    Inverter::~Inverter() {
    }

    BehaviorStatus Inverter::update(float deltaTime) {
        if (!child) {
            return BehaviorStatus::FAILURE;
        }

        BehaviorStatus childStatus = child->update(deltaTime);

        if (childStatus == BehaviorStatus::RUNNING) {
            return BehaviorStatus::RUNNING;
        } else if (childStatus == BehaviorStatus::SUCCESS) {
            return BehaviorStatus::FAILURE;
        } else {
            return BehaviorStatus::SUCCESS;
        }
    }

    // LeafNode implementation
    LeafNode::LeafNode(const std::string& name) : BehaviorNode(name) {
    }

    LeafNode::~LeafNode() {
    }

    void LeafNode::setTask(TaskFunction task) {
        this->task = task;
        SPARKY_LOG_DEBUG("Set task for leaf node: " + name);
    }

    // BehaviorTree implementation
    BehaviorTree::BehaviorTree() {
        SPARKY_LOG_DEBUG("Created behavior tree");
    }

    BehaviorTree::~BehaviorTree() {
    }

    void BehaviorTree::setRoot(std::unique_ptr<BehaviorNode> root) {
        this->root = std::move(root);
        SPARKY_LOG_DEBUG("Set root node for behavior tree");
    }

    BehaviorStatus BehaviorTree::update(float deltaTime) {
        if (!root) {
            return BehaviorStatus::FAILURE;
        }

        return root->update(deltaTime);
    }
}