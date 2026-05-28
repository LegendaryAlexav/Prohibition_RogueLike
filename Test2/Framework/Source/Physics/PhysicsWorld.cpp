#include "CoreHeaders.h"

#include "Physics/Box2D/PhysicsWorldBox2D.h"
#include "Physics/Jolt/PhysicsWorldJolt.h"
#include "Physics/PhysicsWorld.h"

namespace fw
{
    PhysicsWorld* createPhysicsWorld(PhysicsEngine engine, EventManager* pEventManager)
    {
        switch( engine )
        {
        case PhysicsEngine::Box2D:
            return new PhysicsWorldBox2D( pEventManager );
        case PhysicsEngine::Jolt:
            return new PhysicsWorldJolt( pEventManager );
        default:
            return nullptr;
        }
    }
}
