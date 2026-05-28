#pragma once

#include "DataTypes.h"

class Light
{
public:
    Light(const vec3& position, const vec3& rotation, const vec3& color, float ambient);
    Light(const vec3& position, const vec3& color, float range, float ambient);

    void showDebugUI();

public:
    vec3 m_position;
    vec3 m_rotation;
    vec3 m_color;
    float m_range;
    float m_ambient;
};
