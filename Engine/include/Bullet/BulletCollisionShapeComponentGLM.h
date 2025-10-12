/*
 * BulletCollisionShapeComponentGLM.h
 * 
 * Author: Андрій Будильников
 * 
 * GLM wrapper for Bullet Physics collision shape component for Sparky Engine
 * Provides a more convenient interface using glm types
 */

#pragma once

#include "BulletCollisionShapeComponent.h"
#include <glm/glm.hpp>
#include <vector>

namespace Sparky {
    namespace Bullet {
        class BulletCollisionShapeComponentGLM : public BulletCollisionShapeComponent {
        public:
            // Collision shape creation with glm types
            void createBoxShape(const glm::vec3& halfExtents) {
                float halfExtentsArray[3] = {halfExtents.x, halfExtents.y, halfExtents.z};
                BulletCollisionShapeComponent::createBoxShape(halfExtentsArray);
            }
            
            void createSphereShape(float radius) {
                BulletCollisionShapeComponent::createSphereShape(radius);
            }
            
            void createCapsuleShape(float radius, float height) {
                BulletCollisionShapeComponent::createCapsuleShape(radius, height);
            }
            
            void createCylinderShape(const glm::vec3& halfExtents) {
                float halfExtentsArray[3] = {halfExtents.x, halfExtents.y, halfExtents.z};
                BulletCollisionShapeComponent::createCylinderShape(halfExtentsArray);
            }
            
            void createConeShape(float radius, float height) {
                BulletCollisionShapeComponent::createConeShape(radius, height);
            }
            
            void createStaticPlaneShape(const glm::vec3& planeNormal, float planeConstant) {
                float planeNormalArray[3] = {planeNormal.x, planeNormal.y, planeNormal.z};
                BulletCollisionShapeComponent::createStaticPlaneShape(planeNormalArray, planeConstant);
            }
            
            void createConvexHullShape(const std::vector<glm::vec3>& points) {
                // Convert std::vector<glm::::vec3> to std::vector<float>
                std::vector<float> pointsArray;
                pointsArray.reserve(points.size() * 3);
                for (const auto& point : points) {
                    pointsArray.push_back(point.x);
                    pointsArray.push_back(point.y);
                    pointsArray.push_back(point.z);
                }
                BulletCollisionShapeComponent::createConvexHullShape(pointsArray);
            }
            
            void createMeshShape(const std::vector<glm::vec3>& vertices, const std::vector<unsigned int>& indices) {
                // Convert std::vector<glm::::vec3> to std::vector<float>
                std::vector<float> verticesArray;
                verticesArray.reserve(vertices.size() * 3);
                for (const auto& vertex : vertices) {
                    verticesArray.push_back(vertex.x);
                    verticesArray.push_back(vertex.y);
                    verticesArray.push_back(vertex.z);
                }
                BulletCollisionShapeComponent::createMeshShape(verticesArray, indices);
            }
        };
    }
}