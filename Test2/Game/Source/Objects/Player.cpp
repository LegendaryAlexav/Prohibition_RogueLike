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
#include "Objects/Player.h"
#include "HeightmapMesh.h"

Player::Player(Game* pGame, std::string name, vec3 pos, fw::Mesh* pMesh, fw::Material* pMaterial)
    : fw::GameObject( pGame, name, pos, pMesh, pMaterial )
    , m_inputEventListener( std::bind(&Player::onInputEvent, this, std::placeholders::_1) )
{
    pGame->getEventManager()->registerListener<fw::InputEvent>( &m_inputEventListener );
}

Player::~Player()
{
    m_pGameCore->getEventManager()->unregisterListener<fw::InputEvent>( &m_inputEventListener );
}

void Player::update(float deltaTime)
{
    float speed = 4.0f;

    vec3 dir = { m_controls.x, 0, m_controls.y };
    dir.normalize();

    float cameraYRotation = static_cast<Game*>(m_pGameCore)->getCamera()->getRotation().y;
    mat4 rotationY;
    rotationY.createRotation( vec3(0, cameraYRotation, 0) );
    dir = rotationY * dir;

    m_position += dir * speed * deltaTime;

    // Snap to height of heightmap.
    Game* pGame = static_cast<Game*>(m_pGameCore);
    if( fw::GameObject* pHeightmap = pGame->getHeightmap() )
    {
        mat4 hmWorldMatrix;
        hmWorldMatrix.createSRT(pHeightmap->getScale(), pHeightmap->getRotation(), pHeightmap->getPosition());
        mat4 invHMWorldMatrix = hmWorldMatrix.getInverse();
        vec3 heightmapSpacePos = invHMWorldMatrix * m_position;

        if( HeightmapMesh* pHeightmapMesh = dynamic_cast<HeightmapMesh*>(pHeightmap->getMesh()) )
        {
            float finalHeight = pHeightmapMesh->getHeightForXZPosition( heightmapSpacePos );

            m_position.y = finalHeight; //hmWorldMatrix * vec3(0,finalHeight,0);
        }
    }
}

void Player::onInputEvent(fw::Event* pEvent)
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
                        // Create an area damage event at the player's location.
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
                }
            }

            if( pInputEvent->getDeviceState() == fw::InputEvent::DeviceState::Released )
            {
                switch( pInputEvent->getKeyCode() )
                {
                case 'W': /*case VK_UP:   */   { m_controls.y -= 1; } break;
                case 'A': /*case VK_LEFT: */   { m_controls.x += 1; } break;
                case 'S': /*case VK_DOWN: */   { m_controls.y += 1; } break;
                case 'D': /*case VK_RIGHT:*/   { m_controls.x -= 1; } break;
                }
            }
        }
    }
}
