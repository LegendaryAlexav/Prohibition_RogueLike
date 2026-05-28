#include "Framework.h"

#include "Events/GameEvents.h"
#include "Game.h"
#include "GameCore.h"
#include "Objects/Deer.h"
#include "HeightmapMesh.h"


Deer::Deer(Game* pGame, std::string name, vec3 pos, fw::Mesh* pMesh, fw::Material* pMaterial)
    : fw::GameObject(pGame, name, pos, pMesh, pMaterial)
    , m_collisionEventListener(std::bind(&Deer::onCollisionEvent, this, std::placeholders::_1))
    , m_soundEventListener(std::bind(&Deer::onSoundPlayedEvent, this, std::placeholders::_1))
{
    pGame->getEventManager()->registerListener<fw::CollisionEvent>(&m_collisionEventListener);
    pGame->getEventManager()->registerListener<SoundPlayedEvent>(&m_soundEventListener);
}

Deer::~Deer()
{
    m_pGameCore->getEventManager()->unregisterListener<fw::CollisionEvent>(&m_collisionEventListener);
    m_pGameCore->getEventManager()->unregisterListener<SoundPlayedEvent>(&m_soundEventListener);
}

void Deer::update(float deltaTime)
{
    GameObject::update(deltaTime);

    // After syncing with physics, lower the mesh position a bit, since the deer's pivot is at the bottom center.
    //m_position.y -= 0.25f;

    // Raycast from below up to our position to see if we're still on ground, disable jumping if the ray hits us.
    vec3 rayStart = m_position + vec3(0, -0.5f, 0);
    vec3 rayEnd = m_position;
    fw::GameObject* pHitObject = m_pGameCore->getPhysicsWorld()->raycastForNearestObject(rayStart, rayEnd);
    if (pHitObject == this)
    {
        m_canJump = false;
    }

    // Deal with deer movement.
    float speed = 0.2f;
    if (m_scaredTimer > 0.0f)
    {
        m_scaredTimer -= deltaTime;
        speed = 2.0f;
    }

    m_movementTimer -= deltaTime;
    m_jumpTimer -= deltaTime;

    if (m_movementTimer <= 0.0f) // when the timer is over, restart it and choose a new movement direction
    {
        m_movementTimer = fw::Random::randomFloat(1.0f, 2.0f);
        float randomX = fw::Random::randomFloat(-1.0f, 1.0f);
        float randomY = fw::Random::randomFloat(-1.0f, 1.0f);
        m_controls = { randomX , randomY };
    }
    if (m_jumpTimer <= 0.0f && m_canJump)
    {
        m_jumpTimer = fw::Random::randomFloat(1.0f, 2.0f);

        m_pPhysicsBody->applyImpulseToCenter(vec3(0, 0.5f, 0));
        m_canJump = false;
    }

    vec3 dir = { m_controls.x, 0, m_controls.y };
    dir.normalize();

    m_pPhysicsBody->applyForceToCenter(dir * speed);
}

void Deer::onCollisionEvent(fw::Event* pEvent)
{
    fw::CollisionEvent* pCollisionEvent = static_cast<fw::CollisionEvent*>(pEvent);

    vec3 normal;
    if (pCollisionEvent->getObject1() == this)
    {
        normal = pCollisionEvent->getNormal() * -1.0f;
    }
    if (pCollisionEvent->getObject2() == this)
    {
        normal = pCollisionEvent->getNormal();
    }

    if (normal.y > 0.5f)
    {
        m_canJump = true;
    }
}

void Deer::onSoundPlayedEvent(fw::Event* pEvent)
{
    SoundPlayedEvent* pSoundEvent = static_cast<SoundPlayedEvent*>(pEvent);
    {
        float distance = m_position.distanceFrom(pSoundEvent->getLocation());
        if (distance <= pSoundEvent->getRadius())
        {
            m_scaredTimer = fw::Random::randomFloat(0.3f, 0.4f);
        }
    }
}
