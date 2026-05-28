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
#include "Core/FWCore.h"
#include "EventSystem/EventManager.h"
#include "EventSystem/Events.h"
#include "GameCore.h"
#include "Utility/Uniforms.h"

namespace fw
{
    Camera::Camera(GameCore* pGameCore, vec3 pos)
        : GameObject( pGameCore, "Camera", pos, nullptr, nullptr )
        , m_eventListener( std::bind(&Camera::onWindowResizeEvent, this, std::placeholders::_1) )
    {
        FWCore* pFW = pGameCore->getFramework();
        setAspectRatio( (float)pFW->getWindowClientWidth()/pFW->getWindowClientHeight() );

        pGameCore->getEventManager()->registerListener<fw::WindowResizeEvent>( &m_eventListener );
    }

    Camera::~Camera()
    {
        m_pGameCore->getEventManager()->unregisterListener<fw::WindowResizeEvent>( &m_eventListener );
    }

    void Camera::update(float32 deltaTime)
    {
    }

    void Camera::enable(int viewID)
    {
        ImGui::Begin( "Camera Controls" );
        ImGui::DragFloat3( "Cam Position", &m_position.x, 0.02f );
        ImGui::End();

        Uniforms* pUniforms = m_pGameCore->getUniforms();

        // Setup uniforms.
        mat4 view;
        mat4 proj;

        view.createLookAtView( m_position, vec3(0,1,0), m_lookAtPosition );
        proj.createPerspectiveVFoV( 60, m_aspectRatio, 0.01f, 100.0f );

        pUniforms->set( "u_cameraPosition", m_position );
        pUniforms->set( "u_viewMatrix", view );
        pUniforms->set( "u_projMatrix", proj );
    }

    void Camera::onWindowResizeEvent(Event* pEvent)
    {
        // Set the new aspect ratio in the camera.
        if( pEvent->getType() == fw::WindowResizeEvent::getStaticEventType() )
        {
            fw::WindowResizeEvent* pResizeEvent = static_cast<fw::WindowResizeEvent*>( pEvent );

            setAspectRatio( (float)pResizeEvent->getWidth()/pResizeEvent->getHeight() );
        }
    }
} // namespace fw
