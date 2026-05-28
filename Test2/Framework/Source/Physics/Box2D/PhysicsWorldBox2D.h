#pragma once

#include "box2d/include/box2d/box2d.h"
#include "Physics/PhysicsWorld.h"

namespace fw
{
    class EventManager;
    class JoltWorldBundle;
    class PhysicsBody;

    class PhysicsWorldBox2D : public PhysicsWorld
    {
    public:
        PhysicsWorldBox2D(EventManager* pEventManager);
        virtual ~PhysicsWorldBox2D();

        virtual void update(float deltaTime) override;
        virtual PhysicsBody* createBoxBody(GameObject* pGameObject, bool dynamic, vec3 position, vec3 boxSize) override;

        b2WorldId getWorldId() { return m_physicsWorld; }

    private:
        b2WorldId m_physicsWorld;
    };
}
