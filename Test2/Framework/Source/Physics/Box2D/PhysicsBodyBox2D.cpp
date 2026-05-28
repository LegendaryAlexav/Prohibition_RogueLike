#include "CoreHeaders.h"

#include "box2d/include/box2d/box2d.h"
#include "Math/Matrix.h"
#include "Physics/Box2D/PhysicsBodyBox2D.h"
#include "Physics/Box2D/PhysicsWorldBox2D.h"
#include "Physics/PhysicsBody.h"
#include "Physics/PhysicsWorld.h"

namespace fw
{
    PhysicsBodyBox2D::PhysicsBodyBox2D(GameObject* pGameObject, PhysicsWorldBox2D* pPhysicsWorld, bool dynamic, vec3 position, vec3 boxSize)
        : m_pPhysicsWorld( pPhysicsWorld )
    {
        b2BodyDef groundBodyDef = b2DefaultBodyDef();
        groundBodyDef.position = { position.x, position.y };
        if( dynamic )
        {
            groundBodyDef.type = b2_dynamicBody;
        }
        m_physicsBody = b2CreateBody( pPhysicsWorld->getWorldId(), &groundBodyDef );

        b2Polygon polygon = b2MakeBox( boxSize.x/2, boxSize.y/2 );
        b2ShapeDef shapeDef = b2DefaultShapeDef();
        b2CreatePolygonShape( m_physicsBody, &shapeDef, &polygon );
    }

    PhysicsBodyBox2D::~PhysicsBodyBox2D()
    {
        b2DestroyBody( m_physicsBody );
    }

    vec3 PhysicsBodyBox2D::getPosition()
    {
        assert( b2Body_IsValid(m_physicsBody) );

        b2Vec2 position = b2Body_GetPosition( m_physicsBody );
        return vec3( position.x, position.y, 0 );
    }

    vec3 PhysicsBodyBox2D::getRotation()
    {
        assert( b2Body_IsValid(m_physicsBody) );

        b2Rot rotation = b2Body_GetRotation( m_physicsBody );
        float angle = b2Rot_GetAngle( rotation );
        angle = -angle * 180.0f / PI;
        return vec3( 0, 0, angle );
    }

    void PhysicsBodyBox2D::setPosition(vec3 pos)
    {
        b2Rot rotation = b2Body_GetRotation( m_physicsBody );
        b2Body_SetTransform( m_physicsBody, { pos.x, pos.y }, rotation );
    }

    void PhysicsBodyBox2D::setRotation(vec3 rot)
    {
        b2Vec2 position = b2Body_GetPosition( m_physicsBody );
        float angle = -rot.z * PI / 180.0f;
        b2Rot rotation = b2MakeRot( angle );
        b2Body_SetTransform( m_physicsBody, position, rotation );
    }

    void PhysicsBodyBox2D::applyForceToCenter(vec3 force)
    {
        assert( b2Body_IsValid(m_physicsBody) );
        b2Body_ApplyForceToCenter( m_physicsBody, { force.x, force.y }, true );
    }

    void PhysicsBodyBox2D::applyImpulseToCenter(vec3 impulse)
    {
        assert( b2Body_IsValid(m_physicsBody) );
        b2Body_ApplyLinearImpulseToCenter( m_physicsBody, { impulse.x, impulse.y }, true );
    }
}
