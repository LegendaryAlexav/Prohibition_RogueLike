#include "CoreHeaders.h"

#include "Jolt/Jolt.h"
#include "Jolt/Physics/Collision/BroadPhase/BroadPhaseQuery.h"
#include "Jolt/Physics/Collision/CastResult.h"
#include "Jolt/Physics/Collision/CollisionCollectorImpl.h"
#include "Jolt/Physics/Collision/RayCast.h"
#include "Jolt/Physics/PhysicsSystem.h"
#include "Physics/JoltHelpers.h"
#include "Physics/Jolt/PhysicsBodyJolt.h"
#include "Physics/Jolt/PhysicsWorldJolt.h"
#include "Physics/PhysicsWorld.h"

namespace fw
{
    PhysicsWorldJolt::PhysicsWorldJolt(EventManager* pEventManager)
    {
        m_physicsWorldBundle = CreateJoltWorld( pEventManager );
    }

    PhysicsWorldJolt::~PhysicsWorldJolt()
    {
        DestroyJoltWorld( m_physicsWorldBundle );
    }

    void PhysicsWorldJolt::update(float deltaTime)
    {
        UpdateJoltWorld( m_physicsWorldBundle, deltaTime );
    }

    JoltWorldBundle* PhysicsWorldJolt::getWorldBundle()
    {
        return m_physicsWorldBundle;
    }

    JPH::PhysicsSystem* PhysicsWorldJolt::getWorld()
    {
        return m_physicsWorldBundle->m_pWorld;
    }

    PhysicsBody* PhysicsWorldJolt::createBoxBody(GameObject* pGameObject, bool dynamic, vec3 position, vec3 boxSize)
    {
        PhysicsBodyJolt* pBody = new PhysicsBodyJolt( pGameObject, this, dynamic, position, boxSize );
        return pBody;
    }

    PhysicsBody* PhysicsWorldJolt::createSphereBody(GameObject* pGameObject, bool dynamic, vec3 position, float radius)
    {
        PhysicsBodyJolt* pBody = new PhysicsBodyJolt( pGameObject, this, dynamic, position, radius );
        return pBody;
    }

    PhysicsBody* PhysicsWorldJolt::createTerrainBody(GameObject* pGameObject, bool dynamic, vec3 position, const std::vector<vec3>& positions, int sampleCount)
    {
        PhysicsBodyJolt* pBody = new PhysicsBodyJolt( pGameObject, this, dynamic, position, positions, sampleCount );
        return pBody;
    }

    GameObject* PhysicsWorldJolt::raycastForNearestObject(vec3 start, vec3 end)
    {
        GameObject* pNearestObject = nullptr;

        const JPH::BroadPhaseQuery& broadPhase = m_physicsWorldBundle->m_pWorld->GetBroadPhaseQuery();

        JPH::RayCast ray( start, end-start );
        JPH::ClosestHitCollisionCollector<JPH::RayCastBodyCollector> collector;
        broadPhase.CastRay( ray, collector );

        JPH::BodyID bodyid = collector.mHit.mBodyID;
        JPH::BodyInterface& bodyInterface = m_physicsWorldBundle->m_pWorld->GetBodyInterface();
        pNearestObject = reinterpret_cast<GameObject*>( bodyInterface.GetUserData(bodyid) );

        return pNearestObject;
    }

    std::vector<GameObject*> PhysicsWorldJolt::raycastForAllObjects(vec3 start, vec3 end)
    {
        return {};
    }
}
