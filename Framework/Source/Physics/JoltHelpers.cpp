#include "CoreHeaders.h"

#include "EventSystem/EventManager.h"

#include "JoltHelpers.h"
#include "Jolt/Jolt.h"
#include "JoltPhysics/Jolt/RegisterTypes.h"
#include "JoltPhysics/Jolt/Core/Factory.h"
#include "JoltPhysics/Jolt/Core/JobSystemSingleThreaded.h"
#include "JoltPhysics/Jolt/Core/TempAllocator.h"
#include "JoltPhysics/Jolt/Physics/Body/Body.h"
#include "JoltPhysics/Jolt/Physics/Body/BodyActivationListener.h"
#include "JoltPhysics/Jolt/Physics/Body/BodyCreationSettings.h"
#include "JoltPhysics/Jolt/Physics/Collision/ContactListener.h"

#include "JoltPhysics/Jolt/Physics/Collision/CastResult.h"
#include "JoltPhysics/Jolt/Physics/Collision/NarrowPhaseQuery.h"
#include "JoltPhysics/Jolt/Physics/Collision/RayCast.h"
#include "Jolt/Physics/Collision/BroadPhase/BroadPhaseQuery.h"
#include "Jolt/Physics/Collision/CollisionCollectorImpl.h"

#include "JoltPhysics/Jolt/Physics/Collision/Shape/BoxShape.h"
#include "JoltPhysics/Jolt/Physics/Collision/Shape/SphereShape.h"
#include "JoltPhysics/Jolt/Physics/Collision/Shape/HeightFieldShape.h"
#include "JoltPhysics/Jolt/Physics/PhysicsSettings.h"
#include "JoltPhysics/Jolt/Physics/PhysicsSystem.h"
#include "Resources/Mesh.h"
//#include "../Game/Source/Objects/HeightmapMesh.h"

namespace JPH {
    class TempAllocatorImpl;
    class JobSystemSingleThreaded;
}

namespace fw {

    namespace Layers
    {
        static constexpr JPH::ObjectLayer NON_MOVING = 0;
        static constexpr JPH::ObjectLayer MOVING = 1;
        static constexpr JPH::ObjectLayer NUM_LAYERS = 2;
    };

    namespace BroadPhaseLayers
    {
        static constexpr JPH::BroadPhaseLayer NON_MOVING(0);
        static constexpr JPH::BroadPhaseLayer MOVING(1);
        static constexpr JPH::uint NUM_LAYERS(2);
    };

    // BroadPhaseLayerInterface implementation.
    // This defines a mapping between object and broadphase layers.
    class BPLayerInterfaceImpl final : public JPH::BroadPhaseLayerInterface
    {
    public:
        BPLayerInterfaceImpl()
        {
            // Create a mapping table from object to broad phase layer
            mObjectToBroadPhase[Layers::NON_MOVING] = BroadPhaseLayers::NON_MOVING;
            mObjectToBroadPhase[Layers::MOVING] = BroadPhaseLayers::MOVING;
        }

        virtual JPH::uint GetNumBroadPhaseLayers() const override
        {
            return BroadPhaseLayers::NUM_LAYERS;
        }

        virtual JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const override
        {
            JPH_ASSERT(inLayer < Layers::NUM_LAYERS);
            return mObjectToBroadPhase[inLayer];
        }

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
        virtual const char* GetBroadPhaseLayerName(JPH::BroadPhaseLayer inLayer) const override
        {
            switch ((JPH::BroadPhaseLayer::Type)inLayer)
            {
            case (JPH::BroadPhaseLayer::Type)BroadPhaseLayers::NON_MOVING:  return "NON_MOVING";
            case (JPH::BroadPhaseLayer::Type)BroadPhaseLayers::MOVING:      return "MOVING";
            default:                                                        JPH_ASSERT(false); return "INVALID";
            }
        }
#endif // JPH_EXTERNAL_PROFILE || JPH_PROFILE_ENABLED

    private:
        JPH::BroadPhaseLayer mObjectToBroadPhase[Layers::NUM_LAYERS];
    };

    /// Class that determines if two object layers can collide.
    class ObjectLayerPairFilterImpl : public JPH::ObjectLayerPairFilter
    {
    public:
        virtual bool ShouldCollide(JPH::ObjectLayer inObject1, JPH::ObjectLayer inObject2) const override
        {
            switch (inObject1)
            {
            case Layers::NON_MOVING:
                return inObject2 == Layers::MOVING; // Non moving only collides with moving
            case Layers::MOVING:
                return true; // Moving collides with everything
            default:
                JPH_ASSERT(false);
                return false;
            }
        }
    };

    // Class that determines if an object layer can collide with a broadphase layer.
    class ObjectVsBroadPhaseLayerFilterImpl : public JPH::ObjectVsBroadPhaseLayerFilter
    {
    public:
        virtual bool ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const override
        {
            switch (inLayer1)
            {
            case Layers::NON_MOVING:
                return inLayer2 == BroadPhaseLayers::MOVING;
            case Layers::MOVING:
                return true;
            default:
                JPH_ASSERT(false);
                return false;
            }
        }
    };

    // An example contact listener.
    class MyContactListener : public JPH::ContactListener
    {
    public:
        MyContactListener(EventManager* pEventManager)
            : m_pEventManager(pEventManager)
        {
        }

        // See: ContactListener
        virtual JPH::ValidateResult OnContactValidate(const JPH::Body& inBody1, const JPH::Body& inBody2, JPH::RVec3Arg inBaseOffset, const JPH::CollideShapeResult& inCollisionResult) override
        {
            //cout << "Contact validate callback" << endl;

            // Allows you to ignore a contact before it is created (using layers to not make objects collide is cheaper!)
            return JPH::ValidateResult::AcceptAllContactsForThisBodyPair;
        }

        virtual void OnContactAdded(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings) override
        {
            //cout << "A contact was added" << endl;
            GameObject* pObject1 = reinterpret_cast<GameObject*>(inBody1.GetUserData());
            GameObject* pObject2 = reinterpret_cast<GameObject*>(inBody2.GetUserData());
            
            CollisionEvent* pEvent = new CollisionEvent(pObject1, pObject2);
            m_pEventManager->addEvent(pEvent);
        }

        virtual void OnContactPersisted(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings) override
        {
            //cout << "A contact was persisted" << endl;
        }

        virtual void OnContactRemoved(const JPH::SubShapeIDPair& inSubShapePair) override
        {
            //cout << "A contact was removed" << endl;
        }

    protected:
        EventManager* m_pEventManager;
    };

    JoltWorldBundle* CreateJoltWorld(EventManager* pEventManager)
    {
        // If we haven't ever initialized Jolt, do it once.
        if (JPH::Factory::sInstance == nullptr)
        {
            JPH::RegisterDefaultAllocator();
            JPH::Factory::sInstance = new JPH::Factory();
            JPH::RegisterTypes();
        }

        JoltWorldBundle* pBundle = new JoltWorldBundle;

        pBundle->m_pWorld = new JPH::PhysicsSystem();
        pBundle->m_pTempAllocator = new JPH::TempAllocatorImpl(10 * 1024 * 1024);
        pBundle->m_pJobSystem = new JPH::JobSystemSingleThreaded(JPH::cMaxPhysicsJobs);
        pBundle->m_narrowPhaseQuery = new JPH::NarrowPhaseQuery();
        pBundle->m_pBroadPhaseLayerInterface = new BPLayerInterfaceImpl();
        pBundle->m_pObjecVsBroadphaseLayerFilter = new ObjectVsBroadPhaseLayerFilterImpl();
        pBundle->m_pObjectVsObjectLayerFilter = new ObjectLayerPairFilterImpl();
        pBundle->m_pContactListener = new MyContactListener(pEventManager);


        BPLayerInterfaceImpl& interface = *pBundle->m_pBroadPhaseLayerInterface;
        ObjectVsBroadPhaseLayerFilterImpl& objectVsBroadphase = *pBundle->m_pObjecVsBroadphaseLayerFilter;
        ObjectLayerPairFilterImpl& objectVsObject = *pBundle->m_pObjectVsObjectLayerFilter;

        pBundle->m_pWorld->Init(1024, 0, 1024, 4096, interface, objectVsBroadphase, objectVsObject);
        pBundle->m_pWorld->SetGravity(JPH::Vec3Arg(0, -9.8f, 0));
        pBundle->m_pWorld->SetContactListener( pBundle->m_pContactListener );

        return pBundle;
    }

    void UpdateJoltWorld(JoltWorldBundle* pBundle, float deltaTime)
    {
        pBundle->m_pWorld->Update(deltaTime, 1, pBundle->m_pTempAllocator, pBundle->m_pJobSystem);
    }

    void DestroyJoltWorld(JoltWorldBundle* pBundle)
    {
        delete pBundle->m_pObjectVsObjectLayerFilter;
        delete pBundle->m_pObjecVsBroadphaseLayerFilter;
        delete pBundle->m_pBroadPhaseLayerInterface;
        delete pBundle->m_pJobSystem;
        delete pBundle->m_pTempAllocator;
        delete pBundle->m_narrowPhaseQuery;

        delete pBundle->m_pWorld;

        JPH::UnregisterTypes();
        delete JPH::Factory::sInstance;
        JPH::Factory::sInstance = nullptr;
    }

    JPH::Body* CreateJoltBody(GameObject* pGameObject, Mesh* pMesh, JPH::PhysicsSystem* pWorld, EPhysicsShapeType shapeType, vec3 pos, vec3 rot, vec3 scale, bool isDynamic, float density)
    {
        // Create the shape.
        JPH::ShapeRefC shape;
        JPH::ShapeSettings* shapeSettings = nullptr;
        switch (shapeType)
        {
        case EPhysicsShapeType::BoxShape:
            {
                shapeSettings = new JPH::BoxShapeSettings(JPH::Vec3(scale.x / 2, scale.y / 2, scale.x / 2));
            }
            break;
        case EPhysicsShapeType::SphereShape:
            {
                shapeSettings = new JPH::SphereShapeSettings(scale.x / 2);
            }
            break;
        case EPhysicsShapeType::HeightFieldShape:
            {
                
                ivec2 vertCount = pMesh->getVertCount();
                vec2 worldSize = pMesh->getWorldSize();
                const float* points = pMesh->getYPositions();

                assert(points);
                
                JPH::Vec3 tileScale = { worldSize.x / vertCount.x, 1, worldSize.y / vertCount.y };
                uint32 sampleCount = vertCount.x;
                
                shapeSettings = new JPH::HeightFieldShapeSettings(points, JPH::Vec3(0,0,0), tileScale, sampleCount);
            }
            break;
        }
        
        assert(shapeSettings);

        JPH::ShapeSettings::ShapeResult shapeResult = shapeSettings->Create();
        shape = shapeResult.Get();


        // Setup the body.
        JPH::BodyInterface& bodyInterface = pWorld->GetBodyInterface();
        JPH::EMotionType motionType = isDynamic ? JPH::EMotionType::Dynamic : JPH::EMotionType::Static;
        int objectLayer = isDynamic ? Layers::MOVING : Layers::NON_MOVING;
        JPH::Mat44 rotMat = JPH::Mat44::sRotationY(-rot.y / 180 * PI) * JPH::Mat44::sRotationX(-rot.x / 180 * PI) * JPH::Mat44::sRotationZ(-rot.z / 180 * PI);
        JPH::Quat quat = rotMat.GetQuaternion();
        JPH::BodyCreationSettings bodySettings(shape, JPH::RVec3(pos.x, pos.y, pos.z), quat, motionType, objectLayer);
        // reinterpret_cast is VERY dangerous, but will convert 1-1 withought changing from one type to another, this is usefull here since uint64 and a pointer are the same length and both ints, just one is an address
        bodySettings.mUserData = reinterpret_cast<uint64>(pGameObject); 

        // Create the rigid body.
        JPH::Body* pRigidBody = bodyInterface.CreateBody(bodySettings);
        bodyInterface.AddBody(pRigidBody->GetID(), JPH::EActivation::Activate);

        return pRigidBody;
    }

    void DestroyJoltBody(JPH::PhysicsSystem* pWorld, JPH::Body* pBody)
    {
        JPH::BodyInterface& bodyInterface = pWorld->GetBodyInterface();
        bodyInterface.RemoveBody(pBody->GetID());
        bodyInterface.DestroyBody(pBody->GetID());
    }

    GameObject* RayCast(JoltWorldBundle* pBundle, vec3 origin, vec3 dir)
    {
        GameObject* pNearestObject = nullptr;

        const JPH::BroadPhaseQuery& broadPhase = pBundle->m_pWorld->GetBroadPhaseQuery();

        JPH::RayCast ray(origin, dir);
        JPH::ClosestHitCollisionCollector<JPH::RayCastBodyCollector> collector;
        broadPhase.CastRay(ray, collector);

        JPH::BodyID bodyid = collector.mHit.mBodyID;
        JPH::BodyInterface& bodyInterface = pBundle->m_pWorld->GetBodyInterface();
        pNearestObject = reinterpret_cast<GameObject*>(bodyInterface.GetUserData(bodyid));

        return pNearestObject;

        // Different way of 
        //JPH::RRayCast ray(JPH::Vec3(origin.x, origin.y, origin.z), JPH::Vec3(dir.x, dir.y, dir.z));
        //
        //JPH::RayCastResult result;
        //
        //bool isHit = pBundle->m_narrowPhaseQuery->CastRay(ray, result);
        //if (isHit)
        //{
        //    JPH::BodyInterface& bodyInterface = pBundle->m_pWorld->GetBodyInterface();
        //    uint64 userData = bodyInterface.GetUserData(result.mBodyID);
        //    return reinterpret_cast<GameObject*>(userData);
        //}
        //
        //return nullptr;
    }

} // namespace fw