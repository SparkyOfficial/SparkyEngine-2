/*
 * BulletRigidBodyComponent.h
 * 
 * Author: Андрій Будильников
 * 
 * Bullet Physics rigid body component for Sparky Engine
 * Wraps Bullet Physics btRigidBody for use with GameObjects
 */

#pragma once

#include "../Component.h"

// Forward declarations for Bullet Physics
#ifdef HAS_BULLET
class btRigidBody;
class btCollisionShape;
class btMotionState;
#endif

namespace Sparky {
    namespace Bullet {
        enum class BulletBodyType {
            STATIC,
            KINEMATIC,
            DYNAMIC
        };

        class BulletRigidBodyComponent : public Component {
        public:
            BulletRigidBodyComponent();
            virtual ~BulletRigidBodyComponent();

            void update(float deltaTime) override;
            void render() override;

            // Body type
            void setBodyType(BulletBodyType type);
            BulletBodyType getBodyType() const { return bodyType; }

            // Mass and inertia
            void setMass(float mass);
            float getMass() const { return mass; }

            // Physics properties
            void setRestitution(float restitution);
            float getRestitution() const { return cachedRestitution; }

            void setFriction(float friction);
            float getFriction() const { return cachedFriction; }

            void setRollingFriction(float rollingFriction);
            float getRollingFriction() const { return cachedRollingFriction; }

            void setLinearDamping(float damping);
            float getLinearDamping() const { return cachedLinearDamping; }

            void setAngularDamping(float damping);
            float getAngularDamping() const { return cachedAngularDamping; }

            // Velocity
            void setLinearVelocity(const float velocity[3]);
            void getLinearVelocity(float outVelocity[3]) const;

            void setAngularVelocity(const float velocity[3]);
            void getAngularVelocity(float outVelocity[3]) const;

            // Gravity
            void setAffectedByGravity(bool affected);
            bool isAffectedByGravity() const;

            // Forces and torques
            void addForce(const float force[3]);
            void addForceAtPosition(const float force[3], const float position[3]);
            void addTorque(const float torque[3]);
            void clearForces();

            // Collision shape
            void setCollisionShape(void* shape);
            void* getCollisionShape() const;

            // Activation state
            void activate(bool force = false);
            bool isActive() const;

            // Make createRigidBody public so BulletPhysicsWorld can access it
            void createRigidBody();

#ifdef HAS_BULLET
            // Get Bullet rigid body (only available when HAS_BULLET is defined)
            btRigidBody* getRigidBody() const { return rigidBody; }
#else
            // Fallback when Bullet is not available
            void* getRigidBody() const { return nullptr; }
#endif

        private:
            BulletBodyType bodyType;
            float mass;
            
#ifdef HAS_BULLET
            btRigidBody* rigidBody;
            btCollisionShape* collisionShape;
            btMotionState* motionState;
#endif
            
            // Cached values for when rigid body is not created yet
            float cachedLinearVelocity[3];
            float cachedAngularVelocity[3];
            float cachedLinearDamping;
            float cachedAngularDamping;
            float cachedRestitution;
            float cachedFriction;
            float cachedRollingFriction;
            bool cachedAffectedByGravity;
            
            // Internal methods
            void updateRigidBodyFromCache();
            void syncTransformFromBullet();
        };
    }
}