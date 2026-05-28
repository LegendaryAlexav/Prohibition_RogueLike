//
// Copyright (c) 2022-2023 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "Framework.h"

#include "Events/GameEvents.h"
#include "Game.h"
#include "GameCore.h"
#include "Objects/PhysicsPlayer.h"
#include "HeightmapMesh.h"

PhysicsPlayer::PhysicsPlayer(Game* pGame, std::string name, vec3 pos, fw::Mesh* pMesh, fw::Material* pMaterial)
    : fw::GameObject( pGame, name, pos, pMesh, pMaterial )
    , m_inputEventListener( std::bind(&PhysicsPlayer::onInputEvent, this, std::placeholders::_1) )
    , m_collisionEventListener( std::bind(&PhysicsPlayer::onCollisionEvent, this, std::placeholders::_1) )
{
    pGame->getEventManager()->registerListener<fw::InputEvent>( &m_inputEventListener );
    pGame->getEventManager()->registerListener<fw::CollisionEvent>( &m_collisionEventListener );
}

PhysicsPlayer::~PhysicsPlayer()
{
    m_pGameCore->getEventManager()->unregisterListener<fw::InputEvent>( &m_inputEventListener );
    m_pGameCore->getEventManager()->unregisterListener<fw::CollisionEvent>( &m_collisionEventListener );
}

void PhysicsPlayer::update(float deltaTime)
{
    GameObject::update( deltaTime );

    // After syncing with physics, lower the mesh position a bit, since the player's pivot is at the bottom center.
    //m_position.y -= 0.25f;

    // Raycast from below up to our position to see if we're still on ground, disable jumping if the ray hits us.
    vec3 rayStart = m_position + vec3(0, -0.5f, 0);
    vec3 rayEnd = m_position;
    fw::GameObject* pHitObject = m_pGameCore->getPhysicsWorld()->raycastForNearestObject( rayStart, rayEnd );
    if( pHitObject == this )
    {
        m_canJump = false;
    }

    // Deal with player movement.
    float speed = 1.0f;

    vec3 dir = { m_controls.x, 0, m_controls.y };
    dir.normalize();

    float cameraYRotation = static_cast<Game*>(m_pGameCore)->getCamera()->getRotation().y;
    mat4 rotationY;
    rotationY.createRotation( vec3(0, cameraYRotation, 0) );
    dir = rotationY * dir;

    m_pPhysicsBody->applyForceToCenter( dir * speed );
}

void PhysicsPlayer::onInputEvent(fw::Event* pEvent)
{
    // Code to set controls based on events.
    if( pEvent->getType() == fw::InputEvent::getStaticEventType() )
    {
        fw::InputEvent* pInputEvent = static_cast<fw::InputEvent*>( pEvent );

        if( pInputEvent->getDeviceType() == fw::InputEvent::DeviceType::Keyboard )
        {
            if( pInputEvent->getDeviceState() == fw::InputEvent::DeviceState::Pressed )
            {
                switch( pInputEvent->getKeyCode() )
                {
                case 'W': /*case VK_UP:   */    { m_controls.y += 1; } break;
                case 'A': /*case VK_LEFT: */    { m_controls.x -= 1; } break;
                case 'S': /*case VK_DOWN: */    { m_controls.y -= 1; } break;
                case 'D': /*case VK_RIGHT:*/    { m_controls.x += 1; } break;
                case 'X':
                    {
                        // Create an area damage event at the PhysicsPlayer's location.
                        AreaDamageEvent* pAreaDamageEvent = new AreaDamageEvent( m_position, 2.0f, 1.0f );
                        m_pGameCore->getEventManager()->addEvent( pAreaDamageEvent );
                    }
                    break;
                case 'E':
                    {
                        // Create a spawn ball event above the player's location.
                        SpawnBallEvent* pSpawnBallEvent = new SpawnBallEvent( m_position + vec3(0,2,0), 0.5f );
                        m_pGameCore->getEventManager()->addEvent( pSpawnBallEvent );
                    }
                    break;
                case ' ': // Jump.
                    {
                        if( m_canJump )
                        {
                            m_pPhysicsBody->applyImpulseToCenter( vec3(0,0.5f,0) );
                            m_canJump = false;
                        }
                    }
                    break;
                }
            }

            if( pInputEvent->getDeviceState() == fw::InputEvent::DeviceState::Released )
            {
                switch( pInputEvent->getKeyCode() )
                {
                case 'W': /*case VK_UP:   */    { m_controls.y -= 1; } break;
                case 'A': /*case VK_LEFT: */    { m_controls.x += 1; } break;
                case 'S': /*case VK_DOWN: */    { m_controls.y += 1; } break;
                case 'D': /*case VK_RIGHT:*/    { m_controls.x -= 1; } break;
                }
            }
        }
    }
}

void PhysicsPlayer::onCollisionEvent(fw::Event* pEvent)
{
    fw::CollisionEvent* pCollisionEvent = static_cast<fw::CollisionEvent*>( pEvent );

    vec3 normal;
    if( pCollisionEvent->getObject1() == this )
    {
        normal = pCollisionEvent->getNormal() * -1.0f;
    }
    if( pCollisionEvent->getObject2() == this )
    {
        normal = pCollisionEvent->getNormal();
    }

    if( normal.y > 0.5f )
    {
        if (!m_canJump)
        {
            SoundPlayedEvent* pSoundPlayedEvent = new SoundPlayedEvent(m_position, 5.0f);
            m_pGameCore->getEventManager()->addEvent(pSoundPlayedEvent);
            m_canJump = true;
        }
    }
}
