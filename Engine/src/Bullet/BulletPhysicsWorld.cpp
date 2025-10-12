#ifdef HAS_BULLET

/*
 * BulletPhysicsWorld.cpp
 * 
 * Author: Андрій Будильников
 * 
 * Bullet Physics world implementation for Sparky Engine
 */

#include "../../include/Bullet/BulletPhysicsWorld.h"
#include "../../include/Bullet/BulletRigidBodyComponent.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Bullet Physics includes
#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionDispatch/btCollisionWorld.h"
#include "LinearMath/btDefaultMotionState.h"

namespace Sparky {
    namespace Bullet {
        // Static instance
        static BulletPhysicsWorld* instance = nullptr;

        BulletPhysicsWorld::BulletPhysicsWorld() 
            : dynamicsWorld(nullptr), dispatcher(nullptr), 
              broadphase(nullptr), solver(nullptr), 
              collisionConfiguration(nullptr) {
        }

        BulletPhysicsWorld::~BulletPhysicsWorld() {
            cleanup();
        }

        BulletPhysicsWorld& BulletPhysicsWorld::getInstance() {
            if (!instance) {
                instance = new BulletPhysicsWorld();
            }
            return *instance;
        }

        void BulletPhysicsWorld::initialize() {
#ifdef HAS_BULLET
            // Create collision configuration
            collisionConfiguration = new btDefaultCollisionConfiguration();

            // Create dispatcher
            dispatcher = new btCollisionDispatcher(collisionConfiguration);

            // Create broadphase
            broadphase = new btDbvtBroadphase();

            // Create solver
            solver = new btSequentialImpulseConstraintSolver();

            // Create dynamics world
            dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
            
            // Set default gravity
            dynamicsWorld->setGravity(btVector3(0, -9.81f, 0));
#endif
        }

        void BulletPhysicsWorld::cleanup() {
#ifdef HAS_BULLET
            // Clean up in reverse order of creation
            if (dynamicsWorld) {
                delete dynamicsWorld;
                dynamicsWorld = nullptr;
            }

            if (solver) {
                delete solver;
                solver = nullptr;
            }

            if (broadphase) {
                delete broadphase;
                broadphase = nullptr;
            }

            if (dispatcher) {
                delete dispatcher;
                dispatcher = nullptr;
            }

            if (collisionConfiguration) {
                delete collisionConfiguration;
                collisionConfiguration = nullptr;
            }
#endif
        }

        void BulletPhysicsWorld::addRigidBody(BulletRigidBodyComponent* rigidBody) {
#ifdef HAS_BULLET
            if (dynamicsWorld && rigidBody) {
                dynamicsWorld->addRigidBody(rigidBody->getRigidBody());
                rigidBodies.push_back(rigidBody);
            }
#endif
        }

        void BulletPhysicsWorld::removeRigidBody(BulletRigidBodyComponent* rigidBody) {
#ifdef HAS_BULLET
            if (dynamicsWorld && rigidBody) {
                dynamicsWorld->removeRigidBody(rigidBody->getRigidBody());
                // Remove from our list
                rigidBodies.erase(
                    std::remove(rigidBodies.begin(), rigidBodies.end(), rigidBody),
                    rigidBodies.end()
                );
            }
#endif
        }

        void BulletPhysicsWorld::update(float deltaTime) {
#ifdef HAS_BULLET
            if (dynamicsWorld) {
                dynamicsWorld->stepSimulation(deltaTime, 10);
            }
#endif
        }

        void BulletPhysicsWorld::setGravity(const float gravity[3]) {
#ifdef HAS_BULLET
            if (dynamicsWorld) {
                dynamicsWorld->setGravity(btVector3(gravity[0], gravity[1], gravity[2]));
            }
#endif
        }

        void BulletPhysicsWorld::getGravity(float outGravity[3]) const {
#ifdef HAS_BULLET
            if (dynamicsWorld) {
                btVector3 gravity = dynamicsWorld->getGravity();
                outGravity[0] = gravity.x();
                outGravity[1] = gravity.y();
                outGravity[2] = gravity.z();
            } else {
                outGravity[0] = outGravity[1] = outGravity[2] = 0.0f;
            }
#else
            outGravity[0] = outGravity[1] = outGravity[2] = 0.0f;
#endif
        }

        btCollisionShape* BulletPhysicsWorld::createBoxShape(const float halfExtents[3]) {
#ifdef HAS_BULLET
            auto shape = new btBoxShape(btVector3(halfExtents[0], halfExtents[1], halfExtents[2]));
            return shape;
#else
            return nullptr;
#endif
        }

        btCollisionShape* BulletPhysicsWorld::createSphereShape(float radius) {
#ifdef HAS_BULLET
            auto shape = new btSphereShape(radius);
            return shape;
#else
            return nullptr;
#endif
        }

        btCollisionShape* BulletPhysicsWorld::createCapsuleShape(float radius, float height) {
#ifdef HAS_BULLET
            auto shape = new btCapsuleShape(radius, height);
            return shape;
#else
            return nullptr;
#endif
        }

        btCollisionShape* BulletPhysicsWorld::createCylinderShape(const float halfExtents[3]) {
#ifdef HAS_BULLET
            auto shape = new btCylinderShape(btVector3(halfExtents[0], halfExtents[1], halfExtents[2]));
            return shape;
#else
            return nullptr;
#endif
        }

        btCollisionShape* BulletPhysicsWorld::createConeShape(float radius, float height) {
#ifdef HAS_BULLET
            auto shape = new btConeShape(radius, height);
            return shape;
#else
            return nullptr;
#endif
        }

        btCollisionShape* BulletPhysicsWorld::createStaticPlaneShape(const float normal[3], float constant) {
#ifdef HAS_BULLET
            auto shape = new btStaticPlaneShape(btVector3(normal[0], normal[1], normal[2]), constant);
            return shape;
#else
            return nullptr;
#endif
        }

        BulletRaycastHit BulletPhysicsWorld::raycast(const float origin[3], const float direction[3], float maxDistance) {
#ifdef HAS_BULLET
            BulletRaycastHit result = { false, {0,0,0}, {0,0,0}, 0.0f, nullptr };

            if (!dynamicsWorld) {
                return result;
            }

            btVector3 btOrigin(origin[0], origin[1], origin[2]);
            btVector3 btDirection(direction[0], direction[1], direction[2]);
            btVector3 btEnd = btOrigin + btDirection.normalized() * maxDistance;

            btCollisionWorld::ClosestRayResultCallback rayCallback(btOrigin, btEnd);

            dynamicsWorld->rayTest(btOrigin, btEnd, rayCallback);

            if (rayCallback.hasHit()) {
                result.hit = true;
                btVector3 hitPoint = rayCallback.m_hitPointWorld;
                btVector3 hitNormal = rayCallback.m_hitNormalWorld;
                
                result.point[0] = hitPoint.x();
                result.point[1] = hitPoint.y();
                result.point[2] = hitPoint.z();
                
                result.normal[0] = hitNormal.x();
                result.normal[1] = hitNormal.y();
                result.normal[2] = hitNormal.z();
                
                result.distance = (hitPoint - btOrigin).length();
                
                // Try to get the rigid body
                if (rayCallback.m_collisionObject) {
                    btRigidBody* btBody = btRigidBody::upcast(rayCallback.m_collisionObject);
                    if (btBody && btBody->getUserPointer()) {
                        result.rigidBody = static_cast<BulletRigidBodyComponent*>(btBody->getUserPointer());
                    }
                }
            }

            return result;
#else
            BulletRaycastHit result = { false, {0,0,0}, {0,0,0}, 0.0f, nullptr };
            return result;
#endif
        }

        void BulletPhysicsWorld::addConstraint(btTypedConstraint* constraint, bool disableCollisionsBetweenLinkedBodies) {
#ifdef HAS_BULLET
            if (dynamicsWorld && constraint) {
                dynamicsWorld->addConstraint(constraint, disableCollisionsBetweenLinkedBodies);
            }
#endif
        }

        void BulletPhysicsWorld::removeConstraint(btTypedConstraint* constraint) {
#ifdef HAS_BULLET
            if (dynamicsWorld && constraint) {
                dynamicsWorld->removeConstraint(constraint);
            }
#endif
        }
    }
}

#endif // HAS_BULLET