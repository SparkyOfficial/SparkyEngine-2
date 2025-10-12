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
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#ifdef HAS_BULLET
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
              cachedLinearVelocity(0.0f), cachedAngularVelocity(0.0f),
              cachedLinearDamping(0.0f), cachedAngularDamping(0.0f),
              cachedRestitution(0.0f), cachedFriction(0.0f), cachedRollingFriction(0.0f),
              cachedAffectedByGravity(true) {
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

        void BulletRigidBodyComponent::setLinearVelocity(const glm::vec3& velocity) {
            cachedLinearVelocity = velocity;
#ifdef HAS_BULLET
            if (rigidBody) {
                rigidBody->setLinearVelocity(btVector3(velocity.x, velocity.y, velocity.z));
            }
#else
            // Fallback implementation when Bullet is not available
#endif
        }

        void BulletRigidBodyComponent::setAngularVelocity(const glm::vec3& velocity) {
            cachedAngularVelocity = velocity;
#ifdef HAS_BULLET
            if (rigidBody) {
                rigidBody->setAngularVelocity(btVector3(velocity.x, velocity.y, velocity.z));
            }
#else
            // Fallback implementation when Bullet is not available
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
        void BulletRigidBodyComponent::addForce(const glm::vec3& force) {
            if (rigidBody) {
                rigidBody->applyCentralForce(btVector3(force.x, force.y, force.z));
            }
        }

        void BulletRigidBodyComponent::addForceAtPosition(const glm::vec3& force, const glm::vec3& position) {
            if (rigidBody) {
                rigidBody->applyForce(btVector3(force.x, force.y, force.z), 
                                    btVector3(position.x, position.y, position.z));
            }
        }

        void BulletRigidBodyComponent::addTorque(const glm::vec3& torque) {
            if (rigidBody) {
                rigidBody->applyTorque(btVector3(torque.x, torque.y, torque.z));
            }
        }

        void BulletRigidBodyComponent::clearForces() {
            if (rigidBody) {
                rigidBody->clearForces();
            }
        }

        void BulletRigidBodyComponent::setCollisionShape(btCollisionShape* shape) {
            collisionShape = shape;
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
#else
        void BulletRigidBodyComponent::addForce(const glm::vec3& force) {
            // Fallback implementation when Bullet is not available
        }

        void BulletRigidBodyComponent::addForceAtPosition(const glm::vec3& force, const glm::vec3& position) {
            // Fallback implementation when Bullet is not available
        }

        void BulletRigidBodyComponent::addTorque(const glm::vec3& torque) {
            // Fallback implementation when Bullet is not available
        }

        void BulletRigidBodyComponent::clearForces() {
            // Fallback implementation when Bullet is not available
        }

        void BulletRigidBodyComponent::setCollisionShape(void* shape) {
            // Fallback implementation when Bullet is not available
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
            rigidBody->setLinearVelocity(btVector3(cachedLinearVelocity.x, cachedLinearVelocity.y, cachedLinearVelocity.z));
            rigidBody->setAngularVelocity(btVector3(cachedAngularVelocity.x, cachedAngularVelocity.y, cachedAngularVelocity.z));
            
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
    }
}
#endif // HAS_BULLET
