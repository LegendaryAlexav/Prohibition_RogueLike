#pragma once

#include "DataTypes.h"

class HeightmapMesh : public fw::Mesh
{
public:
    HeightmapMesh(const bgfx::VertexLayout& vertexFormat, const void* verts, uint32 vertsSize, const void* indices, uint32 indicesSize,
                  ivec2 vertCount, vec2 worldSize, const std::vector<vec3>& positions);

    float getHeightForXZPosition(vec3 objectSpacePoint);

    virtual ivec2 getVertCount() override { return m_vertCount; }
    virtual vec2 getWorldSize() override  { return m_worldSize; }

    virtual std::vector<vec3>* getPoints() override { return &m_positions; };
    virtual const float* getYPositions() override { return m_yPos.data(); };


protected:
    ivec2 m_vertCount;
    vec2 m_worldSize;

    std::vector<vec3> m_positions;
    std::vector<float> m_yPos;
};
