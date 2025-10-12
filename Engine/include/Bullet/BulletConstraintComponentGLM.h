/*
 * BulletConstraintComponentGLM.h
 * 
 * Author: Андрій Будильников
 * 
 * GLM wrapper for Bullet Physics constraint component for Sparky Engine
 * Provides a more convenient interface using glm types
 */

#pragma once

#include "BulletConstraintComponent.h"
#include <glm/glm.hpp>

namespace Sparky {
    namespace Bullet {
        class BulletConstraintComponentGLM : public BulletConstraintComponent {
        public:
            // Constraint creation with glm types
            void createPointToPointConstraint(BulletRigidBodyComponent* bodyA, BulletRigidBodyComponent* bodyB,
                                            const glm::vec3& pivotA, const glm::vec3& pivotB) {
                float pivotAArray[3] = {pivotA.x, pivotA.y, pivotA.z};
                float pivotBArray[3] = {pivotB.x, pivotB.y, pivotB.z};
                BulletConstraintComponent::createPointToPointConstraint(bodyA, bodyB, pivotAArray, pivotBArray);
            }
            
            void createHingeConstraint(BulletRigidBodyComponent* bodyA, BulletRigidBodyComponent* bodyB,
                                     const glm::vec3& pivotA, const glm::vec3& pivotB,
                                     const glm::vec3& axisA, const glm::vec3& axisB) {
                float pivotAArray[3] = {pivotA.x, pivotA.y, pivotA.z};
                float pivotBArray[3] = {pivotB.x, pivotB.y, pivotB.z};
                float axisAArray[3] = {axisA.x, axisA.y, axisA.z};
                float axisBArray[3] = {axisB.x, axisB.y, axisB.z};
                BulletConstraintComponent::createHingeConstraint(bodyA, bodyB, pivotAArray, pivotBArray, axisAArray, axisBArray);
            }
            
            void createSliderConstraint(BulletRigidBodyComponent* bodyA, BulletRigidBodyComponent* bodyB,
                                      const glm::mat4& frameA, const glm::mat4& frameB, bool useLinearReferenceFrameA = true) {
                float frameAArray[16];
                float frameBArray[16];
                // Convert glm::mat4 to float array (column-major order)
                for (int i = 0; i < 16; i++) {
                    frameAArray[i] = frameA[i/4][i%4];
                    frameBArray[i] = frameB[i/4][i%4];
                }
                BulletConstraintComponent::createSliderConstraint(bodyA, bodyB, frameAArray, frameBArray, useLinearReferenceFrameA);
            }
            
            void createConeTwistConstraint(BulletRigidBodyComponent* bodyA, BulletRigidBodyComponent* bodyB,
                                         const glm::mat4& frameA, const glm::mat4& frameB) {
                float frameAArray[16];
                float frameBArray[16];
                // Convert glm::mat4 to float array (column-major order)
                for (int i = 0; i < 16; i++) {
                    frameAArray[i] = frameA[i/4][i%4];
                    frameBArray[i] = frameB[i/4][i%4];
                }
                BulletConstraintComponent::createConeTwistConstraint(bodyA, bodyB, frameAArray, frameBArray);
            }
            
            void createGeneric6DofConstraint(BulletRigidBodyComponent* bodyA, BulletRigidBodyComponent* bodyB,
                                           const glm::mat4& frameA, const glm::mat4& frameB, bool useLinearReferenceFrameA = true) {
                float frameAArray[16];
                float frameBArray[16];
                // Convert glm::mat4 to float array (column-major order)
                for (int i = 0; i < 16; i++) {
                    frameAArray[i] = frameA[i/4][i%4];
                    frameBArray[i] = frameB[i/4][i%4];
                }
                BulletConstraintComponent::createGeneric6DofConstraint(bodyA, bodyB, frameAArray, frameBArray, useLinearReferenceFrameA);
            }
            
            void createFixedConstraint(BulletRigidBodyComponent* bodyA, BulletRigidBodyComponent* bodyB) {
                BulletConstraintComponent::createFixedConstraint(bodyA, bodyB);
            }
            
            // Limits and motors (for applicable constraints) with glm types
            void setLinearLimit(const glm::vec3& lower, const glm::vec3& upper) {
                float lowerArray[3] = {lower.x, lower.y, lower.z};
                float upperArray[3] = {upper.x, upper.y, upper.z};
                BulletConstraintComponent::setLinearLimit(lowerArray, upperArray);
            }
            
            void setAngularLimit(const glm::vec3& lower, const glm::vec3& upper) {
                float lowerArray[3] = {lower.x, lower.y, lower.z};
                float upperArray[3] = {upper.x, upper.y, upper.z};
                BulletConstraintComponent::setAngularLimit(lowerArray, upperArray);
            }
            
            void setLinearMotor(bool enable, const glm::vec3& targetVelocity, const glm::vec3& maxForce) {
                float targetVelocityArray[3] = {targetVelocity.x, targetVelocity.y, targetVelocity.z};
                float maxForceArray[3] = {maxForce.x, maxForce.y, maxForce.z};
                BulletConstraintComponent::setLinearMotor(enable, targetVelocityArray, maxForceArray);
            }
            
            void setAngularMotor(bool enable, const glm::vec3& targetVelocity, const glm::vec3& maxForce) {
                float targetVelocityArray[3] = {targetVelocity.x, targetVelocity.y, targetVelocity.z};
                float maxForceArray[3] = {maxForce.x, maxForce.y, maxForce.z};
                BulletConstraintComponent::setAngularMotor(enable, targetVelocityArray, maxForceArray);
            }
        };
    }
}