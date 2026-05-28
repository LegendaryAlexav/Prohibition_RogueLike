#include "CoreHeaders.h"

#include "box2d/include/box2d/box2d.h"
#include "Jolt/Jolt.h"
#include "JoltPhysics/Jolt/Physics/Body/Body.h"
#include "JoltPhysics/Jolt/Physics/Body/BodyInterface.h"
#include "JoltPhysics/Jolt/Physics/PhysicsSystem.h"
#include "Math/Matrix.h"
#include "PhysicsBody.h"
#include "PhysicsWorld.h"

namespace fw
{
    PhysicsBody::PhysicsBody()
    {
    }

#pragma region - Jolt Physics -

    PhysicsBodyJolt::PhysicsBodyJolt(GameObject* pGameObject, Mesh* pMesh, PhysicsWorldJolt* pPhysicsWorld, EPhysicsShapeType shapeType, bool dynamic, vec3 position, vec3 size)
        : PhysicsBody()
        , m_pPhysicsWorld(pPhysicsWorld)
    {
        m_pPhysicsBody = CreateJoltBody(pGameObject, pMesh, pPhysicsWorld->getWorld()->m_pWorld, shapeType, position, 0, size, dynamic, 1);
    }

    PhysicsBodyJolt::~PhysicsBodyJolt()
    {
        DestroyJoltBody(m_pPhysicsWorld->getWorld()->m_pWorld, m_pPhysicsBody);
    }

    vec3 PhysicsBodyJolt::getPosition()
    {
        JPH::RVec3 bodyPos = m_pPhysicsBody->GetPosition();
        vec3 pos(bodyPos.GetX(), bodyPos.GetY(), bodyPos.GetZ());

        return pos;
    }

    vec3 PhysicsBodyJolt::getRotation()
    {
        JPH::Mat44 world = m_pPhysicsBody->GetWorldTransform().GetRotationSafe();
        mat4 rm = mat4(world(0, 0), world(1, 0), world(2, 0), world(3, 0),
            world(0, 1), world(1, 1), world(2, 1), world(3, 1),
            world(0, 2), world(1, 2), world(2, 2), world(3, 2),
            world(0, 3), world(1, 3), world(2, 3), world(3, 3));
        vec3 rot = rm.getEulerAngles();

        return rot;
    }

    void PhysicsBodyJolt::applyForceToCenter(vec3 force)
    {
        JPH::Vec3 joltForce = JPH::Vec3(force.x, force.y, force.z) * 1000;

        JPH::BodyInterface& bodyInterface = m_pPhysicsWorld->getWorld()->m_pWorld->GetBodyInterface();
        bodyInterface.AddForce(m_pPhysicsBody->GetID(), joltForce);
    }

    void PhysicsBodyJolt::applyTorque(vec3 force)
    {
        JPH::Vec3 joltForce = JPH::Vec3(force.x, force.y, force.z) * 1000;

        JPH::BodyInterface& bodyInterface = m_pPhysicsWorld->getWorld()->m_pWorld->GetBodyInterface();
        bodyInterface.AddTorque(m_pPhysicsBody->GetID(), joltForce);
    }


    
#pragma endregion

#pragma region - Box2D Physics -

    PhysicsBodyBox2D::PhysicsBodyBox2D(GameObject* pGameObject, PhysicsWorldBox2D* pPhysicsWorld, bool dynamic, vec3 position, vec3 boxSize)
        : PhysicsBody()
        , m_pPhysicsWorld(pPhysicsWorld)
    {
        b2BodyDef groundBodyDef = b2DefaultBodyDef();
        groundBodyDef.position = { position.x, position.y };
        if (dynamic)
        {
            groundBodyDef.type = b2_dynamicBody;
        }
        m_physicsBody = b2CreateBody(m_pPhysicsWorld->getWorldId(), &groundBodyDef);

        b2Polygon polygon = b2MakeBox(boxSize.x / 2, boxSize.y / 2);
        b2ShapeDef shapeDef = b2DefaultShapeDef();
        b2CreatePolygonShape(m_physicsBody, &shapeDef, &polygon);
    }

    PhysicsBodyBox2D::~PhysicsBodyBox2D()
    {
        b2DestroyBody(m_physicsBody);
    }

    vec3 PhysicsBodyBox2D::getPosition()
    {
        assert( b2Body_IsValid(m_physicsBody) );

        b2Vec2 position = b2Body_GetPosition( m_physicsBody );
        return vec3( position.x, position.y, 0 );
    }

    vec3 PhysicsBodyBox2D::getRotation()
    {
        assert(b2Body_IsValid(m_physicsBody));

        b2Rot rotation = b2Body_GetRotation(m_physicsBody);
        float angle = b2Rot_GetAngle(rotation);
        angle = -angle * 180.0f / PI;
        return vec3(0, 0, angle);
    }

    void PhysicsBodyBox2D::applyForceToCenter(vec3 force)
    {
        assert( b2Body_IsValid(m_physicsBody) );
        b2Body_ApplyForceToCenter( m_physicsBody, { force.x, force.y }, true );
    }

    void PhysicsBodyBox2D::applyTorque(vec3 force)
    {
        b2Vec2 topLeftPoint = b2Body_GetWorldPoint(m_physicsBody, b2Vec2(-1, 1));
        b2Body_ApplyForce(m_physicsBody, { force.x, force.y }, topLeftPoint, true);
    }

#pragma endregion

}