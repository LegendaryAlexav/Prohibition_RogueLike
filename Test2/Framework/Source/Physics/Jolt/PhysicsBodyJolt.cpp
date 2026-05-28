#include "CoreHeaders.h"

#include "Jolt/Jolt.h"
#include "JoltPhysics/Jolt/Physics/Body/Body.h"
#include "JoltPhysics/Jolt/Physics/Body/BodyInterface.h"
#include "JoltPhysics/Jolt/Physics/PhysicsSystem.h"
#include "Math/Matrix.h"
#include "Physics/JoltHelpers.h"
#include "Physics/Jolt/PhysicsBodyJolt.h"
#include "Physics/Jolt/PhysicsWorldJolt.h"
#include "Physics/PhysicsBody.h"
#include "Physics/PhysicsWorld.h"

namespace fw
{
    PhysicsBodyJolt::PhysicsBodyJolt(GameObject* pGameObject, PhysicsWorldJolt* pPhysicsWorld, bool dynamic, vec3 position, vec3 boxSize)
        : m_pPhysicsWorld( pPhysicsWorld )
    {
        m_physicsBody = CreateJoltBodyBox( pGameObject, pPhysicsWorld->getWorld(), position, 0, boxSize, dynamic, 1 );
    }

    PhysicsBodyJolt::PhysicsBodyJolt(GameObject* pGameObject, PhysicsWorldJolt* pPhysicsWorld, bool dynamic, vec3 position, float radius)
        : m_pPhysicsWorld( pPhysicsWorld )
    {
        m_physicsBody = CreateJoltBodySphere( pGameObject, pPhysicsWorld->getWorld(), position, 0, radius, dynamic, 1 );
    }

    PhysicsBodyJolt::PhysicsBodyJolt(GameObject* pGameObject, PhysicsWorldJolt* pPhysicsWorld, bool dynamic, vec3 position, const std::vector<vec3>& positions, int sampleCount)
        : m_pPhysicsWorld( pPhysicsWorld )
    {
        m_physicsBody = CreateJoltBodyTerrain( pGameObject, pPhysicsWorld->getWorld(), position, 0, dynamic, 1, positions, sampleCount );
    }

    PhysicsBodyJolt::~PhysicsBodyJolt()
    {
        DestroyJoltBody( m_pPhysicsWorld->getWorld(), m_physicsBody );
    }

    vec3 PhysicsBodyJolt::getPosition()
    {
        JPH::RVec3 bodyPos = m_physicsBody->GetPosition();
        vec3 pos( bodyPos.GetX(), bodyPos.GetY(), bodyPos.GetZ() );

        return pos;
    }

    vec3 PhysicsBodyJolt::getRotation()
    {
        JPH::Mat44 world = m_physicsBody->GetWorldTransform().GetRotationSafe();
        mat4 rm = mat4( world(0, 0), world(1, 0), world(2, 0), world(3, 0),
                        world(0, 1), world(1, 1), world(2, 1), world(3, 1),
                        world(0, 2), world(1, 2), world(2, 2), world(3, 2),
                        world(0, 3), world(1, 3), world(2, 3), world(3, 3) );
        vec3 rot = rm.getEulerAngles();

        return rot;
    }

    void PhysicsBodyJolt::setPosition(vec3 pos)
    {
        JPH::BodyInterface& bodyInterface = m_pPhysicsWorld->getWorld()->GetBodyInterface();
        bodyInterface.SetPosition( m_physicsBody->GetID(), pos, JPH::EActivation::Activate );
    }

    void PhysicsBodyJolt::setRotation(vec3 rot)
    {
        mat4 rotMat;
        rotMat.createRotation( rot );
        JPH::Mat44 world = JPH::Mat44( JPH::Vec4(rotMat.m11, rotMat.m12, rotMat.m13, rotMat.m14),
                                       JPH::Vec4(rotMat.m21, rotMat.m22, rotMat.m23, rotMat.m24),
                                       JPH::Vec4(rotMat.m31, rotMat.m32, rotMat.m33, rotMat.m34),
                                       JPH::Vec4(rotMat.m41, rotMat.m42, rotMat.m43, rotMat.m44) );
        JPH::Quat quat = world.GetQuaternion();
        JPH::BodyInterface& bodyInterface = m_pPhysicsWorld->getWorld()->GetBodyInterface();
        bodyInterface.SetRotation( m_physicsBody->GetID(), quat, JPH::EActivation::Activate );
    }

    void PhysicsBodyJolt::applyForceToCenter(vec3 force)
    {
        JPH::BodyInterface& bodyInterface = m_pPhysicsWorld->getWorld()->GetBodyInterface();
        bodyInterface.AddForce( m_physicsBody->GetID(), force );
    }

    void PhysicsBodyJolt::applyImpulseToCenter(vec3 impulse)
    {
        JPH::BodyInterface& bodyInterface = m_pPhysicsWorld->getWorld()->GetBodyInterface();
        bodyInterface.AddImpulse( m_physicsBody->GetID(), impulse );
    }
}
