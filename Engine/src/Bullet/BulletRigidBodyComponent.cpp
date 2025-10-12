/*
 * BulletRigidBodyComponent.cpp
 * 
 * Author: Андрій Будильников
 * 
 * Bullet Physics rigid body component implementation for Sparky Engine
 */

#include "../../include/Bullet/BulletRigidBodyComponent.h"
#include "../../include/GameObject.h"
#include <iostream>

#ifdef HAS_BULLET
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
// Bullet Physics includes
#include "btBulletDynamicsCommon.h"
#include "LinearMath/btDefaultMotionState.h"
#endif

namespace Sparky {
    namespace Bullet {
        BulletRigidBodyComponent::BulletRigidBodyComponent() 
            : bodyType(BulletBodyType::DYNAMIC), mass(1.0f), 
#ifdef HAS_BULLET
              rigidBody(nullptr), collisionShape(nullptr), motionState(nullptr),
#endif
              cachedLinearDamping(0.0f), cachedAngularDamping(0.0f),
              cachedRestitution(0.0f), cachedFriction(0.0f), cachedRollingFriction(0.0f),
              cachedAffectedByGravity(true) {
            cachedLinearVelocity[0] = cachedLinearVelocity[1] = cachedLinearVelocity[2] = 0.0f;
            cachedAngularVelocity[0] = cachedAngularVelocity[1] = cachedAngularVelocity[2] = 0.0f;
        }

        BulletRigidBodyComponent::~BulletRigidBodyComponent() {
#ifdef HAS_BULLET
            if (motionState) {
                delete motionState;
            }
            if (rigidBody) {
                delete rigidBody;
            }
#endif
        }

        void BulletRigidBodyComponent::update(float deltaTime) {
#ifdef HAS_BULLET
            if (rigidBody && getGameObject()) {
                syncTransformFromBullet();
            }
#else
            // Fallback implementation when Bullet is not available
#endif
        }

        void BulletRigidBodyComponent::render() {
            // Rendering is handled by the renderer, not the physics component
        }

        void BulletRigidBodyComponent::setBodyType(BulletBodyType type) {
            bodyType = type;
#ifdef HAS_BULLET
            // If rigid body already exists, we need to recreate it with the new body type
            if (rigidBody) {
                // Store current properties
                btVector3 linearVel = rigidBody->getLinearVelocity();
                btVector3 angularVel = rigidBody->getAngularVelocity();
                
                // Remove from world and delete
                if (motionState) {
                    delete motionState;
                    motionState = nullptr;
                }
                delete rigidBody;
                rigidBody = nullptr;
                
                // Recreate with new body type
                createRigidBody();
                
                // Restore properties
                if (rigidBody) {
                    rigidBody->setLinearVelocity(linearVel);
                    rigidBody->setAngularVelocity(angularVel);
                }
            }
#else
            // Fallback implementation when Bullet is not available
#endif
        }

        void BulletRigidBodyComponent::setMass(float mass) {
            this->mass = mass;
#ifdef HAS_BULLET
            // If rigid body already exists, we need to recreate it with the new mass
            if (rigidBody) {
                // Store current properties
                btVector3 linearVel = rigidBody->getLinearVelocity();
                btVector3 angularVel = rigidBody->getAngularVelocity();
                
                // Remove from world and delete
                if (motionState) {
                    delete motionState;
                    motionState = nullptr;
                }
                delete rigidBody;
                rigidBody = nullptr;
                
                // Recreate with new mass
                createRigidBody();
                
                // Restore properties
                if (rigidBody) {
                    rigidBody->setLinearVelocity(linearVel);
                    rigidBody->setAngularVelocity(angularVel);
                }
            }
#else
            // Fallback implementation when Bullet is not available
#endif
        }

        void BulletRigidBodyComponent::setRestitution(float restitution) {
            cachedRestitution = restitution;
#ifdef HAS_BULLET
            if (rigidBody) {
                rigidBody->setRestitution(restitution);
            }
#else
            // Fallback implementation when Bullet is not available
#endif
        }

        void BulletRigidBodyComponent::setFriction(float friction) {
            cachedFriction = friction;
#ifdef HAS_BULLET
            if (rigidBody) {
                rigidBody->setFriction(friction);
            }
#else
            // Fallback implementation when Bullet is not available
#endif
        }

        void BulletRigidBodyComponent::setRollingFriction(float rollingFriction) {
            cachedRollingFriction = rollingFriction;
#ifdef HAS_BULLET
            if (rigidBody) {
                rigidBody->setRollingFriction(rollingFriction);
            }
#else
            // Fallback implementation when Bullet is not available
#endif
        }

        void BulletRigidBodyComponent::setLinearDamping(float damping) {
            cachedLinearDamping = damping;
#ifdef HAS_BULLET
            if (rigidBody) {
                rigidBody->setDamping(damping, rigidBody->getAngularDamping());
            }
#else
            // Fallback implementation when Bullet is not available
#endif
        }

        void BulletRigidBodyComponent::setAngularDamping(float damping) {
            cachedAngularDamping = damping;
#ifdef HAS_BULLET
            if (rigidBody) {
                rigidBody->setDamping(rigidBody->getLinearDamping(), damping);
            }
#else
            // Fallback implementation when Bullet is not available
#endif
        }

        void BulletRigidBodyComponent::setLinearVelocity(const float velocity[3]) {
            cachedLinearVelocity[0] = velocity[0];
            cachedLinearVelocity[1] = velocity[1];
            cachedLinearVelocity[2] = velocity[2];
#ifdef HAS_BULLET
            if (rigidBody) {
                rigidBody->setLinearVelocity(btVector3(velocity[0], velocity[1], velocity[2]));
            }
#else
            // Fallback implementation when Bullet is not available
#endif
        }

        void BulletRigidBodyComponent::getLinearVelocity(float outVelocity[3]) const {
#ifdef HAS_BULLET
            if (rigidBody) {
                btVector3 vel = rigidBody->getLinearVelocity();
                outVelocity[0] = vel.x();
                outVelocity[1] = vel.y();
                outVelocity[2] = vel.z();
            } else {
                outVelocity[0] = cachedLinearVelocity[0];
                outVelocity[1] = cachedLinearVelocity[1];
                outVelocity[2] = cachedLinearVelocity[2];
            }
#else
            outVelocity[0] = cachedLinearVelocity[0];
            outVelocity[1] = cachedLinearVelocity[1];
            outVelocity[2] = cachedLinearVelocity[2];
#endif
        }

        void BulletRigidBodyComponent::setAngularVelocity(const float velocity[3]) {
            cachedAngularVelocity[0] = velocity[0];
            cachedAngularVelocity[1] = velocity[1];
            cachedAngularVelocity[2] = velocity[2];
#ifdef HAS_BULLET
            if (rigidBody) {
                rigidBody->setAngularVelocity(btVector3(velocity[0], velocity[1], velocity[2]));
            }
#else
            // Fallback implementation when Bullet is not available
#endif
        }

        void BulletRigidBodyComponent::getAngularVelocity(float outVelocity[3]) const {
#ifdef HAS_BULLET
            if (rigidBody) {
                btVector3 vel = rigidBody->getAngularVelocity();
                outVelocity[0] = vel.x();
                outVelocity[1] = vel.y();
                outVelocity[2] = vel.z();
            } else {
                outVelocity[0] = cachedAngularVelocity[0];
                outVelocity[1] = cachedAngularVelocity[1];
                outVelocity[2] = cachedAngularVelocity[2];
            }
#else
            outVelocity[0] = cachedAngularVelocity[0];
            outVelocity[1] = cachedAngularVelocity[1];
            outVelocity[2] = cachedAngularVelocity[2];
#endif
        }

        void BulletRigidBodyComponent::setAffectedByGravity(bool affected) {
            cachedAffectedByGravity = affected;
#ifdef HAS_BULLET
            if (rigidBody) {
                if (affected) {
                    rigidBody->setGravity(rigidBody->getWorld()->getGravity());
                } else {
                    rigidBody->setGravity(btVector3(0, 0, 0));
                }
            }
#else
            // Fallback implementation when Bullet is not available
#endif
        }

        bool BulletRigidBodyComponent::isAffectedByGravity() const {
            return cachedAffectedByGravity;
        }

#ifdef HAS_BULLET
        void BulletRigidBodyComponent::addForce(const float force[3]) {
            if (rigidBody) {
                rigidBody->applyCentralForce(btVector3(force[0], force[1], force[2]));
            }
        }

        void BulletRigidBodyComponent::addForceAtPosition(const float force[3], const float position[3]) {
            if (rigidBody) {
                rigidBody->applyForce(btVector3(force[0], force[1], force[2]), 
                                    btVector3(position[0], position[1], position[2]));
            }
        }

        void BulletRigidBodyComponent::addTorque(const float torque[3]) {
            if (rigidBody) {
                rigidBody->applyTorque(btVector3(torque[0], torque[1], torque[2]));
            }
        }

        void BulletRigidBodyComponent::clearForces() {
            if (rigidBody) {
                rigidBody->clearForces();
            }
        }

        void BulletRigidBodyComponent::setCollisionShape(void* shape) {
            collisionShape = static_cast<btCollisionShape*>(shape);
            // If rigid body already exists, we need to recreate it with the new shape
            if (rigidBody) {
                // Store current properties
                btVector3 linearVel = rigidBody->getLinearVelocity();
                btVector3 angularVel = rigidBody->getAngularVelocity();
                
                // Remove from world and delete
                if (rigidBody->getMotionState()) {
                    delete rigidBody->getMotionState();
                }
                delete rigidBody;
                
                // Recreate with new shape
                createRigidBody();
                
                // Restore properties
                if (rigidBody) {
                    rigidBody->setLinearVelocity(linearVel);
                    rigidBody->setAngularVelocity(angularVel);
                }
            }
        }

        void* BulletRigidBodyComponent::getCollisionShape() const {
            return collisionShape;
        }
#else
        void BulletRigidBodyComponent::addForce(const float force[3]) {
            // Fallback implementation when Bullet is not available
            // Store force data for potential use when Bullet becomes available
        }

        void BulletRigidBodyComponent::addForceAtPosition(const float force[3], const float position[3]) {
            // Fallback implementation when Bullet is not available
            // Store force and position data for potential use when Bullet becomes available
        }

        void BulletRigidBodyComponent::addTorque(const float torque[3]) {
            // Fallback implementation when Bullet is not available
            // Store torque data for potential use when Bullet becomes available
        }

        void BulletRigidBodyComponent::clearForces() {
            // Fallback implementation when Bullet is not available
        }

        void BulletRigidBodyComponent::setCollisionShape(void* shape) {
            // Fallback implementation when Bullet is not available
            // Store shape data for potential use when Bullet becomes available
        }

        void* BulletRigidBodyComponent::getCollisionShape() const {
            return nullptr;
        }
#endif

        void BulletRigidBodyComponent::activate(bool force) {
#ifdef HAS_BULLET
            if (rigidBody) {
                rigidBody->activate(force);
            }
#else
            // Fallback implementation when Bullet is not available
#endif
        }

        bool BulletRigidBodyComponent::isActive() const {
#ifdef HAS_BULLET
            if (rigidBody) {
                return rigidBody->isActive();
            }
#endif
            return false;
        }

        void BulletRigidBodyComponent::createRigidBody() {
#ifdef HAS_BULLET
            if (!collisionShape) {
                std::cerr << "Cannot create rigid body without collision shape!" << std::endl;
                return;
            }

            // Clean up existing rigid body if it exists
            if (rigidBody) {
                if (motionState) {
                    delete motionState;
                    motionState = nullptr;
                }
                delete rigidBody;
                rigidBody = nullptr;
            }

            // Get initial transform from GameObject
            glm::vec3 position(0.0f);
            glm::quat rotation(1.0f, 0.0f, 0.0f, 0.0f);
            
            if (getGameObject()) {
                position = getGameObject()->getPosition();
                rotation = getGameObject()->getRotation();
            }

            // Convert to Bullet transform
            btTransform transform;
            transform.setOrigin(btVector3(position.x, position.y, position.z));
            
            btQuaternion btRot(rotation.x, rotation.y, rotation.z, rotation.w);
            transform.setRotation(btRot);

            // Create motion state
            motionState = new btDefaultMotionState(transform);

            // Calculate local inertia
            btVector3 localInertia(0, 0, 0);
            if (bodyType == BulletBodyType::DYNAMIC && mass > 0.0f) {
                collisionShape->calculateLocalInertia(mass, localInertia);
            }

            // Create rigid body construction info
            btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, collisionShape, localInertia);
            
            // Apply cached properties
            rbInfo.m_linearDamping = cachedLinearDamping;
            rbInfo.m_angularDamping = cachedAngularDamping;
            rbInfo.m_restitution = cachedRestitution;
            rbInfo.m_friction = cachedFriction;
            rbInfo.m_rollingFriction = cachedRollingFriction;

            // Create the rigid body
            rigidBody = new btRigidBody(rbInfo);
            
            // Apply cached velocities
            rigidBody->setLinearVelocity(btVector3(cachedLinearVelocity[0], cachedLinearVelocity[1], cachedLinearVelocity[2]));
            rigidBody->setAngularVelocity(btVector3(cachedAngularVelocity[0], cachedAngularVelocity[1], cachedAngularVelocity[2]));
            
            // Apply gravity setting
            if (!cachedAffectedByGravity) {
                rigidBody->setGravity(btVector3(0, 0, 0));
            }
#else
            // Fallback implementation when Bullet is not available
#endif
        }

        void BulletRigidBodyComponent::syncTransformFromBullet() {
#ifdef HAS_BULLET
            if (!rigidBody || !getGameObject()) {
                return;
            }

            // Get the world transform from Bullet
            btTransform transform;
            rigidBody->getMotionState()->getWorldTransform(transform);

            // Convert to glm
            btVector3 origin = transform.getOrigin();
            btQuaternion rotation = transform.getRotation();

            glm::vec3 position(origin.x(), origin.y(), origin.z());
            glm::quat glmRotation(rotation.w(), rotation.x(), rotation.y(), rotation.z());

            // Update GameObject transform
            getGameObject()->setPosition(position);
            getGameObject()->setRotation(glmRotation);
#else
            // Fallback implementation when Bullet is not available
#endif
        }
    }
}