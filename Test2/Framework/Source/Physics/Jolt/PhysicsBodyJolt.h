#pragma once

#include "Math/Vector.h"
#include "Physics/PhysicsBody.h"

namespace JPH
{
    class Body;
}

namespace fw
{
    class PhysicsWorld;
    class PhysicsWorldJolt;

    class PhysicsBodyJolt : public PhysicsBody
    {
    public:
        PhysicsBodyJolt(GameObject* pGameObject, PhysicsWorldJolt* pPhysicsWorld, bool dynamic, vec3 position, vec3 boxSize);
        PhysicsBodyJolt(GameObject* pGameObject, PhysicsWorldJolt* pPhysicsWorld, bool dynamic, vec3 position, float radius);
        PhysicsBodyJolt(GameObject* pGameObject, PhysicsWorldJolt* pPhysicsWorld, bool dynamic, vec3 position, const std::vector<vec3>& positions, int sampleCount);
        virtual ~PhysicsBodyJolt();

        virtual vec3 getPosition() override;
        virtual vec3 getRotation() override;

        virtual void setPosition(vec3 pos) override;
        virtual void setRotation(vec3 rot) override;

        virtual void applyForceToCenter(vec3 force) override;
        virtual void applyImpulseToCenter(vec3 impulse) override;

    private:
        JPH::Body* m_physicsBody = nullptr;
        PhysicsWorldJolt* m_pPhysicsWorld = nullptr;
    };
}
