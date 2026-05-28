#include "CoreHeaders.h"

#include "box2d/include/box2d/box2d.h"
#include "Objects/GameObject.h"
#include "Physics/JoltHelpers.h"
#include "Physics/PhysicsBody.h"
#include "PhysicsWorld.h"
#include "Resources/Mesh.h"

namespace fw
{
    PhysicsWorld::PhysicsWorld(PhysicsLibraries physicsLibType)
        : m_physicsLibType(physicsLibType)
    {
    }

#pragma region - Jolt Physics -

    PhysicsWorldJolt::PhysicsWorldJolt(EventManager* pEventManager)
        : PhysicsWorld(PhysicsLibraries::Jolt)
    {
        m_physicsWorld = CreateJoltWorld(pEventManager);
    }

    PhysicsWorldJolt::~PhysicsWorldJolt()
    {
        DestroyJoltWorld(m_physicsWorld);
    }

    PhysicsBody* PhysicsWorldJolt::createPhysicsBody(GameObject* pGameObject, PhysicsWorld* pPhysicsWorld, EPhysicsShapeType shapeType, bool dynamic, vec3 position, vec3 size)
    {
        PhysicsWorldJolt* pPhysicsWorldJolt = dynamic_cast<PhysicsWorldJolt*>(pPhysicsWorld);
        return new PhysicsBodyJolt(pGameObject, pGameObject->getMesh(), pPhysicsWorldJolt, shapeType, dynamic, position, size);
    }

    void PhysicsWorldJolt::update(float deltaTime)
    {
        UpdateJoltWorld(m_physicsWorld, deltaTime);
    }

    GameObject* PhysicsWorldJolt::rayCastNearestObject(vec3 location, vec3 dir)
    {
        return RayCast(m_physicsWorld, location, dir);
    }

#pragma endregion

#pragma region - Box2D Physics -

    PhysicsWorldBox2D::PhysicsWorldBox2D()
        : PhysicsWorld(PhysicsLibraries::Box2D)
    {
        b2WorldDef worldDef = b2DefaultWorldDef();
        m_physicsWorld = b2CreateWorld( &worldDef );
    }

    PhysicsWorldBox2D::~PhysicsWorldBox2D()
    {
        b2DestroyWorld( m_physicsWorld );
    }

    PhysicsBody* PhysicsWorldBox2D::createPhysicsBody(GameObject* pGameObject, PhysicsWorld* pPhysicsWorld, EPhysicsShapeType shapeType, bool dynamic, vec3 position, vec3 size)
    {
        PhysicsWorldBox2D* pPhysicsWorldBox2D = dynamic_cast<PhysicsWorldBox2D*>(pPhysicsWorld);
        return new PhysicsBodyBox2D(pGameObject, pPhysicsWorldBox2D, dynamic, position, size);
    }

    void PhysicsWorldBox2D::update(float deltaTime)
    {
        b2World_Step(m_physicsWorld, deltaTime, 4);
    }

    GameObject* PhysicsWorldBox2D::rayCastNearestObject(vec3 location, vec3 dir)
    {
        return nullptr;
    }

#pragma endregion

}