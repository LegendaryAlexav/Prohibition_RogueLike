//
// Copyright (c) 2022-2023 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#pragma once

#include "Math/Matrix.h"
#include "Objects/GameObject.h"
#include "EventSystem/EventManager.h"

namespace fw
{
    class Event;

    class Camera : public GameObject
    {
    public:
        Camera(GameCore* pGameCore, vec3 pos);
        virtual ~Camera();

        virtual void update(float32 deltaTime) override;

        virtual void enable(int viewID);

        // Setters.
        void setProjectionScale(vec2 scale) { m_projectionScale = scale; }
        void setAspectRatio(float32 aspectRatio) { m_aspectRatio = aspectRatio; }
        void setLookAtPosition(vec3 lookAtPosition) { m_lookAtPosition = lookAtPosition; }

        void onWindowResizeEvent(Event* pEvent);

    protected:
        vec2 m_projectionScale = { 0.1f, 0.1f };
        float32 m_aspectRatio = 1.0f;

        vec3 m_lookAtPosition = vec3(0, 0, 0);

        EventListener m_eventListener;
    };

} // namespace fw
