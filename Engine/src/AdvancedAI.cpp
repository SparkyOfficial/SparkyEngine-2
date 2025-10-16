#include "../include/AdvancedAI.h"
#include "../include/GameObject.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <cmath>
#include <limits>

namespace Sparky {
    // PerceptionComponent implementation
    PerceptionComponent::PerceptionComponent()
        : m_visionRange(20.0f)
        , m_hearingRange(30.0f)
        , m_fieldOfView(90.0f)
        , m_primaryThreat(nullptr) {
    }
    
    void PerceptionComponent::initialize() {
        // Initialize perception system
    }
    
    void PerceptionComponent::update(float deltaTime) {
        // Update perception system
        // In a real implementation, this would process stimuli and update threat assessment
        
        // Simple threat selection based on proximity and stimulus intensity
        GameObject* bestThreat = nullptr;
        float bestThreatScore = 0.0f;
        
        for (const auto& stimulus : m_stimuli) {
            if (stimulus.source && owner) {
                glm::vec3 ownerPos = owner->getPosition();
                float distance = glm::distance(ownerPos, stimulus.position);
                float score = stimulus.intensity / (distance + 1.0f);
                
                if (score > bestThreatScore) {
                    bestThreatScore = score;
                    bestThreat = stimulus.source;
                }
            }
        }
        
        m_primaryThreat = bestThreat;
    }
    
    void PerceptionComponent::destroy() {
        // Cleanup perception system
    }
    
    void PerceptionComponent::render() {
        // Perception system doesn't render directly
    }
    
    void PerceptionComponent::setVisionRange(float range) {
        m_visionRange = range;
    }
    
    float PerceptionComponent::getVisionRange() const {
        return m_visionRange;
    }
    
    void PerceptionComponent::setHearingRange(float range) {
        m_hearingRange = range;
    }
    
    float PerceptionComponent::getHearingRange() const {
        return m_hearingRange;
    }
    
    void PerceptionComponent::setFieldOfView(float fov) {
        m_fieldOfView = fov;
    }
    
    float PerceptionComponent::getFieldOfView() const {
        return m_fieldOfView;
    }
    
    void PerceptionComponent::addStimulus(const Stimulus& stimulus) {
        m_stimuli.push_back(stimulus);
    }
    
    std::vector<Stimulus> PerceptionComponent::getStimuli() const {
        return m_stimuli;
    }
    
    void PerceptionComponent::clearStimuli() {
        m_stimuli.clear();
    }
    
    bool PerceptionComponent::canSee(const GameObject* target) const {
        if (!owner || !target) return false;
        
        glm::vec3 ownerPos = owner->getPosition();
        glm::vec3 targetPos = target->getPosition();
        
        float distance = glm::distance(ownerPos, targetPos);
        if (distance > m_visionRange) return false;
        
        // Simple line-of-sight check (in a real implementation, this would use raycasting)
        glm::vec3 direction = glm::normalize(targetPos - ownerPos);
        
        // Check if target is within field of view
        // This is a simplified implementation
        return true;
    }
    
    bool PerceptionComponent::canHear(const GameObject* source) const {
        if (!owner || !source) return false;
        
        glm::vec3 ownerPos = owner->getPosition();
        glm::vec3 sourcePos = source->getPosition();
        
        float distance = glm::distance(ownerPos, sourcePos);
        return distance <= m_hearingRange;
    }
    
    GameObject* PerceptionComponent::getPrimaryThreat() const {
        return m_primaryThreat;
    }
    
    // NavigationMesh implementation
    NavigationMesh::NavigationMesh() {
    }
    
    NavigationMesh::~NavigationMesh() {
    }
    
    void NavigationMesh::addNode(const NavNode& node) {
        m_nodes.push_back(node);
    }
    
    void NavigationMesh::addConnection(int from, int to) {
        if (from >= 0 && from < static_cast<int>(m_nodes.size()) &&
            to >= 0 && to < static_cast<int>(m_nodes.size())) {
            m_connections[from].push_back(to);
        }
    }
    
    void NavigationMesh::removeNode(int nodeId) {
        if (nodeId >= 0 && nodeId < static_cast<int>(m_nodes.size())) {
            m_nodes.erase(m_nodes.begin() + nodeId);
            
            // Update connections
            std::unordered_map<int, std::vector<int>> newConnections;
            for (const auto& pair : m_connections) {
                int oldId = pair.first;
                int newId = oldId > nodeId ? oldId - 1 : oldId;
                
                if (oldId != nodeId) {
                    std::vector<int> newConnectionsList;
                    for (int connectedId : pair.second) {
                        if (connectedId != nodeId) {
                            int newConnectedId = connectedId > nodeId ? connectedId - 1 : connectedId;
                            newConnectionsList.push_back(newConnectedId);
                        }
                    }
                    newConnections[newId] = newConnectionsList;
                }
            }
            m_connections = newConnections;
        }
    }
    
    void NavigationMesh::clear() {
        m_nodes.clear();
        m_connections.clear();
    }
    
    std::vector<glm::vec3> NavigationMesh::findPath(const glm::vec3& start, const glm::vec3& end) const {
        // Simplified A* implementation
        // In a real implementation, this would be much more sophisticated
        
        std::vector<glm::vec3> path;
        
        // Find closest nodes to start and end positions
        int startNode = -1;
        int endNode = -1;
        float minStartDistance = std::numeric_limits<float>::max();
        float minEndDistance = std::numeric_limits<float>::max();
        
        for (size_t i = 0; i < m_nodes.size(); ++i) {
            float startDistance = glm::distance(start, m_nodes[i].position);
            float endDistance = glm::distance(end, m_nodes[i].position);
            
            if (startDistance < minStartDistance) {
                minStartDistance = startDistance;
                startNode = static_cast<int>(i);
            }
            
            if (endDistance < minEndDistance) {
                minEndDistance = endDistance;
                endNode = static_cast<int>(i);
            }
        }
        
        if (startNode == -1 || endNode == -1) {
            // No valid nodes found, return direct path
            path.push_back(start);
            path.push_back(end);
            return path;
        }
        
        // Simple path - just connect start to end node
        // In a real implementation, this would use A* pathfinding
        path.push_back(start);
        path.push_back(m_nodes[endNode].position);
        
        return path;
    }
    
    glm::vec3 NavigationMesh::getClosestNodePosition(const glm::vec3& position) const {
        if (m_nodes.empty()) return position;
        
        glm::vec3 closest = m_nodes[0].position;
        float minDistance = glm::distance(position, closest);
        
        for (const auto& node : m_nodes) {
            float distance = glm::distance(position, node.position);
            if (distance < minDistance) {
                minDistance = distance;
                closest = node.position;
            }
        }
        
        return closest;
    }
    
    bool NavigationMesh::isPositionWalkable(const glm::vec3& position) const {
        // Simplified check
        // In a real implementation, this would check against the navigation mesh
        return true;
    }
    
    float NavigationMesh::getDistance(int from, int to) const {
        if (from >= 0 && from < static_cast<int>(m_nodes.size()) &&
            to >= 0 && to < static_cast<int>(m_nodes.size())) {
            return glm::distance(m_nodes[from].position, m_nodes[to].position);
        }
        return std::numeric_limits<float>::max();
    }
    
    std::vector<glm::vec3> NavigationMesh::reconstructPath(int start, int end, const std::unordered_map<int, int>& cameFrom) const {
        std::vector<glm::vec3> path;
        int current = end;
        
        while (current != start) {
            path.push_back(m_nodes[current].position);
            auto it = cameFrom.find(current);
            if (it == cameFrom.end()) break;
            current = it->second;
        }
        
        path.push_back(m_nodes[start].position);
        std::reverse(path.begin(), path.end());
        
        return path;
    }
    
    float NavigationMesh::heuristic(int from, int to) const {
        return glm::distance(m_nodes[from].position, m_nodes[to].position);
    }
    
    // AdvancedAI implementation
    AdvancedAI::AdvancedAI()
        : m_difficulty(1.0f)
        , m_aggression(0.5f)
        , m_tacticalAwareness(0.5f)
        , m_navMesh(nullptr)
        , m_currentPathIndex(0)
        , m_isMoving(false)
        , m_currentTarget(nullptr)
        , m_inCombat(false)
        , m_takingCover(false)
        , m_lastAttackTime(0.0f)
        , m_attackCooldown(1.0f)
        , m_groupLeader(nullptr) {
    }
    
    void AdvancedAI::initialize() {
        // Get required components
        m_perception = owner->getComponent<PerceptionComponent>();
        m_characterController = owner->getComponent<CharacterController>();
        
        // If perception component doesn't exist, create one
        if (!m_perception) {
            m_perception = owner->addComponent<PerceptionComponent>();
        }
        
        // If character controller doesn't exist, create one
        if (!m_characterController) {
            m_characterController = owner->addComponent<CharacterController>();
        }
    }
    
    void AdvancedAI::update(float deltaTime) {
        // Update all AI subsystems
        updatePerception(deltaTime);
        updateMovement(deltaTime);
        updateCombat(deltaTime);
        updateGroupBehavior(deltaTime);
    }
    
    void AdvancedAI::destroy() {
        // Cleanup AI system
    }
    
    void AdvancedAI::render() {
        // AI system doesn't render directly
    }
    
    void AdvancedAI::setDifficulty(float difficulty) {
        m_difficulty = difficulty;
    }
    
    float AdvancedAI::getDifficulty() const {
        return m_difficulty;
    }
    
    void AdvancedAI::setAggression(float aggression) {
        m_aggression = aggression;
    }
    
    float AdvancedAI::getAggression() const {
        return m_aggression;
    }
    
    void AdvancedAI::setTacticalAwareness(float awareness) {
        m_tacticalAwareness = awareness;
    }
    
    float AdvancedAI::getTacticalAwareness() const {
        return m_tacticalAwareness;
    }
    
    void AdvancedAI::setNavigationMesh(NavigationMesh* navMesh) {
        m_navMesh = navMesh;
    }
    
    void AdvancedAI::moveTo(const glm::vec3& target) {
        if (m_navMesh) {
            glm::vec3 currentPosition = owner->getPosition();
            m_currentPath = m_navMesh->findPath(currentPosition, target);
            m_currentPathIndex = 0;
            m_targetPosition = target;
            m_isMoving = !m_currentPath.empty();
        } else {
            // Direct movement without navigation mesh
            m_targetPosition = target;
            m_isMoving = true;
        }
    }
    
    void AdvancedAI::stopMovement() {
        m_isMoving = false;
        m_currentPath.clear();
        m_currentPathIndex = 0;
        
        if (m_characterController) {
            m_characterController->move(glm::vec3(0.0f, 0.0f, 0.0f));
        }
    }
    
    void AdvancedAI::attack(GameObject* target) {
        m_currentTarget = target;
        m_inCombat = true;
    }
    
    void AdvancedAI::takeCover() {
        m_takingCover = true;
        m_inCombat = true;
        
        // Find cover position
        glm::vec3 coverPosition = findCoverPosition();
        moveTo(coverPosition);
    }
    
    void AdvancedAI::flank(GameObject* target) {
        if (!target) return;
        
        // Find flanking position
        glm::vec3 flankPosition = findFlankPosition();
        moveTo(flankPosition);
    }
    
    void AdvancedAI::retreat() {
        // Move away from current position
        if (owner) {
            glm::vec3 currentPosition = owner->getPosition();
            glm::vec3 retreatDirection(0.0f, 0.0f, 1.0f); // Default retreat direction
            
            if (m_currentTarget) {
                // Retreat away from target
                glm::vec3 targetPosition = m_currentTarget->getPosition();
                retreatDirection = glm::normalize(currentPosition - targetPosition);
            }
            
            glm::vec3 retreatPosition = currentPosition + retreatDirection * 10.0f;
            moveTo(retreatPosition);
        }
    }
    
    void AdvancedAI::setGroupLeader(AdvancedAI* leader) {
        m_groupLeader = leader;
    }
    
    void AdvancedAI::addGroupMember(AdvancedAI* member) {
        if (member && member != this) {
            // Check if already in group
            if (std::find(m_groupMembers.begin(), m_groupMembers.end(), member) == m_groupMembers.end()) {
                m_groupMembers.push_back(member);
            }
        }
    }
    
    void AdvancedAI::removeGroupMember(AdvancedAI* member) {
        if (member) {
            m_groupMembers.erase(
                std::remove(m_groupMembers.begin(), m_groupMembers.end(), member),
                m_groupMembers.end()
            );
        }
    }
    
    std::vector<AdvancedAI*> AdvancedAI::getGroupMembers() const {
        return m_groupMembers;
    }
    
    bool AdvancedAI::isMoving() const {
        return m_isMoving;
    }
    
    bool AdvancedAI::isInCombat() const {
        return m_inCombat;
    }
    
    bool AdvancedAI::isTakingCover() const {
        return m_takingCover;
    }
    
    GameObject* AdvancedAI::getCurrentTarget() const {
        return m_currentTarget;
    }
    
    void AdvancedAI::updatePerception(float deltaTime) {
        if (m_perception) {
            m_perception->update(deltaTime);
        }
    }
    
    void AdvancedAI::updateMovement(float deltaTime) {
        if (!m_isMoving || !m_characterController) return;
        
        glm::vec3 currentPosition = owner->getPosition();
        
        if (!m_currentPath.empty() && m_currentPathIndex < m_currentPath.size()) {
            glm::vec3 targetPosition = m_currentPath[m_currentPathIndex];
            
            // Check if we've reached the current path node
            float distanceToNode = glm::distance(currentPosition, targetPosition);
            if (distanceToNode < 0.5f) {
                // Move to next node
                m_currentPathIndex++;
                
                if (m_currentPathIndex >= m_currentPath.size()) {
                    // Reached the end of the path
                    m_isMoving = false;
                    m_characterController->move(glm::vec3(0.0f, 0.0f, 0.0f));
                    return;
                }
                
                targetPosition = m_currentPath[m_currentPathIndex];
            }
            
            // Calculate movement direction
            glm::vec3 direction = glm::normalize(targetPosition - currentPosition);
            m_characterController->move(direction);
        } else {
            // Direct movement to target position
            glm::vec3 direction = glm::normalize(m_targetPosition - currentPosition);
            float distance = glm::distance(currentPosition, m_targetPosition);
            
            if (distance < 0.5f) {
                // Reached target
                m_isMoving = false;
                m_characterController->move(glm::vec3(0.0f, 0.0f, 0.0f));
            } else {
                m_characterController->move(direction);
            }
        }
    }
    
    void AdvancedAI::updateCombat(float deltaTime) {
        if (!m_inCombat) return;
        
        m_lastAttackTime += deltaTime;
        
        // Select best target if we don't have one
        if (!m_currentTarget) {
            m_currentTarget = selectBestTarget();
        }
        
        // Make tactical decisions based on combat state
        makeTacticalDecision();
    }
    
    void AdvancedAI::updateGroupBehavior(float deltaTime) {
        // If we're the group leader, coordinate group behavior
        if (m_groupLeader == nullptr && !m_groupMembers.empty()) {
            // This is a group leader
            // Coordinate group members based on tactical situation
        }
        
        // If we're a group member, follow group leader's decisions
        if (m_groupLeader) {
            // Follow group leader's tactical decisions
        }
    }
    
    void AdvancedAI::makeTacticalDecision() {
        // Simple tactical decision making
        // In a real implementation, this would be much more sophisticated
        
        if (m_currentTarget && owner) {
            glm::vec3 ownerPos = owner->getPosition();
            glm::vec3 targetPos = m_currentTarget->getPosition();
            float distance = glm::distance(ownerPos, targetPos);
            
            // Decide based on aggression and tactical awareness
            if (m_aggression > 0.7f && distance > 3.0f) {
                // Aggressive - move toward target
                moveTo(targetPos);
            } else if (m_tacticalAwareness > 0.7f && distance < 2.0f) {
                // Tactical - take cover or flank
                if (m_takingCover) {
                    // Continue taking cover
                } else {
                    // Decide between cover and flanking
                    if (rand() % 2 == 0) {
                        takeCover();
                    } else {
                        flank(m_currentTarget);
                    }
                }
            }
        }
    }
    
    GameObject* AdvancedAI::selectBestTarget() {
        if (m_perception) {
            return m_perception->getPrimaryThreat();
        }
        return nullptr;
    }
    
    glm::vec3 AdvancedAI::findCoverPosition() {
        // Simplified cover finding
        // In a real implementation, this would analyze the environment for cover
        
        if (owner && m_currentTarget) {
            glm::vec3 ownerPos = owner->getPosition();
            glm::vec3 targetPos = m_currentTarget->getPosition();
            
            // Move perpendicular to the line between owner and target
            glm::vec3 direction = glm::normalize(targetPos - ownerPos);
            glm::vec3 perpendicular(-direction.z, 0.0f, direction.x);
            
            return ownerPos + perpendicular * 3.0f;
        }
        
        return owner ? owner->getPosition() : glm::vec3(0.0f, 0.0f, 0.0f);
    }
    
    glm::vec3 AdvancedAI::findFlankPosition() {
        // Simplified flanking position calculation
        // In a real implementation, this would find optimal flanking positions
        
        if (owner && m_currentTarget) {
            glm::vec3 ownerPos = owner->getPosition();
            glm::vec3 targetPos = m_currentTarget->getPosition();
            
            // Move to the side of the target
            glm::vec3 direction = glm::normalize(targetPos - ownerPos);
            glm::vec3 flankDirection(-direction.z, 0.0f, direction.x);
            
            return targetPos + flankDirection * 5.0f;
        }
        
        return owner ? owner->getPosition() : glm::vec3(0.0f, 0.0f, 0.0f);
    }
}