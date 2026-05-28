#pragma once

#include "Math/Vector.h"

namespace JPH
{
    class Body;
    class JobSystemSingleThreaded;
    class PhysicsSystem;
    class ShapeSettings;
    class TempAllocatorImpl;
}

namespace fw
{
    class BPLayerInterfaceImpl;
    class EventManager;
    class GameObject;
    class MyContactListener;
    class ObjectVsBroadPhaseLayerFilterImpl;
    class ObjectLayerPairFilterImpl;

    class JoltWorldBundle
    {
    public:
        JPH::PhysicsSystem* m_pWorld = nullptr;
        JPH::JobSystemSingleThreaded* m_pJobSystem = nullptr;
        JPH::TempAllocatorImpl* m_pTempAllocator = nullptr;

        BPLayerInterfaceImpl* m_pBroadPhaseLayerInterface = nullptr;
        ObjectVsBroadPhaseLayerFilterImpl* m_pObjecVsBroadphaseLayerFilter = nullptr;
        ObjectLayerPairFilterImpl* m_pObjectVsObjectLayerFilter = nullptr;

        MyContactListener* m_pContactListener = nullptr;
    };

    JoltWorldBundle* CreateJoltWorld(EventManager* pEventManager);
    void UpdateJoltWorld(JoltWorldBundle* pBundle, float deltaTime);
    void DestroyJoltWorld(JoltWorldBundle* pBundle);

    JPH::Body* CreateJoltBodyBox(GameObject* pGameObject, JPH::PhysicsSystem* pWorld, vec3 pos,
                              vec3 rot, vec3 scale, bool isDynamic, float density);
    JPH::Body* CreateJoltBodySphere(GameObject* pGameObject, JPH::PhysicsSystem* pWorld, vec3 pos,
                              vec3 rot, float radius, bool isDynamic, float density);
    JPH::Body* CreateJoltBodyTerrain(GameObject* pGameObject, JPH::PhysicsSystem* pWorld, vec3 pos,
                              vec3 rot, bool isDynamic, float density, const std::vector<vec3>& positions, int sampleCount);
    JPH::Body* CreateJoltBody(GameObject* pGameObject, JPH::PhysicsSystem* pWorld, vec3 pos,
                              vec3 rot, vec3 scale, bool isDynamic, float density, JPH::ShapeSettings* pShapeSettings);
    void DestroyJoltBody(JPH::PhysicsSystem* pWorld, JPH::Body* pBody);
} // namespace fw
