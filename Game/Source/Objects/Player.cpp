//
// Copyright (c) 2022-2023 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "Framework.h"

#include "Game.h"
#include "GameCore.h"
#include "Objects/Player.h"
#include "Events/GameEvents.h"
#include "HeightMapMesh.h"

Player::Player(Game* pGame, std::string name, vec3 pos, fw::Mesh* pMesh, fw::Material* pMaterial)
    : fw::GameObject( pGame, name, pos, pMesh, pMaterial )
	, m_inputEventListener(std::bind(&Player::onInputEvent, this, std::placeholders::_1))
{
	m_controls = vec3::zero();
	pGame->getEventManager()->registerListener<fw::InputEvent>(&m_inputEventListener);
}

Player::~Player()
{
	m_pGameCore->getEventManager()->unregisterListener<fw::InputEvent>(&m_inputEventListener);
}

void Player::update(float deltaTime)
{
    float speed = 4.0f;

	vec3 dir;
	dir = vec3(m_controls.x, m_controls.z, m_controls.y);
    dir.normalize();

    //m_rotation.z++;
    //m_rotation.y++;
    //m_rotation.x--;

    m_position += dir * speed * deltaTime;

    Game* pGame = static_cast<Game*>(m_pGameCore);
    if (fw::GameObject* pHeightmap = pGame->getHeightmap())
    {
        mat4 hmWorldMatrix;
        hmWorldMatrix.createSRT(pHeightmap->getScale(), pHeightmap->getRotation(), pHeightmap->getPosition());
        mat4 invHMWorldMatrix = hmWorldMatrix.getInverse();
        vec3 heightmapSpacePos = invHMWorldMatrix * m_position;

        if (HeightmapMesh* pHeightmapMesh = dynamic_cast<HeightmapMesh*>(pHeightmap->getMesh()))
        {
            float finalHeight = pHeightmapMesh->getHeightForXZPosition(heightmapSpacePos);

            m_position.y = finalHeight + (m_scale.y / 2); //hmWorldMatrix * vec3(0,finalHeight,0);
        }
    }
}

void Player::onInputEvent(fw::Event* pEvent)
{
    // Code to set controls based on events.
    if (pEvent->getType() == fw::InputEvent::getStaticEventType())
    {
        fw::InputEvent* pInputEvent = static_cast<fw::InputEvent*>(pEvent);

        if (pInputEvent->getDeviceType() == fw::InputEvent::DeviceType::Keyboard)
        {
            if (pInputEvent->getDeviceState() == fw::InputEvent::InputState::Pressed)
            {
                switch (pInputEvent->getKeyCode())
                {
                case 'W': case VK_UP: { m_controls.y += 1; } break;
                case 'A': case VK_LEFT: { m_controls.x -= 1; } break;
                case 'S': case VK_DOWN: { m_controls.y -= 1; } break;
                case 'D': case VK_RIGHT: { m_controls.x += 1; } break;
                case 'X':
                {
                    // Create an area damage event at the player's location.
                    AreaDamageEvent* pAreaDamageEvent = new AreaDamageEvent(m_position, 2.0f, 1.0f);
                    m_pGameCore->getEventManager()->addEvent(pAreaDamageEvent);
                } break;
                
                case 'E':
                {
                    // Spawn Grass Around the player's location
                    vec2 rangeX = vec2(-5.0f, 5.0f);
                    vec2 rangeZ = vec2(-5.0f, 5.0f);
                    SpawnGrassEvent* pSpawnGrassEvent = new SpawnGrassEvent(m_position, rangeX, rangeZ, 50);
                    m_pGameCore->getEventManager()->addEvent(pSpawnGrassEvent);
                } break;

                }
            }

            if (pInputEvent->getDeviceState() == fw::InputEvent::InputState::Released)
            {
                switch (pInputEvent->getKeyCode())
                {
                case 'W': case VK_UP: { m_controls.y -= 1; } break;
                case 'A': case VK_LEFT: { m_controls.x += 1; } break;
                case 'S': case VK_DOWN: { m_controls.y += 1; } break;
                case 'D': case VK_RIGHT: { m_controls.x -= 1; } break;
                }
            }
        }
    }
}
