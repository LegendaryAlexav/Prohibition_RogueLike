#pragma once

#include "Math/Vector.h"

namespace JPH {
    class PhysicsSystem;
    class TempAllocatorImpl;
    class JobSystemSingleThreaded;
    class NarrowPhaseQuery;
    class Body;
}

namespace fw {

    class BPLayerInterfaceImpl;
    class MyContactListener;
    class GameObject;
    class Mesh;
    class EventManager;
    class ObjectVsBroadPhaseLayerFilterImpl;
    class ObjectLayerPairFilterImpl;

    enum class EPhysicsShapeType
    {
        BoxShape,
        SphereShape,
        HeightFieldShape
    };

    class JoltWorldBundle
    {
    public:
        JPH::PhysicsSystem* m_pWorld = nullptr;
        JPH::JobSystemSingleThreaded* m_pJobSystem = nullptr;
        JPH::TempAllocatorImpl* m_pTempAllocator = nullptr;
        JPH::NarrowPhaseQuery* m_narrowPhaseQuery = nullptr;

        BPLayerInterfaceImpl* m_pBroadPhaseLayerInterface = nullptr;
        ObjectVsBroadPhaseLayerFilterImpl* m_pObjecVsBroadphaseLayerFilter = nullptr;
        ObjectLayerPairFilterImpl* m_pObjectVsObjectLayerFilter = nullptr;

        MyContactListener* m_pContactListener = nullptr;

    };

    JoltWorldBundle* CreateJoltWorld(EventManager* pEventManager);
    void UpdateJoltWorld(JoltWorldBundle* pBundle, float deltaTime);
    void DestroyJoltWorld(JoltWorldBundle* pBundle);

    JPH::Body* CreateJoltBody(GameObject* pGameObject, Mesh* pMesh, JPH::PhysicsSystem* pWorld, EPhysicsShapeType shapeType, vec3 pos, vec3 rot, vec3 scale, bool isDynamic, float density);
    void DestroyJoltBody(JPH::PhysicsSystem* pWorld, JPH::Body* pBody);

    GameObject* RayCast(JoltWorldBundle* pBundle, vec3 origin, vec3 dir);

} // namespace fw