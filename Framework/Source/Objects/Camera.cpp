//
// Copyright (c) 2022-2023 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "CoreHeaders.h"

#include "Camera.h"
#include "FWCore.h"
#include "GameCore.h"
#include "Utility/Uniforms.h"
#include "EventSystem/EventManager.h"
#include "EventSystem/Events.h"

namespace fw
{
    Camera::Camera(GameCore* pGameCore, vec3 pos)
        : GameObject( pGameCore, "Camera", pos, nullptr, nullptr )
        , m_eventListener(std::bind(&Camera::onWindowResizeEvent, this, std::placeholders::_1))
        , m_inputEventListener(std::bind(&Camera::onInputEvent, this, std::placeholders::_1))
    {
        FWCore* pFW = pGameCore->getFramework();
        setAspectRatio( (float)pFW->getWindowClientWidth()/pFW->getWindowClientHeight() );

        pGameCore->getEventManager()->registerListener<fw::WindowResizeEvent>(&m_eventListener);
        pGameCore->getEventManager()->registerListener<fw::InputEvent>(&m_inputEventListener);
    }

    Camera::~Camera()
    {
        m_pGameCore->getEventManager()->unregisterListener<fw::WindowResizeEvent>(&m_eventListener);
        m_pGameCore->getEventManager()->unregisterListener<fw::InputEvent>(&m_inputEventListener);
    }

    void Camera::update(float32 deltaTime)
    {
        vec2 rotationSpeed = vec2(1.0f, 0.4f);

        m_rotation.x += 90 * deltaTime * rotationSpeed.x * m_controls.x;
        m_rotation.y += 90 * deltaTime * rotationSpeed.x * m_controls.y;
        m_distance += 10 * deltaTime * m_controls.z;

        fw::myClamp(m_distance, 10.0f, 25.0f);
        fw::myClamp(m_rotation.x, -70.0f, 70.0f);

        float sx = sin(m_rotation.x / 180 * PI);
        float sy = sin(m_rotation.y / 180 * PI);
        float cx = cos(m_rotation.x / 180 * PI);
        float cy = cos(m_rotation.y / 180 * PI);
        vec3 offset = vec3(cx * sy * m_distance, -sx * m_distance, -1 * cx * cy * m_distance);

        m_position = m_pTarget->getPosition() + offset;
        m_lookAtPosition = m_pTarget->getPosition();

        m_forward = offset/m_distance;
        m_forward.normalize();
    }

    void Camera::enable(int viewID)
    {
        // &m_position.x is the start of the array of the x y z, even tho it is a vec3 since it saves the data one after the other
        ImGui::Begin("Camera Controls");
        ImGui::DragFloat3("Cam Position", &m_position.x, 0.02f);
        ImGui::DragFloat3("Cam Forward", &m_forward.x, 0.02f);
        ImGui::End();

        Uniforms* pUniforms = m_pGameCore->getUniforms();
        vec2 scaleWithAspectRatio = m_projectionScale.xy() * vec2( 1.0f/m_aspectRatio, 1 );

        mat4 view;
        mat4 proj;

        view.createLookAtView(m_position, vec3(0, 1, 0), m_lookAtPosition);
        proj.createPerspectiveVFoV(60, m_aspectRatio, 0.01f, 100.0f);

        pUniforms->set("u_viewMatrix", view);
        pUniforms->set("u_projMatrix", proj);
    }

    void Camera::onWindowResizeEvent(Event* pEvent)
    {
        if (pEvent->getType() == WindowResizeEvent::getStaticEventType())
        {
            FWCore* pFW = m_pGameCore->getFramework();
            setAspectRatio((float)pFW->getWindowClientWidth() / pFW->getWindowClientHeight());
        }
    }

    void Camera::onInputEvent(Event* pEvent)
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
                    case 'I': case VK_UP: { m_controls.x -= 1; } break;
                    case 'J': case VK_LEFT: { m_controls.y -= 1; } break;
                    case 'K': case VK_DOWN: { m_controls.x += 1; } break;
                    case 'L': case VK_RIGHT: { m_controls.y += 1; } break;
                    case 'U': { m_controls.z += 1; } break;
                    case 'M': { m_controls.z -= 1; } break;
                    }
                }

                if (pInputEvent->getDeviceState() == fw::InputEvent::InputState::Released)
                {
                    switch (pInputEvent->getKeyCode())
                    {
                    case 'I': case VK_UP: { m_controls.x += 1; } break;
                    case 'J': case VK_LEFT: { m_controls.y += 1; } break;
                    case 'K': case VK_DOWN: { m_controls.x -= 1; } break;
                    case 'L': case VK_RIGHT: { m_controls.y -= 1; } break;
                    case 'U': { m_controls.z -= 1; } break;
                    case 'M': { m_controls.z += 1; } break;
                    }
                }
            }
        }
    }

} // namespace fw
