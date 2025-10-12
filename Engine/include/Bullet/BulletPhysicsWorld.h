/*
 * BulletPhysicsWorld.h
 * 
 * Author: Андрій Будильников
 * 
 * Bullet Physics world for Sparky Engine
 * Integrates Bullet Physics btDiscreteDynamicsWorld with Sparky's physics system
 */

#pragma once

#include <vector>
#include <memory>

#ifdef HAS_BULLET
// Forward declarations for Bullet Physics
class btDiscreteDynamicsWorld;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btConstraintSolver;
class btDefaultCollisionConfiguration;
class btRigidBody;
class btCollisionShape;
#endif

namespace Sparky {
    namespace Bullet {
        class BulletRigidBodyComponent;
        
#ifdef HAS_BULLET
        struct BulletRaycastHit {
            bool hit;
            // We'll use float arrays instead of glm::vec3 to avoid include issues
            float point[3];
            float normal[3];
            float distance;
            BulletRigidBodyComponent* rigidBody;
        };
#else
        struct BulletRaycastHit {
            bool hit;
            float point[3];
            float normal[3];
            float distance;
            BulletRigidBodyComponent* rigidBody;
        };
#endif

        class BulletPhysicsWorld {
        public:
            static BulletPhysicsWorld& getInstance();

            void initialize();
            void cleanup();
            
            void addRigidBody(BulletRigidBodyComponent* rigidBody);
            void removeRigidBody(BulletRigidBodyComponent* rigidBody);

            void update(float deltaTime);
#ifdef HAS_BULLET
            void setGravity(const float gravity[3]);
            void getGravity(float outGravity[3]) const;
#else
            void setGravity(const float gravity[3]) {}
            void getGravity(float outGravity[3]) const { 
                outGravity[0] = outGravity[1] = outGravity[2] = 0.0f;
            }
#endif

            // Collision shapes
#ifdef HAS_BULLET
            btCollisionShape* createBoxShape(const float halfExtents[3]);
            btCollisionShape* createSphereShape(float radius);
            btCollisionShape* createCapsuleShape(float radius, float height);
            btCollisionShape* createCylinderShape(const float halfExtents[3]);
            btCollisionShape* createConeShape(float radius, float height);
            btCollisionShape* createStaticPlaneShape(const float normal[3], float constant);
#else
            void* createBoxShape(const float halfExtents[3]) { return nullptr; }
            void* createSphereShape(float radius) { return nullptr; }
            void* createCapsuleShape(float radius, float height) { return nullptr; }
            void* createCylinderShape(const float halfExtents[3]) { return nullptr; }
            void* createConeShape(float radius, float height) { return nullptr; }
            void* createStaticPlaneShape(const float normal[3], float constant) { return nullptr; }
#endif

            // Raycasting
#ifdef HAS_BULLET
            BulletRaycastHit raycast(const float origin[3], const float direction[3], float maxDistance);
#else
            BulletRaycastHit raycast(const float origin[3], const float direction[3], float maxDistance) {
                BulletRaycastHit result = { false, {0,0,0}, {0,0,0}, 0.0f, nullptr };
                return result;
            }
#endif

            // Constraints
#ifdef HAS_BULLET
            void addConstraint(class btTypedConstraint* constraint, bool disableCollisionsBetweenLinkedBodies = false);
            void removeConstraint(class btTypedConstraint* constraint);
#else
            void addConstraint(void* constraint, bool disableCollisionsBetweenLinkedBodies = false) {}
            void removeConstraint(void* constraint) {}
#endif

            // Get Bullet world
#ifdef HAS_BULLET
            btDiscreteDynamicsWorld* getWorld() const { return dynamicsWorld; }
#else
            void* getWorld() const { return nullptr; }
#endif

        private:
            BulletPhysicsWorld();
            ~BulletPhysicsWorld();

#ifdef HAS_BULLET
            btDiscreteDynamicsWorld* dynamicsWorld;
            btCollisionDispatcher* dispatcher;
            btBroadphaseInterface* broadphase;
            btConstraintSolver* solver;
            btDefaultCollisionConfiguration* collisionConfiguration;
#endif
            
            std::vector<BulletRigidBodyComponent*> rigidBodies;
        };
    }
}