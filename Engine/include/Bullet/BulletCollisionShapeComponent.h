/*
 * BulletCollisionShapeComponent.h
 * 
 * Author: Андрій Будильников
 * 
 * Bullet Physics collision shape component for Sparky Engine
 * Wraps Bullet Physics collision shapes for use with GameObjects
 */

#pragma once

#include "../Component.h"
#include <glm/glm.hpp>
#include <memory>

// Forward declarations for Bullet Physics
class btCollisionShape;

namespace Sparky {
    namespace Bullet {
        enum class CollisionShapeType {
            BOX,
            SPHERE,
            CAPSULE,
            CYLINDER,
            CONE,
            STATIC_PLANE,
            MESH,
            CONVEX_HULL
        };

        class BulletCollisionShapeComponent : public Component {
        public:
            BulletCollisionShapeComponent();
            virtual ~BulletCollisionShapeComponent();

            void update(float deltaTime) override;
            void render() override;

            // Shape creation
            void createBoxShape(const glm::vec3& halfExtents);
            void createSphereShape(float radius);
            void createCapsuleShape(float radius, float height);
            void createCylinderShape(const glm::vec3& halfExtents);
            void createConeShape(float radius, float height);
            void createStaticPlaneShape(const glm::vec3& normal, float constant);
            
            // For mesh shapes
            void createMeshShape(const std::vector<glm::vec3>& vertices, const std::vector<unsigned int>& indices);
            void createConvexHullShape(const std::vector<glm::vec3>& points);

            // Shape properties
            CollisionShapeType getShapeType() const { return shapeType; }
            btCollisionShape* getCollisionShape() const { return collisionShape.get(); }
            
            // Shape transformation
            void setLocalScaling(const glm::vec3& scaling);
            glm::vec3 getLocalScaling() const;

        private:
            CollisionShapeType shapeType;
            std::unique_ptr<btCollisionShape> collisionShape;
            
            // For mesh shapes
            std::vector<glm::vec3> vertices;
            std::vector<unsigned int> indices;
            std::vector<glm::vec3> points;
        };
    }
}