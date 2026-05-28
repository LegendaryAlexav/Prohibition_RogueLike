#pragma once

#include "DataTypes.h"

class HeightmapMesh : public fw::Mesh
{
public:
    HeightmapMesh(const bgfx::VertexLayout& vertexFormat, const void* verts, uint32 vertsSize, const void* indices, uint32 indicesSize,
                  ivec2 vertCount, vec2 worldSize, const std::vector<vec3>& positions);

    float getHeightForXZPosition(vec3 objectSpacePoint);
    const std::vector<vec3>& getPositions() { return m_positions; }
    ivec2 getVertCount() { return m_vertCount; }

protected:
    ivec2 m_vertCount;
    vec2 m_worldSize;

    std::vector<vec3> m_positions;
};
