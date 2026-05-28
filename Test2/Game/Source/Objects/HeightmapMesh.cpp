#include "Framework.h"
#include "HeightmapMesh.h"

HeightmapMesh::HeightmapMesh(const bgfx::VertexLayout& vertexFormat, const void* verts, uint32 vertsSize, const void* indices, uint32 indicesSize,
                             ivec2 vertCount, vec2 worldSize, const std::vector<vec3>& positions)
    : fw::Mesh( vertexFormat, verts, vertsSize, indices, indicesSize )
    , m_vertCount( vertCount )
    , m_worldSize( worldSize )
    , m_positions( positions )
{
}

float HeightmapMesh::getHeightForXZPosition(vec3 objectSpacePoint)
{
    vec2 point = { objectSpacePoint.x, objectSpacePoint.z };

    ivec2 boxCoordinates;
    boxCoordinates = point / m_worldSize * vec2((float)m_vertCount.x-1, (float)m_vertCount.y-1);

    // Bounds checks.
    fw::myClamp( boxCoordinates.x, 0, m_vertCount.x-2 );
    fw::myClamp( boxCoordinates.y, 0, m_vertCount.y-2 );

    vec2 stepSize = m_worldSize / vec2((float)m_vertCount.x-1, (float)m_vertCount.y-1);

    ivec2 blCoord = (boxCoordinates + ivec2(0,0));
    ivec2 trCoord = (boxCoordinates + ivec2(1,1));
    ivec2 brCoord = (boxCoordinates + ivec2(1,0));
    ivec2 tlCoord = (boxCoordinates + ivec2(0,1));

    float blHeight = m_positions[blCoord.y*m_vertCount.x + blCoord.x].y;
    float trHeight = m_positions[trCoord.y*m_vertCount.x + trCoord.x].y;
    float brHeight = m_positions[brCoord.y*m_vertCount.x + brCoord.x].y;
    float tlHeight = m_positions[tlCoord.y*m_vertCount.x + tlCoord.x].y;

    vec2 blPos = blCoord * stepSize;
    vec2 trPos = trCoord * stepSize;
    vec2 brPos = brCoord * stepSize;
    vec2 tlPos = tlCoord * stepSize;

    bool isBottomTri = (trPos-blPos).cross( point-blPos ) < 0;
    vec2 thirdPos = isBottomTri ? brPos : tlPos;
    float thirdHeight = isBottomTri ? brHeight : tlHeight;

    vec2 A = trPos-blPos;
    vec2 B = thirdPos-blPos;
    vec2 C = point-blPos;

    float totalArea = fabs( A.cross(B) );
    float centerInfluenceArea = fabs( A.cross(C) );
    float trInfluenceArea = fabs( C.cross(B) );

    float trRatio = trInfluenceArea / totalArea;
    float thirdRatio = centerInfluenceArea / totalArea;
    float blRatio = 1.0f - trRatio - thirdRatio;

    float finalHeight = trRatio*trHeight + thirdRatio*thirdHeight + blRatio*blHeight;

    //if( ImGui::Begin("Heightmap Debug") )
    //{
    //    ImGui::Text( "object space pos %0.2f, %0.2f", objectSpacePoint.x, objectSpacePoint.z );
    //    ImGui::Text( "boxCoordinates %d, %d", boxCoordinates.x, boxCoordinates.y );
    //    ImGui::Text( "blPos %0.2f, %0.2f", blPos.x, blPos.y );
    //    ImGui::Text( "trPos %0.2f, %0.2f", trPos.x, trPos.y );
    //    ImGui::Text( "isBottomTri %d", isBottomTri );
    //    ImGui::Text( "trRatio %0.2f", trRatio );
    //    ImGui::Text( "thirdRatio %0.2f", thirdRatio );
    //    ImGui::Text( "blRatio %0.2f", blRatio );
    //    ImGui::Text( "finalHeight %0.2f", finalHeight );
    //}
    //ImGui::End();

    return finalHeight;
}
