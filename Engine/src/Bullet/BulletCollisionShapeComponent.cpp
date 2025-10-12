/*
 * BulletCollisionShapeComponent.cpp
 * 
 * Author: Андрій Будильников
 * 
 * Bullet Physics collision shape component implementation for Sparky Engine
 */

#include "../../include/Bullet/BulletCollisionShapeComponent.h"
#include <iostream>
#include <glm/glm.hpp>
#include <vector>

#ifdef HAS_BULLET
// Bullet Physics includes
#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionShapes/btBoxShape.h"
#include "BulletCollision/CollisionShapes/btSphereShape.h"
#include "BulletCollision/CollisionShapes/btCapsuleShape.h"
#include "BulletCollision/CollisionShapes/btCylinderShape.h"
#include "BulletCollision/CollisionShapes/btConeShape.h"
#include "BulletCollision/CollisionShapes/btStaticPlaneShape.h"
#include "BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h"
#include "BulletCollision/CollisionShapes/btConvexHullShape.h"
#include "LinearMath/btDefaultMotionState.h"
#endif

namespace Sparky {
    namespace Bullet {
        BulletCollisionShapeComponent::BulletCollisionShapeComponent() 
            : shapeType(CollisionShapeType::BOX) {
        }

        BulletCollisionShapeComponent::~BulletCollisionShapeComponent() {
            // The collisionShape is managed by a unique_ptr, so it will be automatically deleted
        }

        void BulletCollisionShapeComponent::update(float deltaTime) {
            // Collision shapes don't need updating
        }

        void BulletCollisionShapeComponent::render() {
            // Rendering is handled by the renderer, not the collision shape component
        }

#ifdef HAS_BULLET
        void BulletCollisionShapeComponent::createBoxShape(const glm::vec3& halfExtents) {
            shapeType = CollisionShapeType::BOX;
            collisionShape = std::make_unique<btBoxShape>(btVector3(halfExtents.x, halfExtents.y, halfExtents.z));
        }

        void BulletCollisionShapeComponent::createSphereShape(float radius) {
            shapeType = CollisionShapeType::SPHERE;
            collisionShape = std::make_unique<btSphereShape>(radius);
        }

        void BulletCollisionShapeComponent::createCapsuleShape(float radius, float height) {
            shapeType = CollisionShapeType::CAPSULE;
            collisionShape = std::make_unique<btCapsuleShape>(radius, height);
        }

        void BulletCollisionShapeComponent::createCylinderShape(const glm::vec3& halfExtents) {
            shapeType = CollisionShapeType::CYLINDER;
            collisionShape = std::make_unique<btCylinderShape>(btVector3(halfExtents.x, halfExtents.y, halfExtents.z));
        }

        void BulletCollisionShapeComponent::createConeShape(float radius, float height) {
            shapeType = CollisionShapeType::CONE;
            collisionShape = std::make_unique<btConeShape>(radius, height);
        }

        void BulletCollisionShapeComponent::createStaticPlaneShape(const glm::vec3& normal, float constant) {
            shapeType = CollisionShapeType::STATIC_PLANE;
            collisionShape = std::make_unique<btStaticPlaneShape>(btVector3(normal.x, normal.y, normal.z), constant);
        }

        void BulletCollisionShapeComponent::createMeshShape(const std::vector<glm::vec3>& vertices, const std::vector<unsigned int>& indices) {
            shapeType = CollisionShapeType::MESH;
            
            // Store the vertices and indices for reference
            this->vertices = vertices;
            this->indices = indices;
            
            // Create Bullet triangle mesh
            auto triangleMesh = std::make_unique<btTriangleMesh>();
            
            // Add triangles to the mesh
            for (size_t i = 0; i < indices.size(); i += 3) {
                if (i + 2 < indices.size()) {
                    btVector3 v0(vertices[indices[i]].x, vertices[indices[i]].y, vertices[indices[i]].z);
                    btVector3 v1(vertices[indices[i + 1]].x, vertices[indices[i + 1]].y, vertices[indices[i + 1]].z);
                    btVector3 v2(vertices[indices[i + 2]].x, vertices[indices[i + 2]].y, vertices[indices[i + 2]].z);
                    triangleMesh->addTriangle(v0, v1, v2);
                }
            }
            
            // Create the collision shape
            collisionShape = std::make_unique<btBvhTriangleMeshShape>(triangleMesh.get(), true);
            
            // Note: In a real implementation, we would need to manage the triangleMesh lifetime properly
            // For simplicity, we're not doing that here
        }

        void BulletCollisionShapeComponent::createConvexHullShape(const std::vector<glm::vec3>& points) {
            shapeType = CollisionShapeType::CONVEX_HULL;
            
            // Store the points for reference
            this->points = points;
            
            // Create the convex hull shape
            auto convexHull = std::make_unique<btConvexHullShape>();
            
            // Add points to the convex hull
            for (const auto& point : points) {
                convexHull->addPoint(btVector3(point.x, point.y, point.z));
            }
            
            collisionShape = std::move(convexHull);
        }

        void BulletCollisionShapeComponent::setLocalScaling(const glm::vec3& scaling) {
            if (collisionShape) {
                collisionShape->setLocalScaling(btVector3(scaling.x, scaling.y, scaling.z));
            }
        }

        glm::vec3 BulletCollisionShapeComponent::getLocalScaling() const {
            if (collisionShape) {
                btVector3 scale = collisionShape->getLocalScaling();
                return glm::vec3(scale.x(), scale.y(), scale.z());
            }
            return glm::vec3(1.0f);
        }
#else
        void BulletCollisionShapeComponent::createBoxShape(const glm::vec3& halfExtents) {
            // Fallback implementation when Bullet is not available
        }

        void BulletCollisionShapeComponent::createSphereShape(float radius) {
            // Fallback implementation when Bullet is not available
        }

        void BulletCollisionShapeComponent::createCapsuleShape(float radius, float height) {
            // Fallback implementation when Bullet is not available
        }

        void BulletCollisionShapeComponent::createCylinderShape(const glm::vec3& halfExtents) {
            // Fallback implementation when Bullet is not available
        }

        void BulletCollisionShapeComponent::createConeShape(float radius, float height) {
            // Fallback implementation when Bullet is not available
        }

        void BulletCollisionShapeComponent::createStaticPlaneShape(const glm::vec3& normal, float constant) {
            // Fallback implementation when Bullet is not available
        }

        void BulletCollisionShapeComponent::createMeshShape(const std::vector<glm::vec3>& vertices, const std::vector<unsigned int>& indices) {
            // Fallback implementation when Bullet is not available
        }

        void BulletCollisionShapeComponent::createConvexHullShape(const std::vector<glm::vec3>& points) {
            // Fallback implementation when Bullet is not available
        }

        void BulletCollisionShapeComponent::setLocalScaling(const glm::vec3& scaling) {
            // Fallback implementation when Bullet is not available
        }

        glm::vec3 BulletCollisionShapeComponent::getLocalScaling() const {
            return glm::vec3(1.0f);
        }
#endif
    }
}