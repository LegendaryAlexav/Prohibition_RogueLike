//
// Copyright (c) 2022-2023 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "CoreHeaders.h"

#include "Core/FWCore.h"
#include "EventSystem/EventManager.h"
#include "EventSystem/Events.h"
#include "GameCore.h"
#include "ImGuiManager.h"
#include "imgui/imgui.h"
#include "imgui_impl_bgfx.h"

// Relies on imgui_impl_bgfx
//    https://gist.github.com/pr0g/aff79b71bf9804ddb03f39ca7c0c3bbb
// Forked from
//    https://gist.github.com/RichardGale/6e2b74bc42b3005e08397236e4be0fd0
// Along with these shaders from the bgfx examples:
//    fs_ocornut_imgui.bin.h
//    vs_ocornut_imgui.bin.h

namespace fw
{
    ImGuiManager* g_pImGuiManager = nullptr;

    ImGuiManager::ImGuiManager(FWCore* pFramework, int viewID, EventManager* pEventManager)
        : m_pEventManager( pEventManager )
        , m_eventListener( std::bind(&ImGuiManager::onCharEvent, this, std::placeholders::_1) )
    {
        m_pFramework = pFramework;
        init( viewID );

        pEventManager->registerListener<fw::CharEvent>( &m_eventListener );
    }

    ImGuiManager::~ImGuiManager()
    {
        m_pEventManager->unregisterListener<fw::CharEvent>(&m_eventListener);
        shutdown();
    }

    void ImGuiManager::init(int viewID)
    {
        ImGui::CreateContext();

        createDeviceObjects();

        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize.x = (float)m_pFramework->getWindowClientWidth();
        io.DisplaySize.y = (float)m_pFramework->getWindowClientHeight();
        io.IniFilename = "imgui.ini";
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        ImGui_Implbgfx_Init( viewID );
    }

    void ImGuiManager::shutdown()
    {
        ImGui_Implbgfx_Shutdown();

        ImGui::DestroyContext();
    }

    void ImGuiManager::onCharEvent(Event* pEvent)
    {
        if( pEvent->getType() == fw::CharEvent::getStaticEventType() )
        {
            int character = static_cast<fw::CharEvent*>(pEvent)->getValue();
            addInputCharacter( character );
        }
    }

    void ImGuiManager::addInputCharacter(unsigned int value)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.AddInputCharacter( value );
    }

    void ImGuiManager::clearInput()
    {
        ImGuiIO& io = ImGui::GetIO();

        for( int i=0; i<5; i++ )
        {
            io.MouseDown[i] = false;
        }

        io.MouseWheel = 0;

        io.KeyCtrl = false;
        io.KeyShift = false;
        io.KeyAlt = false;
        io.KeySuper = false;
    }

    void ImGuiManager::onFocusLost()
    {
        clearInput();
    }

    void ImGuiManager::startFrame(float deltaTime)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.DeltaTime = deltaTime;
        io.DisplaySize.x = (float)m_pFramework->getWindowClientWidth();
        io.DisplaySize.y = (float)m_pFramework->getWindowClientHeight();

        int mx, my;
        m_pFramework->getMouseCoordinates( &mx, &my );
        io.MousePos.x = (float)mx;
        io.MousePos.y = (float)my;
        io.MouseDown[0] = m_pFramework->isMouseButtonDown( 0 );
        io.MouseDown[1] = m_pFramework->isMouseButtonDown( 1 );
        io.MouseDown[2] = m_pFramework->isMouseButtonDown( 2 );
        io.MouseWheel = m_pFramework->getMouseWheel();

        ImGui_Implbgfx_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiManager::endFrame()
    {
        ImGui::Render();
        ImDrawData* data = ImGui::GetDrawData();
        renderDrawLists( data );
    }

    // This is the main rendering function
    // If text or lines are blurry when integrating ImGui in your engine:
    // - in your Render function, try translating your projection matrix by (0.5f,0.5f) or (0.375f,0.375f)
    void ImGuiManager::renderDrawLists(ImDrawData* draw_data)
    {
        ImGui_Implbgfx_RenderDrawLists( draw_data );
    }

    bool ImGuiManager::createDeviceObjects()
    {
        return ImGui_Implbgfx_CreateDeviceObjects();
    }

    void ImGuiManager::invalidateDeviceObjects()
    {
        ImGui_Implbgfx_InvalidateDeviceObjects();
    }

} // namespace fw
