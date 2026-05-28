#include "CoreHeaders.h"

#include "box2d/include/box2d/box2d.h"
#include "Physics/Box2D/PhysicsBodyBox2D.h"
#include "Physics/Box2D/PhysicsWorldBox2D.h"

namespace fw
{
    PhysicsWorldBox2D::PhysicsWorldBox2D(EventManager* pEventManager)
    {
        b2WorldDef worldDef = b2DefaultWorldDef();
        m_physicsWorld = b2CreateWorld( &worldDef );
    }

    PhysicsWorldBox2D::~PhysicsWorldBox2D()
    {
        b2DestroyWorld( m_physicsWorld );
    }

    void PhysicsWorldBox2D::update(float deltaTime)
    {
        b2World_Step( m_physicsWorld, deltaTime, 4 );
    }

    PhysicsBody* PhysicsWorldBox2D::createBoxBody(GameObject* pGameObject, bool dynamic, vec3 position, vec3 boxSize)
    {
        PhysicsBodyBox2D* pBody = new PhysicsBodyBox2D( pGameObject, this, dynamic, position, boxSize );
        return pBody;
    }
}
