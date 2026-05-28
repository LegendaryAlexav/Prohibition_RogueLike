#include "CoreHeaders.h"

#include "JoltHelpers.h"
#include "EventSystem/EventManager.h"
#include "EventSystem/Events.h"

#include "Jolt/Jolt.h"
#include "JoltPhysics/Jolt/RegisterTypes.h"
#include "JoltPhysics/Jolt/Core/Factory.h"
#include "JoltPhysics/Jolt/Core/JobSystemSingleThreaded.h"
#include "JoltPhysics/Jolt/Core/TempAllocator.h"
#include "JoltPhysics/Jolt/Physics/Body/Body.h"
#include "JoltPhysics/Jolt/Physics/Body/BodyActivationListener.h"
#include "JoltPhysics/Jolt/Physics/Body/BodyCreationSettings.h"
#include "JoltPhysics/Jolt/Physics/Collision/ContactListener.h"
#include "JoltPhysics/Jolt/Physics/Collision/Shape/BoxShape.h"
#include "JoltPhysics/Jolt/Physics/Collision/Shape/SphereShape.h"
#include "JoltPhysics/Jolt/Physics/Collision/Shape/HeightFieldShape.h"
#include "JoltPhysics/Jolt/Physics/PhysicsSettings.h"
#include "JoltPhysics/Jolt/Physics/PhysicsSystem.h"

namespace JPH
{
    class TempAllocatorImpl;
    class JobSystemSingleThreaded;
}

namespace fw
{
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
        virtual const char * GetBroadPhaseLayerName(JPH::BroadPhaseLayer inLayer) const override
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
            switch( inObject1 )
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
            : m_pEventManager( pEventManager )
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
            GameObject* pObject1 = reinterpret_cast<GameObject*>( inBody1.GetUserData() );
            GameObject* pObject2 = reinterpret_cast<GameObject*>( inBody2.GetUserData() );

            vec3 normal = inManifold.mWorldSpaceNormal;

            CollisionEvent* pEvent = new CollisionEvent( pObject1, pObject2, normal );
            m_pEventManager->addEvent( pEvent );
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
        EventManager* m_pEventManager = nullptr;
    };

    JoltWorldBundle* CreateJoltWorld(EventManager* pEventManager)
    {
        // If we haven't ever initialized Jolt, do it once.
        if( JPH::Factory::sInstance == nullptr )
        {
            JPH::RegisterDefaultAllocator();
            JPH::Factory::sInstance = new JPH::Factory();
            JPH::RegisterTypes();
        }
    
        JoltWorldBundle* pBundle = new JoltWorldBundle;

        pBundle->m_pWorld = new JPH::PhysicsSystem();
        pBundle->m_pTempAllocator = new JPH::TempAllocatorImpl( 10 * 1024 * 1024 );
        pBundle->m_pJobSystem = new JPH::JobSystemSingleThreaded( JPH::cMaxPhysicsJobs );
        pBundle->m_pBroadPhaseLayerInterface = new BPLayerInterfaceImpl();
        pBundle->m_pObjecVsBroadphaseLayerFilter = new ObjectVsBroadPhaseLayerFilterImpl();
        pBundle->m_pObjectVsObjectLayerFilter = new ObjectLayerPairFilterImpl();
        pBundle->m_pContactListener = new MyContactListener( pEventManager );

        BPLayerInterfaceImpl& interface = *pBundle->m_pBroadPhaseLayerInterface;
        ObjectVsBroadPhaseLayerFilterImpl& objectVsBroadphase = *pBundle->m_pObjecVsBroadphaseLayerFilter;
        ObjectLayerPairFilterImpl& objectVsObject = *pBundle->m_pObjectVsObjectLayerFilter;
    
        pBundle->m_pWorld->Init( 1024, 0, 1024, 4096, interface, objectVsBroadphase, objectVsObject );
        pBundle->m_pWorld->SetGravity( JPH::Vec3Arg(0, -9.8f, 0) );
        pBundle->m_pWorld->SetContactListener( pBundle->m_pContactListener );
        
        return pBundle;
    }

    void UpdateJoltWorld(JoltWorldBundle* pBundle, float deltaTime)
    {
        pBundle->m_pWorld->Update( deltaTime, 1, pBundle->m_pTempAllocator, pBundle->m_pJobSystem );
    }

    void DestroyJoltWorld(JoltWorldBundle* pBundle)
    {
        delete pBundle->m_pObjectVsObjectLayerFilter;
        delete pBundle->m_pObjecVsBroadphaseLayerFilter;
        delete pBundle->m_pBroadPhaseLayerInterface;
        delete pBundle->m_pJobSystem;
        delete pBundle->m_pTempAllocator;

        delete pBundle->m_pWorld;

        JPH::UnregisterTypes();
        delete JPH::Factory::sInstance;
        JPH::Factory::sInstance = nullptr;
    }

    JPH::Body* CreateJoltBodyBox(GameObject* pGameObject, JPH::PhysicsSystem* pWorld, vec3 pos, vec3 rot, vec3 scale, bool isDynamic, float density)
    {
        JPH::BoxShapeSettings shapeSettings( JPH::Vec3(scale.x/2, scale.y/2, scale.z/2) );
        shapeSettings.mDensity = density;

        return CreateJoltBody( pGameObject, pWorld, pos, rot, scale, isDynamic, density, &shapeSettings );
    }

    JPH::Body* CreateJoltBodySphere(GameObject* pGameObject, JPH::PhysicsSystem* pWorld, vec3 pos, vec3 rot, float radius, bool isDynamic, float density)
    {
        JPH::SphereShapeSettings shapeSettings( radius );
        shapeSettings.mDensity = density;

        return CreateJoltBody( pGameObject, pWorld, pos, rot, vec3(1), isDynamic, density, &shapeSettings );
    }

    JPH::Body* CreateJoltBodyTerrain(GameObject* pGameObject, JPH::PhysicsSystem* pWorld, vec3 pos, vec3 rot, bool isDynamic, float density, const std::vector<vec3>& positions, int sampleCount)
    {
        std::vector<float> heights;
        for( int i=0; i<sampleCount*sampleCount; i++ )
        {
            heights.push_back( positions.at(i).y );
        }
        float scale = positions.at(1).x - positions.at(0).x;

        JPH::HeightFieldShapeSettings shapeSettings( heights.data(), vec3(0,0,0), vec3(scale,1,scale), sampleCount );

        return CreateJoltBody( pGameObject, pWorld, pos, rot, scale, isDynamic, density, &shapeSettings );
    }

    JPH::Body* CreateJoltBody(GameObject* pGameObject, JPH::PhysicsSystem* pWorld, vec3 pos, vec3 rot, vec3 scale, bool isDynamic, float density, JPH::ShapeSettings* pShapeSettings)
    {
        // Create the shape.
        JPH::ShapeSettings::ShapeResult shapeResult = pShapeSettings->Create();
        JPH::ShapeRefC shape = shapeResult.Get();

        // Setup the body.
        JPH::BodyInterface& bodyInterface = pWorld->GetBodyInterface();
        JPH::EMotionType motionType = isDynamic ? JPH::EMotionType::Dynamic : JPH::EMotionType::Static;
        int objectLayer = isDynamic ? Layers::MOVING : Layers::NON_MOVING;
        JPH::Mat44 rotMat = JPH::Mat44::sRotationY(-rot.y/180*PI) * JPH::Mat44::sRotationX(-rot.x/180*PI) * JPH::Mat44::sRotationZ(-rot.z/180*PI);
        JPH::Quat quat = rotMat.GetQuaternion();
        JPH::BodyCreationSettings bodySettings( shape, JPH::RVec3(pos.x, pos.y, pos.z), quat, motionType, objectLayer );
        bodySettings.mUserData = reinterpret_cast<uint64>( pGameObject );

        // Create the rigid body.
        JPH::Body* pRigidBody = bodyInterface.CreateBody( bodySettings );
        bodyInterface.AddBody( pRigidBody->GetID(), JPH::EActivation::Activate );

        return pRigidBody;
    }

    void DestroyJoltBody(JPH::PhysicsSystem* pWorld, JPH::Body* pBody)
    {
        JPH::BodyInterface& bodyInterface = pWorld->GetBodyInterface();
        bodyInterface.RemoveBody( pBody->GetID() );
        bodyInterface.DestroyBody( pBody->GetID() );
    }
} // namespace fw