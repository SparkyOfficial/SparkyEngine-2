#ifdef HAS_BULLET

/*
 * BulletConstraintComponent.cpp
 * 
 * Author: Андрій Будильников
 * 
 * Bullet Physics constraint component implementation for Sparky Engine
 */

#include "../../include/Bullet/BulletConstraintComponent.h"
#include "../../include/Bullet/BulletRigidBodyComponent.h"
#include <iostream>

// Bullet Physics includes
#include "btBulletDynamicsCommon.h"
#include "BulletDynamics/ConstraintSolver/btPoint2PointConstraint.h"
#include "BulletDynamics/ConstraintSolver/btHingeConstraint.h"
#include "BulletDynamics/ConstraintSolver/btSliderConstraint.h"
#include "BulletDynamics/ConstraintSolver/btConeTwistConstraint.h"
#include "BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.h"
#include "LinearMath/btTransform.h"

namespace Sparky {
    namespace Bullet {
        BulletConstraintComponent::BulletConstraintComponent() 
            : constraintType(ConstraintType::POINT_TO_POINT), constraint(nullptr),
              breakingImpulseThreshold(FLT_MAX), enabled(true) {
        }

        BulletConstraintComponent::~BulletConstraintComponent() {
            if (constraint) {
                delete constraint;
            }
        }

        void BulletConstraintComponent::update(float deltaTime) {
            // Constraints don't need updating
        }

        void BulletConstraintComponent::render() {
            // Rendering is handled by the renderer, not the constraint component
        }

        void BulletConstraintComponent::createPointToPointConstraint(BulletRigidBodyComponent* bodyA, BulletRigidBodyComponent* bodyB,
                                        const float pivotA[3], const float pivotB[3]) {
            constraintType = ConstraintType::POINT_TO_POINT;
            
            // Clean up existing constraint
            if (constraint) {
                delete constraint;
                constraint = nullptr;
            }
            
            // Create the constraint
            if (bodyA && bodyB) {
                constraint = new btPoint2PointConstraint(*bodyA->getRigidBody(), *bodyB->getRigidBody(),
                                                        btVector3(pivotA[0], pivotA[1], pivotA[2]),
                                                        btVector3(pivotB[0], pivotB[1], pivotB[2]));
            } else if (bodyA) {
                constraint = new btPoint2PointConstraint(*bodyA->getRigidBody(),
                                                        btVector3(pivotA[0], pivotA[1], pivotA[2]));
            }
        }

        void BulletConstraintComponent::createHingeConstraint(BulletRigidBodyComponent* bodyA, BulletRigidBodyComponent* bodyB,
                                 const float pivotA[3], const float pivotB[3],
                                 const float axisA[3], const float axisB[3]) {
            constraintType = ConstraintType::HINGE;
            
            // Clean up existing constraint
            if (constraint) {
                delete constraint;
                constraint = nullptr;
            }
            
            // Create the constraint
            if (bodyA && bodyB) {
                constraint = new btHingeConstraint(*bodyA->getRigidBody(), *bodyB->getRigidBody(),
                                                  btVector3(pivotA[0], pivotA[1], pivotA[2]),
                                                  btVector3(pivotB[0], pivotB[1], pivotB[2]),
                                                  btVector3(axisA[0], axisA[1], axisA[2]),
                                                  btVector3(axisB[0], axisB[1], axisB[2]));
            } else if (bodyA) {
                constraint = new btHingeConstraint(*bodyA->getRigidBody(),
                                                  btVector3(pivotA[0], pivotA[1], pivotA[2]),
                                                  btVector3(axisA[0], axisA[1], axisA[2]));
            }
        }

        void BulletConstraintComponent::createSliderConstraint(BulletRigidBodyComponent* bodyA, BulletRigidBodyComponent* bodyB,
                                      const float frameA[16], const float frameB[16], bool useLinearReferenceFrameA) {
            constraintType = ConstraintType::SLIDER;
            
            // Clean up existing constraint
            if (constraint) {
                delete constraint;
                constraint = nullptr;
            }
            
            // Convert float arrays to Bullet transforms
            btTransform btFrameA, btFrameB;
            
            // Extract position from float arrays (assuming column-major order)
            btFrameA.setOrigin(btVector3(frameA[12], frameA[13], frameA[14]));
            btFrameB.setOrigin(btVector3(frameB[12], frameB[13], frameB[14]));
            
            // For rotation, we'll use a simple approach for now
            btFrameA.setRotation(btQuaternion(0, 0, 0, 1));
            btFrameB.setRotation(btQuaternion(0, 0, 0, 1));
            
            // Create the constraint
            if (bodyA && bodyB) {
                constraint = new btSliderConstraint(*bodyA->getRigidBody(), *bodyB->getRigidBody(),
                                                   btFrameA, btFrameB, useLinearReferenceFrameA);
            } else if (bodyA) {
                constraint = new btSliderConstraint(*bodyA->getRigidBody(), btFrameA, useLinearReferenceFrameA);
            }
        }

        void BulletConstraintComponent::createConeTwistConstraint(BulletRigidBodyComponent* bodyA, BulletRigidBodyComponent* bodyB,
                                         const float frameA[16], const float frameB[16]) {
            constraintType = ConstraintType::CONETWIST;
            
            // Clean up existing constraint
            if (constraint) {
                delete constraint;
                constraint = nullptr;
            }
            
            // Convert float arrays to Bullet transforms
            btTransform btFrameA, btFrameB;
            
            // Extract position from float arrays (assuming column-major order)
            btFrameA.setOrigin(btVector3(frameA[12], frameA[13], frameA[14]));
            btFrameB.setOrigin(btVector3(frameB[12], frameB[13], frameB[14]));
            
            // For rotation, we'll use a simple approach for now
            btFrameA.setRotation(btQuaternion(0, 0, 0, 1));
            btFrameB.setRotation(btQuaternion(0, 0, 0, 1));
            
            // Create the constraint
            if (bodyA && bodyB) {
                constraint = new btConeTwistConstraint(*bodyA->getRigidBody(), *bodyB->getRigidBody(),
                                                      btFrameA, btFrameB);
            } else if (bodyA) {
                constraint = new btConeTwistConstraint(*bodyA->getRigidBody(), btFrameA);
            }
        }

        void BulletConstraintComponent::createGeneric6DofConstraint(BulletRigidBodyComponent* bodyA, BulletRigidBodyComponent* bodyB,
                                           const float frameA[16], const float frameB[16], bool useLinearReferenceFrameA) {
            constraintType = ConstraintType::GENERIC_6DOF;
            
            // Clean up existing constraint
            if (constraint) {
                delete constraint;
                constraint = nullptr;
            }
            
            // Convert float arrays to Bullet transforms
            btTransform btFrameA, btFrameB;
            
            // Extract position from float arrays (assuming column-major order)
            btFrameA.setOrigin(btVector3(frameA[12], frameA[13], frameA[14]));
            btFrameB.setOrigin(btVector3(frameB[12], frameB[13], frameB[14]));
            
            // For rotation, we'll use a simple approach for now
            btFrameA.setRotation(btQuaternion(0, 0, 0, 1));
            btFrameB.setRotation(btQuaternion(0, 0, 0, 1));
            
            // Create the constraint
            if (bodyA && bodyB) {
                constraint = new btGeneric6DofConstraint(*bodyA->getRigidBody(), *bodyB->getRigidBody(),
                                                        btFrameA, btFrameB, useLinearReferenceFrameA);
            } else if (bodyA) {
                constraint = new btGeneric6DofConstraint(*bodyA->getRigidBody(), btFrameA, useLinearReferenceFrameA);
            }
        }

        void BulletConstraintComponent::createFixedConstraint(BulletRigidBodyComponent* bodyA, BulletRigidBodyComponent* bodyB) {
            constraintType = ConstraintType::FIXED;
            
            // Clean up existing constraint
            if (constraint) {
                delete constraint;
                constraint = nullptr;
            }
            
            // Convert float arrays to Bullet transforms
            btTransform btFrameA, btFrameB;
            btFrameA.setIdentity();
            btFrameB.setIdentity();
            
            // Create the constraint
            if (bodyA && bodyB) {
                constraint = new btGeneric6DofConstraint(*bodyA->getRigidBody(), *bodyB->getRigidBody(),
                                                        btFrameA, btFrameB, true);
                
                // Set all limits to 0 to make it a fixed constraint
                if (auto* genericConstraint = static_cast<btGeneric6DofConstraint*>(constraint)) {
                    genericConstraint->setLinearLowerLimit(btVector3(0, 0, 0));
                    genericConstraint->setLinearUpperLimit(btVector3(0, 0, 0));
                    genericConstraint->setAngularLowerLimit(btVector3(0, 0, 0));
                    genericConstraint->setAngularUpperLimit(btVector3(0, 0, 0));
                }
            }
        }

        void BulletConstraintComponent::setBreakingImpulseThreshold(float threshold) {
            breakingImpulseThreshold = threshold;
            if (constraint) {
                constraint->setBreakingImpulseThreshold(threshold);
            }
        }

        float BulletConstraintComponent::getBreakingImpulseThreshold() const {
            if (constraint) {
                return constraint->getBreakingImpulseThreshold();
            }
            return breakingImpulseThreshold;
        }

        void BulletConstraintComponent::setEnabled(bool enabled) {
            this->enabled = enabled;
            if (constraint) {
                constraint->setEnabled(enabled);
            }
        }

        bool BulletConstraintComponent::isEnabled() const {
            if (constraint) {
                return constraint->isEnabled();
            }
            return enabled;
        }

        void BulletConstraintComponent::setLinearLimit(const float lower[3], const float upper[3]) {
            if (constraintType == ConstraintType::GENERIC_6DOF) {
                if (auto* genericConstraint = static_cast<btGeneric6DofConstraint*>(constraint)) {
                    genericConstraint->setLinearLowerLimit(btVector3(lower[0], lower[1], lower[2]));
                    genericConstraint->setLinearUpperLimit(btVector3(upper[0], upper[1], upper[2]));
                }
            }
        }

        void BulletConstraintComponent::setAngularLimit(const float lower[3], const float upper[3]) {
            if (constraintType == ConstraintType::GENERIC_6DOF) {
                if (auto* genericConstraint = static_cast<btGeneric6DofConstraint*>(constraint)) {
                    genericConstraint->setAngularLowerLimit(btVector3(lower[0], lower[1], lower[2]));
                    genericConstraint->setAngularUpperLimit(btVector3(upper[0], upper[1], upper[2]));
                }
            }
        }

        void BulletConstraintComponent::setLinearMotor(bool enable, const float targetVelocity[3], const float maxForce[3]) {
            if (constraintType == ConstraintType::GENERIC_6DOF) {
                if (auto* genericConstraint = static_cast<btGeneric6DofConstraint*>(constraint)) {
                    genericConstraint->getTranslationalLimitMotor()->m_enableMotor[0] = enable;
                    genericConstraint->getTranslationalLimitMotor()->m_targetVelocity = btVector3(targetVelocity[0], targetVelocity[1], targetVelocity[2]);
                    genericConstraint->getTranslationalLimitMotor()->m_maxMotorForce = btVector3(maxForce[0], maxForce[1], maxForce[2]);
                }
            }
        }

        void BulletConstraintComponent::setAngularMotor(bool enable, const float targetVelocity[3], const float maxForce[3]) {
            if (constraintType == ConstraintType::GENERIC_6DOF) {
                if (auto* genericConstraint = static_cast<btGeneric6DofConstraint*>(constraint)) {
                    genericConstraint->getRotationalLimitMotor(0)->m_enableMotor = enable;
                    genericConstraint->getRotationalLimitMotor(0)->m_targetVelocity = targetVelocity[0];
                    genericConstraint->getRotationalLimitMotor(0)->m_maxMotorForce = maxForce[0];
                }
            }
        }
    }
}

#else

/*
 * BulletConstraintComponent.cpp
 * 
 * Author: Андрій Будильников
 * 
 * Fallback implementation when Bullet Physics is not available
 */

#include "../../include/Bullet/BulletConstraintComponent.h"
#include "../../include/Bullet/BulletRigidBodyComponent.h"
#include <iostream>

namespace Sparky {
    namespace Bullet {
        BulletConstraintComponent::BulletConstraintComponent() 
            : constraintType(ConstraintType::POINT_TO_POINT), constraint(nullptr),
              breakingImpulseThreshold(FLT_MAX), enabled(true) {
        }

        BulletConstraintComponent::~BulletConstraintComponent() {
            // No cleanup needed for fallback implementation
        }

        void BulletConstraintComponent::update(float deltaTime) {
            // Constraints don't need updating
        }

        void BulletConstraintComponent::render() {
            // Rendering is handled by the renderer, not the constraint component
        }

        void BulletConstraintComponent::createPointToPointConstraint(BulletRigidBodyComponent* bodyA, BulletRigidBodyComponent* bodyB,
                                        const float pivotA[3], const float pivotB[3]) {
            constraintType = ConstraintType::POINT_TO_POINT;
            // Fallback implementation - no actual constraint created
        }

        void BulletConstraintComponent::createHingeConstraint(BulletRigidBodyComponent* bodyA, BulletRigidBodyComponent* bodyB,
                                 const float pivotA[3], const float pivotB[3],
                                 const float axisA[3], const float axisB[3]) {
            constraintType = ConstraintType::HINGE;
            // Fallback implementation - no actual constraint created
        }

        void BulletConstraintComponent::createSliderConstraint(BulletRigidBodyComponent* bodyA, BulletRigidBodyComponent* bodyB,
                                      const float frameA[16], const float frameB[16], bool useLinearReferenceFrameA) {
            constraintType = ConstraintType::SLIDER;
            // Fallback implementation - no actual constraint created
        }

        void BulletConstraintComponent::createConeTwistConstraint(BulletRigidBodyComponent* bodyA, BulletRigidBodyComponent* bodyB,
                                         const float frameA[16], const float frameB[16]) {
            constraintType = ConstraintType::CONETWIST;
            // Fallback implementation - no actual constraint created
        }

        void BulletConstraintComponent::createGeneric6DofConstraint(BulletRigidBodyComponent* bodyA, BulletRigidBodyComponent* bodyB,
                                           const float frameA[16], const float frameB[16], bool useLinearReferenceFrameA) {
            constraintType = ConstraintType::GENERIC_6DOF;
            // Fallback implementation - no actual constraint created
        }

        void BulletConstraintComponent::createFixedConstraint(BulletRigidBodyComponent* bodyA, BulletRigidBodyComponent* bodyB) {
            constraintType = ConstraintType::FIXED;
            // Fallback implementation - no actual constraint created
        }

        void BulletConstraintComponent::setBreakingImpulseThreshold(float threshold) {
            breakingImpulseThreshold = threshold;
            // Fallback implementation - no actual constraint to set
        }

        float BulletConstraintComponent::getBreakingImpulseThreshold() const {
            return breakingImpulseThreshold;
        }

        void BulletConstraintComponent::setEnabled(bool enabled) {
            this->enabled = enabled;
            // Fallback implementation - no actual constraint to set
        }

        bool BulletConstraintComponent::isEnabled() const {
            return enabled;
        }

        void BulletConstraintComponent::setLinearLimit(const float lower[3], const float upper[3]) {
            // Fallback implementation - no actual constraint to set
        }

        void BulletConstraintComponent::setAngularLimit(const float lower[3], const float upper[3]) {
            // Fallback implementation - no actual constraint to set
        }

        void BulletConstraintComponent::setLinearMotor(bool enable, const float targetVelocity[3], const float maxForce[3]) {
            // Fallback implementation - no actual constraint to set
        }

        void BulletConstraintComponent::setAngularMotor(bool enable, const float targetVelocity[3], const float maxForce[3]) {
            // Fallback implementation - no actual constraint to set
        }
    }
}

#endif // HAS_BULLET