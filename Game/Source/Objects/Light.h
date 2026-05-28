#pragma once

#include "DataTypes.h"

class Light
{
public:
    Light(const fw::vec3& position, const fw::vec3& color, float range, float ambient);

    void showDebugUI();

public:
    fw::vec3 m_position;
    fw::vec3 m_color;
    float m_range;
    float m_ambient;
};