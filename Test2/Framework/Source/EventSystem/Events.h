//
// Copyright (c) 2022-2023 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#pragma once

#include "Math/Vector.h"

namespace fw
{
    using EventType = const char*;

    class GameObject;

    //==========================
    // Base event class
    //==========================
    class Event
    {
        friend class EventManager;

    public:
        Event() {};
        virtual ~Event() {};

        virtual EventType getType() = 0;
    };

    //==========================
    // Input event class
    //==========================
    class InputEvent : public Event
    {
    public:
        enum class DeviceType
        {
            Keyboard,
        };

        enum class DeviceState
        {
            Pressed,
            Released,
        };

        InputEvent(DeviceType deviceType, DeviceState deviceState, uint32 keyCode)
        {
            m_deviceType = deviceType;
            m_deviceState = deviceState;
            m_keyCode = keyCode;
        }
        virtual ~InputEvent() {}

        static EventType getStaticEventType() { return "InputEvent"; }
        virtual EventType getType() override { return getStaticEventType(); }

        DeviceType getDeviceType() { return m_deviceType; }
        DeviceState getDeviceState() { return m_deviceState; }
        unsigned int getKeyCode() { return m_keyCode; }

    protected:
        DeviceType m_deviceType;
        DeviceState m_deviceState;
        uint32 m_keyCode;
    };

    //==========================
    // OnChar event class
    //==========================
    class CharEvent : public Event
    {
    public:
        CharEvent(unsigned int value)
        {
            m_value = value;
        }
        virtual ~CharEvent() {}

        static EventType getStaticEventType() { return "CharEvent"; }
        virtual EventType getType() override { return getStaticEventType(); }

        uint32 getValue() { return m_value; }

    protected:
        uint32 m_value;
    };

    //==========================
    // WindowResize event class
    //==========================
    class WindowResizeEvent : public Event
    {
    public:
        WindowResizeEvent(int width, int height)
        {
            m_width = width;
            m_height = height;
        }
        virtual ~WindowResizeEvent() {}

        static EventType getStaticEventType() { return "WindowResizeEvent"; }
        virtual EventType getType() override { return getStaticEventType(); }

        uint16 getWidth() { return m_width; }
        uint16 getHeight() { return m_height; }

    protected:
        uint16 m_width;
        uint16 m_height;
    };

    //==========================
    // Collision event class
    //==========================
    class CollisionEvent : public Event
    {
    public:
        CollisionEvent(GameObject* pObject1, GameObject* pObject2, vec3 normal)
        {
            m_pObject1 = pObject1;
            m_pObject2 = pObject2;
            m_normal = normal;
        }
        virtual ~CollisionEvent() {}

        static EventType getStaticEventType() { return "CollisionEvent"; }
        virtual EventType getType() override { return getStaticEventType(); }

        GameObject* getObject1() { return m_pObject1; }
        GameObject* getObject2() { return m_pObject2; }
        vec3 getNormal() { return m_normal; }

    protected:
        GameObject* m_pObject1;
        GameObject* m_pObject2;
        vec3 m_normal;
    };

} // namespace fw
