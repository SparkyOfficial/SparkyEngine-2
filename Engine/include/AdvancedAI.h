#pragma once

#include "AIComponent.h"
#include "CharacterController.h"
#include "GameObject.h"
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <queue>
#include <unordered_map>

namespace Sparky {
    /**
     * @brief Advanced AI System
     * 
     * Implements sophisticated AI behaviors including:
     * - Navigation mesh pathfinding
     * - Advanced perception system (vision, hearing)
     * - Group coordination and tactics
     * - Adaptive difficulty
     * - Complex behavior trees
     */
    
    // Forward declarations
    class PerceptionComponent;
    class NavigationMesh;
    class BehaviorTree;
    
    // Perception types
    enum class PerceptionType {
        SIGHT,
        HEARING,
        SMELL
    };
    
    // Stimulus structure
    struct Stimulus {
        GameObject* source;
        PerceptionType type;
        glm::vec3 position;
        float intensity;
        float timestamp;
    };
    
    // Navigation node
    struct NavNode {
        glm::vec3 position;
        std::vector<int> connections;
        float cost;
        bool walkable;
    };
    
    /**
     * @brief Perception Component
     * Handles AI sensory input
     */
    class PerceptionComponent : public Component {
    public:
        PerceptionComponent();
        virtual ~PerceptionComponent() = default;
        
        // Component interface
        virtual void initialize();
        virtual void update(float deltaTime) override;
        virtual void destroy();
        virtual void render() override;
        
        // Perception settings
        void setVisionRange(float range);
        float getVisionRange() const;
        
        void setHearingRange(float range);
        float getHearingRange() const;
        
        void setFieldOfView(float fov);
        float getFieldOfView() const;
        
        // Stimulus handling
        void addStimulus(const Stimulus& stimulus);
        std::vector<Stimulus> getStimuli() const;
        void clearStimuli();
        
        // Perception queries
        bool canSee(const GameObject* target) const;
        bool canHear(const GameObject* source) const;
        GameObject* getPrimaryThreat() const;
        
    private:
        float m_visionRange;
        float m_hearingRange;
        float m_fieldOfView;
        std::vector<Stimulus> m_stimuli;
        GameObject* m_primaryThreat;
    };
    
    /**
     * @brief Navigation Mesh
     * Provides pathfinding capabilities for AI
     */
    class NavigationMesh {
    public:
        NavigationMesh();
        ~NavigationMesh();
        
        // Navigation mesh management
        void addNode(const NavNode& node);
        void addConnection(int from, int to);
        void removeNode(int nodeId);
        void clear();
        
        // Pathfinding
        std::vector<glm::vec3> findPath(const glm::vec3& start, const glm::vec3& end) const;
        glm::vec3 getClosestNodePosition(const glm::vec3& position) const;
        
        // Navigation queries
        bool isPositionWalkable(const glm::vec3& position) const;
        float getDistance(int from, int to) const;
        
    private:
        std::vector<NavNode> m_nodes;
        std::unordered_map<int, std::vector<int>> m_connections;
        
        // A* pathfinding helper
        struct Node {
            int id;
            float gCost;
            float hCost;
            int parent;
            
            float getFCost() const { return gCost + hCost; }
        };
        
        std::vector<glm::vec3> reconstructPath(int start, int end, const std::unordered_map<int, int>& cameFrom) const;
        float heuristic(int from, int to) const;
    };
    
    /**
     * @brief Advanced AI Component
     * Main AI controller that integrates perception, navigation, and behavior
     */
    class AdvancedAI : public AIComponent {
    public:
        AdvancedAI();
        virtual ~AdvancedAI() = default;
        
        // Component interface
        virtual void initialize();
        virtual void update(float deltaTime) override;
        virtual void destroy();
        virtual void render() override;
        
        // AI configuration
        void setDifficulty(float difficulty);
        float getDifficulty() const;
        
        void setAggression(float aggression);
        float getAggression() const;
        
        void setTacticalAwareness(float awareness);
        float getTacticalAwareness() const;
        
        // Navigation
        void setNavigationMesh(NavigationMesh* navMesh);
        void moveTo(const glm::vec3& target);
        void stopMovement();
        
        // Combat behaviors
        void attack(GameObject* target);
        void takeCover();
        void flank(GameObject* target);
        void retreat();
        
        // Group coordination
        void setGroupLeader(AdvancedAI* leader);
        void addGroupMember(AdvancedAI* member);
        void removeGroupMember(AdvancedAI* member);
        std::vector<AdvancedAI*> getGroupMembers() const;
        
        // State queries
        bool isMoving() const;
        bool isInCombat() const;
        bool isTakingCover() const;
        GameObject* getCurrentTarget() const;
        
    private:
        // AI properties
        float m_difficulty;
        float m_aggression;
        float m_tacticalAwareness;
        
        // Navigation
        NavigationMesh* m_navMesh;
        std::vector<glm::vec3> m_currentPath;
        size_t m_currentPathIndex;
        glm::vec3 m_targetPosition;
        bool m_isMoving;
        
        // Combat state
        GameObject* m_currentTarget;
        bool m_inCombat;
        bool m_takingCover;
        float m_lastAttackTime;
        float m_attackCooldown;
        
        // Group coordination
        AdvancedAI* m_groupLeader;
        std::vector<AdvancedAI*> m_groupMembers;
        
        // Components
        PerceptionComponent* m_perception;
        CharacterController* m_characterController;
        
        // Internal methods
        void updatePerception(float deltaTime);
        void updateMovement(float deltaTime);
        void updateCombat(float deltaTime);
        void updateGroupBehavior(float deltaTime);
        
        // Decision making
        void makeTacticalDecision();
        GameObject* selectBestTarget();
        glm::vec3 findCoverPosition();
        glm::vec3 findFlankPosition();
    };
}