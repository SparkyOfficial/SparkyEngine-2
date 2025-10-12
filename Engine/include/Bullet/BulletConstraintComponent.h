/*
 * BulletConstraintComponent.h
 * 
 * Author: Андрій Будильников
 * 
 * Bullet Physics constraint component for Sparky Engine
 * Wraps Bullet Physics constraints for connecting rigid bodies
 */

#pragma once

#include "../Component.h"

// We'll use float arrays instead of glm::vec3 and glm::mat4 when HAS_BULLET is not defined
// to avoid glm dependency issues

namespace Sparky {
    namespace Bullet {
        // Forward declaration
        class BulletRigidBodyComponent;
        
        enum class ConstraintType {
            POINT_TO_POINT,
            HINGE,
            SLIDER,
            CONETWIST,
            GENERIC_6DOF,
            FIXED
        };

        class BulletConstraintComponent : public Component {
        public:
            BulletConstraintComponent();
            virtual ~BulletConstraintComponent();

            void update(float deltaTime) override;
            void render() override;

            // Constraint creation
            void createPointToPointConstraint(BulletRigidBodyComponent* bodyA, BulletRigidBodyComponent* bodyB,
                                            const float pivotA[3], const float pivotB[3]);
            void createHingeConstraint(BulletRigidBodyComponent* bodyA, BulletRigidBodyComponent* bodyB,
                                     const float pivotA[3], const float pivotB[3],
                                     const float axisA[3], const float axisB[3]);
            void createSliderConstraint(BulletRigidBodyComponent* bodyA, BulletRigidBodyComponent* bodyB,
                                      const float frameA[16], const float frameB[16], bool useLinearReferenceFrameA = true);
            void createConeTwistConstraint(BulletRigidBodyComponent* bodyA, BulletRigidBodyComponent* bodyB,
                                         const float frameA[16], const float frameB[16]);
            void createGeneric6DofConstraint(BulletRigidBodyComponent* bodyA, BulletRigidBodyComponent* bodyB,
                                           const float frameA[16], const float frameB[16], bool useLinearReferenceFrameA = true);
            void createFixedConstraint(BulletRigidBodyComponent* bodyA, BulletRigidBodyComponent* bodyB);

            // Constraint properties
            ConstraintType getConstraintType() const { return constraintType; }
#ifdef HAS_BULLET
            class btTypedConstraint* getConstraint() const { return constraint; }
#else
            void* getConstraint() const { return constraint; }
#endif
            
            // Constraint parameters
            void setBreakingImpulseThreshold(float threshold);
            float getBreakingImpulseThreshold() const;
            
            void setEnabled(bool enabled);
            bool isEnabled() const;
            
            // Limits and motors (for applicable constraints)
            void setLinearLimit(const float lower[3], const float upper[3]);
            void setAngularLimit(const float lower[3], const float upper[3]);
            
            void setLinearMotor(bool enable, const float targetVelocity[3], const float maxForce[3]);
            void setAngularMotor(bool enable, const float targetVelocity[3], const float maxForce[3]);

        private:
            ConstraintType constraintType;
#ifdef HAS_BULLET
            class btTypedConstraint* constraint;
#else
            void* constraint;
#endif
            
            // Cached constraint parameters
            float breakingImpulseThreshold;
            bool enabled;
        };
    }
}