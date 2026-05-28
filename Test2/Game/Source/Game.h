//
// Copyright (c) 2022-2023 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#pragma once

#include "Framework.h"
#include "DataTypes.h"
#include "Objects/Light.h"

class PhysicsPlayer;
class Player;

class Game : public fw::GameCore
{
public:
    Game(fw::FWCore& fwCore);
    virtual ~Game() override;

    void createUniforms();

    virtual void startFrame(float deltaTime) override;
    virtual void onEvent(fw::Event* pEvent) override;
    virtual void update(float deltaTime) override;
    virtual void draw() override;

    virtual fw::EventManager* getEventManager() override { return m_pEventManager; }
    fw::GameObject* getHeightmap() { return m_pHeightmap; }
    fw::Camera* getCamera() { return m_pCamera; }

protected:
    // Editor Interface.
    fw::ImGuiManager* m_pImGuiManager = nullptr;
    void editor_displayObjectList();
    void editor_displayResources();

    fw::EventManager* m_pEventManager = nullptr;
    fw::EventListener m_eventListener;

    // Resources.
    fw::ResourceManager* m_pResources = nullptr;

    // GameObjects and Helpers.
    fw::Camera* m_pCamera = nullptr;
    PhysicsPlayer* m_pPlayer = nullptr;
    fw::GameObject* m_pHeightmap = nullptr;
    std::vector<fw::GameObject*> m_objects;

    // Lights.
    Light m_lightDirectional;
    Light m_lightPoint;

    bool m_wireframeMode = false;

    bgfx::TextureHandle m_FBOTexture_Color;
    bgfx::TextureHandle m_FBOTexture_Depth;
    bgfx::FrameBufferHandle m_FBO;
};
