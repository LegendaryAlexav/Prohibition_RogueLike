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
#include "Objects/PhysicsPlayer.h"
#include "Events/GameEvents.h"
#include "HeightMapMesh.h"

PhysicsPlayer::PhysicsPlayer(Game* pGame, std::string name, vec3 pos, fw::Mesh* pMesh, fw::Material* pMaterial)
    : fw::GameObject(pGame, name, pos, pMesh, pMaterial)
    , m_inputEventListener(std::bind(&PhysicsPlayer::onInputEvent, this, std::placeholders::_1))
{
    pGame->getEventManager()->registerListener<fw::InputEvent>(&m_inputEventListener);
}

PhysicsPlayer::~PhysicsPlayer()
{
    m_pGameCore->getEventManager()->unregisterListener<fw::InputEvent>(&m_inputEventListener);
}

void PhysicsPlayer::update(float deltaTime)
{
    GameObject::update(deltaTime);

    // Raycast from below up to our position to see if we're still on ground, disable jumping if the ray hits us.
    vec3 rayStart = m_position + vec3(0, -0.5f, 0);
    vec3 rayEnd = m_position;
    fw::GameObject* pHitObject = m_pGameCore->getPhysicsWorld()->rayCastNearestObject(rayStart, rayEnd);
    if (pHitObject == this)
    {
        m_canJump = false;
    }

    m_forward = m_pForwardTarget->getForward();
    float speed = 1.0f;

    vec3 dirForward = { -m_forward.z , 0, m_forward.x };
    vec3 dirRight = { m_forward.x, 0, m_forward.z };

    ImGui::Begin("Camera Controls");
    ImGui::DragFloat3("Player dirForward", &dirForward.x, 0.02f);
    ImGui::DragFloat3("Player dirForward", &dirRight.x, 0.02f);
    ImGui::End();

    // Make a new ImGui to display the inventory

    //m_pPhysicsBody->applyForceToCenter(dir * speed);
    m_pPhysicsBody->applyTorque(dirForward * m_controls.y * speed);
    m_pPhysicsBody->applyTorque(dirRight * m_controls.x * speed);
}

void PhysicsPlayer::followTargetFoward(GameObject* pTarget)
{
    m_pForwardTarget = pTarget;
}

void PhysicsPlayer::addItemToInventory(GameObject* pGameObject)
{
    // The Inventory System is owned and controlled inside the player
    // This would of sent the game object to the invetorysystem the tree/rock.
    // It checks if it is a tree or a rock, with the game object's name
    // Then it would add 1 to the invetory number ie rock or tree.
}

void PhysicsPlayer::onInputEvent(fw::Event* pEvent)
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
                    // I was going to switch this to BreakObjectRayCastEvent, the location point would of been the player's location + the radius of the sphere (for raycast to happen).
                    // Then it would use the m_ForwardTarget, to get the direction, and multiplying that with the distance that I choose
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