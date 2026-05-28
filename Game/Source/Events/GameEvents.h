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

class RemoveFromGameEvent : public fw::Event
{
public:
    RemoveFromGameEvent(fw::GameObject* pObject)
    {
        m_pObject = pObject;
    }
    virtual ~RemoveFromGameEvent() {}

    static fw::EventType getStaticEventType() { return "RemoveFromGameEvent"; }
    virtual fw::EventType getType() override { return getStaticEventType(); }

    fw::GameObject* getGameObject() { return m_pObject; }

protected:
    fw::GameObject* m_pObject;
};

class AreaDamageEvent : public fw::Event
{
public:
    AreaDamageEvent(vec3 location, float radius, float damage)
    {
        m_location = location;
        m_radius = radius;
        m_damage = damage;
    }
    virtual ~AreaDamageEvent() {}

    static fw::EventType getStaticEventType() { return "AreaDamageEvent"; }
    virtual fw::EventType getType() override { return getStaticEventType(); }

    vec3 getLocation() { return m_location; }
    float getRadius() { return m_radius; }
    float getDamage() { return m_damage; }

protected:
    vec3 m_location;
    float m_radius;
    float m_damage;
};

// This can be changed to a SpawnGameObjectInRangeEvent
class SpawnGrassEvent : public fw::Event
{
public:
    SpawnGrassEvent(vec3 location, vec2 rangeX, vec2 rangeY, int numGrass)
    {
        m_location = location;
        m_rangeX = rangeX;
        m_rangeY = rangeY;
        m_numGrass = numGrass;
    }
    virtual ~SpawnGrassEvent() {}

    static fw::EventType getStaticEventType() { return "SpawnGrassEvent"; }
    virtual fw::EventType getType() override { return getStaticEventType(); }

    vec3 getLocation() { return m_location; }
    vec2 getRangeX() { return m_rangeX; }
    vec2 getRangeZ() { return m_rangeY; }
    int getNumGrass() { return m_numGrass; }

protected:
    vec3 m_location;
    vec2 m_rangeX;
    vec2 m_rangeY;
    int m_numGrass;
};

class BreakObjectRayCastEvent : public fw::Event
{
public:
    BreakObjectRayCastEvent(vec3 startLocation, vec3 direction)
    {
        m_startLocation = startLocation;
        m_direction = direction;
    }
    virtual ~BreakObjectRayCastEvent() {}

    static fw::EventType getStaticEventType() { return "BreakObjectRayCastEvent"; }
    virtual fw::EventType getType() override { return getStaticEventType(); }

    vec3 getLocation() { return m_startLocation; }
    vec3 getDirection() { return m_direction; }

protected:
    vec3 m_startLocation;
    vec3 m_direction;
};
