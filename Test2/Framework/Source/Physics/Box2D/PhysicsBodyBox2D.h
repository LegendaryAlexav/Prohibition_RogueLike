#pragma once

#include "Math/Vector.h"
#include "Physics/PhysicsBody.h"

namespace fw
{
    class PhysicsWorld;
    class PhysicsWorldBox2D;

    class PhysicsBodyBox2D : public PhysicsBody
    {
    public:
        PhysicsBodyBox2D(GameObject* pGameObject, PhysicsWorldBox2D* pPhysicsWorld, bool dynamic, vec3 position, vec3 boxSize);
        virtual ~PhysicsBodyBox2D();

        virtual vec3 getPosition() override;
        virtual vec3 getRotation() override;

        virtual void setPosition(vec3 pos) override;
        virtual void setRotation(vec3 rot) override;

        virtual void applyForceToCenter(vec3 force) override;
        virtual void applyImpulseToCenter(vec3 impulse) override;

    private:
        b2BodyId m_physicsBody;
        PhysicsWorldBox2D* m_pPhysicsWorld = nullptr;
    };
}
