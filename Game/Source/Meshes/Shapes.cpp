//
// Copyright (c) 2022-2023 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "Framework.h"

#include "VertexFormats.h"
#include "Shapes.h"
#include "Objects/HeightmapMesh.h"
#include "Utility/Uniforms.h"

//=======================
// Triangle
//=======================
static const VertexFormat_PosNormalColor g_TriangleVerts[] =
{
    { vec3( 0.0f, 0.5f, 2.0f), vec3(0,0,1), 255, 0, 0, 255 }, // Top
    { vec3(-0.5f,-0.5f, 1.0f), vec3(0,0,1), 0, 255, 0, 255 }, // Bottom-left
    { vec3( 0.5f,-0.5f, 4.0f), vec3(0,0,1), 0, 0, 255, 255 }, // Bottom-right
};
static const uint16 g_TriangleIndices[] =
{
    0, 2, 1,
};
fw::Mesh* createTriangleMesh()
{
    return new fw::Mesh( VertexFormat_PosNormalColor::format, g_TriangleVerts, sizeof(g_TriangleVerts), g_TriangleIndices, sizeof(g_TriangleIndices) );
}

//=======================
// Square Colour
//=======================
static const VertexFormat_PosNormalColor g_SquareVerts[] =
{
    { vec2(-0.5f, 0.5f), vec3(0,0,1), 0, 255, 0, 255 }, // Top-left
    { vec2(-0.5f,-0.5f), vec3(0,0,1), 0,   0, 0, 255 }, // Bottom-left
    { vec2( 0.5f,-0.5f), vec3(0,0,1), 0,   0, 0, 255 }, // Bottom-right

    { vec2(-0.5f, 0.5f), vec3(0,0,1), 0, 255, 0, 255 }, // Top-left
    { vec2( 0.5f,-0.5f), vec3(0,0,1), 0,   0, 0, 255 }, // Bottom-right
    { vec2( 0.5f, 0.5f), vec3(0,0,1), 0, 255, 0, 255 }, // Top-right
};
static const uint16 g_SquareIndices[] = 
{
    0,2,1, 3,5,4
};
fw::Mesh* createSquareMesh()
{
    return new fw::Mesh(VertexFormat_PosNormalColor::format, g_SquareVerts, sizeof(g_SquareVerts), g_SquareIndices, sizeof(g_SquareIndices) );
}


//=======================
// Sprite
//=======================
static const VertexFormat_PosNormalUV g_SpriteVerts[] =
{
    { vec3(-0.5f, 0.5f, 0.0f), vec3(0,-1,0), vec2(0,1) }, // Top-left       // 0
    { vec3(-0.5f,-0.5f, 0.0f), vec3(0,-1,0), vec2(0,0) }, // Bottom-left    // 1
    { vec3( 0.5f,-0.5f, 0.0f), vec3(0,-1,0), vec2(1,0) }, // Bottom-right   // 2
    { vec3( 0.5f, 0.5f, 0.0f), vec3(0,-1,0), vec2(1,1) }, // Top-right      // 3
};
static const uint16 g_SpriteIndices[] =
{
    0,2,1, 0,3,2
};
fw::Mesh* createSpriteMesh()
{
    return new fw::Mesh( VertexFormat_PosNormalUV::format, g_SpriteVerts, sizeof(g_SpriteVerts), g_SpriteIndices, sizeof(g_SpriteIndices) );
}

//=======================
// Two Sided Sprite
//=======================
static const VertexFormat_PosNormalUV g_TwoSidedSpriteVerts[] =
{
    { vec3(-0.5f, 1.0f, 0.0f), vec3(0,-1,0), vec2(0,1) }, // Top-left       // 0
    { vec3(-0.5f, 0.0f, 0.0f), vec3(0,-1,0), vec2(0,0) }, // Bottom-left    // 1
    { vec3( 0.5f, 0.0f, 0.0f), vec3(0,-1,0), vec2(1,0) }, // Bottom-right   // 2
    { vec3( 0.5f, 1.0f, 0.0f), vec3(0,-1,0), vec2(1,1) }, // Top-right      // 3
};
static const uint16 g_TwoSidedSpriteIndices[] =
{
    0,2,1, 0,3,2, 0,1,2, 0,2,3
};
fw::Mesh* createTwoSidedSpriteMesh()
{
    return new fw::Mesh(VertexFormat_PosNormalUV::format, g_TwoSidedSpriteVerts, sizeof(g_TwoSidedSpriteVerts), 
        g_TwoSidedSpriteIndices, sizeof(g_TwoSidedSpriteIndices));
}

//=======================
// Cube Sprite
//=======================
static const VertexFormat_PosNormalUV g_CubeSpriteVerts[] =
{
    // Front
    { vec3(-0.5f, 0.5f,-0.5f), vec3(-1, 0, 0), vec2(    0,0.33f) }, // Top-left       // 0
    { vec3(-0.5f,-0.5f,-0.5f), vec3(-1, 0, 0), vec2(    0,    0) }, // Bottom-left    // 1
    { vec3( 0.5f,-0.5f,-0.5f), vec3(-1, 0, 0), vec2(0.33f,    0) }, // Bottom-right   // 2
    { vec3( 0.5f, 0.5f,-0.5f), vec3(-1, 0, 0), vec2(0.33f,0.33f) }, // Top-right      // 3 
                                           
     // Back                                  
    { vec3(-0.5f, 0.5f, 0.5f), vec3( 1, 0, 0), vec2(0.33f,0.33f) }, // Top-left       // 4
    { vec3(-0.5f,-0.5f, 0.5f), vec3( 1, 0, 0), vec2(0.33f,    0) }, // Bottom-left    // 5
    { vec3( 0.5f,-0.5f, 0.5f), vec3( 1, 0, 0), vec2(0.66f,    0) }, // Bottom-right   // 6
    { vec3( 0.5f, 0.5f, 0.5f), vec3( 1, 0, 0), vec2(0.66f,0.33f) }, // Top-right      // 7 
                                           
     // Right                                  
    { vec3( 0.5f, 0.5f, 0.5f), vec3( 1, 0, 0), vec2(0.66f,0.33f) }, // Top-left       // 8
    { vec3( 0.5f,-0.5f, 0.5f), vec3( 1, 0, 0), vec2(0.66f,    0) }, // Bottom-left    // 9
    { vec3( 0.5f,-0.5f,-0.5f), vec3( 1, 0, 0), vec2(    1,    0) }, // Bottom-right   // 10
    { vec3( 0.5f, 0.5f,-0.5f), vec3( 1, 0, 0), vec2(    1,0.33f) }, // Top-right      // 11 
                                           
     // Left                                    
    { vec3(-0.5f, 0.5f,-0.5f), vec3(-1, 0, 0), vec2(    0,0.66f) }, // Top-left       // 12
    { vec3(-0.5f,-0.5f,-0.5f), vec3(-1, 0, 0), vec2(    0,0.33f) }, // Bottom-left    // 13
    { vec3(-0.5f,-0.5f, 0.5f), vec3(-1, 0, 0), vec2(0.33f,0.33f) }, // Bottom-right   // 14
    { vec3(-0.5f, 0.5f, 0.5f), vec3(-1, 0, 0), vec2(0.33f,0.66f) }, // Top-right      // 15 
                                           
     // Bottom                                   
    { vec3(-0.5f, 0.5f,-0.5f), vec3( 0,-1, 0), vec2(0.33f,0.66f) }, // Top-left       // 16
    { vec3(-0.5f, 0.5f, 0.5f), vec3( 0,-1, 0), vec2(0.33f,0.33f) }, // Bottom-left    // 17
    { vec3( 0.5f, 0.5f, 0.5f), vec3( 0,-1, 0), vec2(0.66f,0.33f) }, // Bottom-right   // 18
    { vec3( 0.5f, 0.5f,-0.5f), vec3( 0,-1, 0), vec2(0.66f,0.66f) }, // Top-right      // 19 
                                        
     // Top                                       
    { vec3(-0.5f,-0.5f, 0.5f), vec3( 0, 1, 0), vec2(0.66f,0.66f) }, // Top-left       // 20
    { vec3(-0.5f,-0.5f,-0.5f), vec3( 0, 1, 0), vec2(0.66f,0.33f) }, // Bottom-left    // 21
    { vec3( 0.5f,-0.5f,-0.5f), vec3( 0, 1, 0), vec2(    1,0.33f) }, // Bottom-right   // 22
    { vec3( 0.5f,-0.5f, 0.5f), vec3( 0, 1, 0), vec2(    1,0.66f) }, // Top-right      // 23
};
static const uint16 g_CubeSpriteIndices[] =
{
    0,2,1, 0,3,2,   4,5,6, 4,6,7,   8,9,10, 8,10,11,   12,13,14, 12,14,15,   16,17,18, 16,18,19,   20,21,22, 20,22,23
};
fw::Mesh* createCubeSpriteMesh()
{
    return new fw::Mesh(VertexFormat_PosNormalUV::format, g_CubeSpriteVerts, sizeof(g_CubeSpriteVerts), g_CubeSpriteIndices, sizeof(g_CubeSpriteIndices));
}

//=======================
// Lazy Cube Sprite
//=======================

static const VertexFormat_PosNormalUV g_LazyCubeVerts[] =
{
    // Front
    { vec3(-0.5f, 0.5f,-0.5f), vec3(0,0,-1), vec2(0,1) }, // Top-left
    { vec3(-0.5f,-0.5f,-0.5f), vec3(0,0,-1), vec2(0,0) }, // Bottom-left
    { vec3( 0.5f,-0.5f,-0.5f), vec3(0,0,-1), vec2(1,0) }, // Bottom-right
    { vec3( 0.5f, 0.5f,-0.5f), vec3(0,0,-1), vec2(1,1) }, // Top-right

    // Back
    { vec3( 0.5f, 0.5f, 0.5f), vec3(0,0,1), vec2(1,1) }, // Top-right
    { vec3( 0.5f,-0.5f, 0.5f), vec3(0,0,1), vec2(1,0) }, // Bottom-right
    { vec3(-0.5f,-0.5f, 0.5f), vec3(0,0,1), vec2(0,0) }, // Bottom-left
    { vec3(-0.5f, 0.5f, 0.5f), vec3(0,0,1), vec2(0,1) }, // Top-left

    // Top
    { vec3(-0.5f, 0.5f, 0.5f), vec3(0,1,0), vec2(0,1) }, // Back-left
    { vec3(-0.5f, 0.5f,-0.5f), vec3(0,1,0), vec2(0,0) }, // Front-left
    { vec3( 0.5f, 0.5f,-0.5f), vec3(0,1,0), vec2(1,0) }, // Front-right
    { vec3( 0.5f, 0.5f, 0.5f), vec3(0,1,0), vec2(1,1) }, // Back-right

    // Right
    { vec3( 0.5f, 0.5f,-0.5f), vec3(1,0,0), vec2(0,1) }, // Top-front
    { vec3( 0.5f,-0.5f,-0.5f), vec3(1,0,0), vec2(0,0) }, // Bottom-front
    { vec3( 0.5f,-0.5f, 0.5f), vec3(1,0,0), vec2(1,0) }, // Bottom-back
    { vec3( 0.5f, 0.5f, 0.5f), vec3(1,0,0), vec2(1,1) }, // Top-back

    // Left
    { vec3(-0.5f, 0.5f, 0.5f), vec3(-1,0,0), vec2(0,1) }, // Top-back
    { vec3(-0.5f,-0.5f, 0.5f), vec3(-1,0,0), vec2(0,0) }, // Bottom-back
    { vec3(-0.5f,-0.5f,-0.5f), vec3(-1,0,0), vec2(1,0) }, // Bottom-front
    { vec3(-0.5f, 0.5f,-0.5f), vec3(-1,0,0), vec2(1,1) }, // Top-front

    // Bottom
    { vec3(-0.5f,-0.5f,-0.5f), vec3(0,-1,0), vec2(1,1) }, // Front-left
    { vec3(-0.5f,-0.5f, 0.5f), vec3(0,-1,0), vec2(1,0) }, // Back-left
    { vec3( 0.5f,-0.5f, 0.5f), vec3(0,-1,0), vec2(0,0) }, // Back-right
    { vec3( 0.5f,-0.5f,-0.5f), vec3(0,-1,0), vec2(0,1) }, // Front-right
};
static const uint16 g_LazyCubeIndices[] =
{
    0,2,1, 0,3,2,
    4,6,5, 4,7,6,
    8,10,9, 8,11,10,
    12,14,13, 12,15,14,
    16,18,17, 16,19,18,
    20,22,21, 20,23,22,
};
fw::Mesh* createLazyCubeMesh()
{
    return new fw::Mesh(VertexFormat_PosNormalUV::format, g_LazyCubeVerts, sizeof(g_LazyCubeVerts), g_LazyCubeIndices, sizeof(g_LazyCubeIndices));
}


//=======================
// 3x3 Square Colour Mesh
//=======================

static const VertexFormat_PosNormalColor g_3x3SquareVerts[] =
{
    { vec2(-0.5f,-0.5f), vec3(0,0,1),  255,     0,    0,  255 }, // Bottom-Left      // 0    
    { vec2( 0.0f,-0.5f), vec3(0,0,1),    0,   255,    0,  255 }, // Bottom-Center    // 1    
    { vec2( 0.5f,-0.5f), vec3(0,0,1),    0,     0,  255,  255 }, // Bottom-Right     // 2     

    { vec2(-0.5f, 0.0f), vec3(0,0,1),    0,   255,  255,  255 }, // Middle-Left      // 3    
    { vec2( 0.0f, 0.0f), vec3(0,0,1),  255,     0,  255,  255 }, // Middle-Center    // 4    
    { vec2( 0.5f, 0.0f), vec3(0,0,1),  255,   255,    0,  255 }, // Middle-Right     // 5    

    { vec2(-0.5f, 0.5f), vec3(0,0,1),  127,     0,  255,  255 }, // Top-Left         // 6    
    { vec2( 0.0f, 0.5f), vec3(0,0,1),  255,   127,    0,  255 }, // Top-Center       // 7    
    { vec2( 0.5f, 0.5f), vec3(0,0,1),    0,   255,  127,  255 }, // Top-Right        // 8    
};

static const uint16 g_3x3SquareIndices[] =
{
    0,3,1,  3,4,1,  1,4,2,  4,5,2,  3,6,4,  6,7,4,  4,7,5,  7,8,5
};

fw::Mesh* create3x3SquareMesh()
{
    return new fw::Mesh(VertexFormat_PosNormalColor::format, g_3x3SquareVerts, sizeof(g_3x3SquareVerts), g_3x3SquareIndices, sizeof(g_3x3SquareIndices));
}

//=======================
// Cylinder
//=======================
static const VertexFormat_PosNormalColor g_cylinderVerts[] =
{
    { vec2(-0.5f, 0.5f), vec3(0,0,1), 0, 255, 0, 255 }, // Top-left
    { vec2(-0.5f,-0.5f), vec3(0,0,1), 0,   0, 0, 255 }, // Bottom-left
    { vec2(0.5f,-0.5f), vec3(0,0,1), 0,   0, 0, 255 }, // Bottom-right

    { vec2(-0.5f, 0.5f), vec3(0,0,1), 0, 255, 0, 255 }, // Top-left
    { vec2(0.5f,-0.5f), vec3(0,0,1), 0,   0, 0, 255 }, // Bottom-right
    { vec2(0.5f, 0.5f), vec3(0,0,1), 0, 255, 0, 255 }, // Top-right
};
static const uint16 g_cylinderIndices[] =
{
    0,2,1, 3,5,4
};
fw::Mesh* createCylinderMesh(float heightHalf, float radius, int numFaces)
{
    std::vector<VertexFormat_PosNormalColor> worldVerts;
    worldVerts.reserve(numFaces*2);

    std::vector<vec3> points;
    points.reserve(numFaces * 2);

    // Create the Top Points of the cylinder 
    for (int i = 0; i < numFaces; i++)
    {
        float angle = ((float)i / numFaces) * 360.0f;
        angle = fw::degreesToRads(angle);
        vec3 pointTop = vec3(cosf(angle), sinf(angle), heightHalf);
        vec3 normal = pointTop;
        pointTop.x *= radius;
        pointTop.y *= radius;

        points.push_back(pointTop);

        worldVerts.push_back({ pointTop, normal, 255, 255, 255, 255 });
    }

    for (int i = 0; i < numFaces; i++)
    {
        float angle = ((float)i / numFaces) * 360.0f;
        angle = fw::degreesToRads(angle);
        vec3 pointBottom = vec3(cosf(angle), sinf(angle), heightHalf);
        vec3 normal = pointBottom;
        pointBottom.x *= radius;
        pointBottom.y *= radius;
        pointBottom.z *= -1;

        points.push_back(pointBottom);

        worldVerts.push_back({ pointBottom, normal, 255, 255, 255, 255 });
    }

    // Create the top and bottom faces
    for (int i = 0; i < numFaces; i++) // top
    {
        worldVerts.push_back({ points[i], vec3(0,1,0), 255, 255, 255, 255});
    }
    // Create the top and bottom faces
    for (int i = 0; i < numFaces; i++) // bottom
    {
        worldVerts.push_back({ points[i+numFaces], vec3(0,-1,0), 255, 255, 255, 255 });
    }

    std::vector<uint16> planeIndinces;
    planeIndinces.reserve(numFaces * 4 * 6);

    // Create the Cylinder part of the indices
    for (int i = 0; i < numFaces - 1; i++)
    {
        planeIndinces.push_back(i);
        planeIndinces.push_back(i + numFaces);
        planeIndinces.push_back(i + numFaces + 1);

        planeIndinces.push_back(i);
        planeIndinces.push_back(i + numFaces + 1);
        planeIndinces.push_back(i + 1);
    }
    planeIndinces.push_back(numFaces-1);
    planeIndinces.push_back(numFaces * 2 - 1);
    planeIndinces.push_back(numFaces);

    planeIndinces.push_back(numFaces - 1);
    planeIndinces.push_back(numFaces);
    planeIndinces.push_back(0);

    // Create the top and bottom faces parts of the indices
    for (int i = numFaces * 2; i < numFaces * 3 - 1; i++) // top
    {
        planeIndinces.push_back(numFaces * 2);
        planeIndinces.push_back(i + 1);
        planeIndinces.push_back(i + 2);
    }

    for (int i = numFaces * 3; i < numFaces * 4 - 1; i++) // bottom
    {
        planeIndinces.push_back(numFaces * 3);
        planeIndinces.push_back(i + 2);
        planeIndinces.push_back(i + 1);
    }

    VertexFormat_PosNormalColor* vertsData = worldVerts.data();
    uint32 vertsSize = sizeof(VertexFormat_PosNormalColor) * (int)worldVerts.size();

    uint16* indincesData = planeIndinces.data();
    uint32 indincesSize = sizeof(uint16) * (int)planeIndinces.size();

    return new fw::Mesh(VertexFormat_PosNormalColor::format, vertsData, vertsSize, indincesData, indincesSize);
}

//=======================
// Plane Mesh
//=======================

void generateVert(vec2 worldSize, ivec2 vertCount, vec2 uvRepeatCount, float noiseScale, std::vector<VertexFormat_PosNormalUV>* verts)
{
    vec2 posStepSize = { worldSize.x / (vertCount.x - 1), worldSize.y / (vertCount.y - 1) };

    vec2 noiseRange = 100;
    vec2 noiseStepSize = { noiseRange.x / (vertCount.x - 1), noiseRange.y / (vertCount.y - 1) };

    for (int y = 0; y < vertCount.y; y++)
    {
        for (int x = 0; x < vertCount.x; x++)
        {
            float height = (float)fw::perlin::ValueNoise_2D(x * noiseStepSize.x, y * noiseStepSize.y) * noiseScale;
            vec3 pos = { x * posStepSize.x, height, y * posStepSize.y };
            vec3 normal = { 0,1,0 };
            vec2 uv = { pos.x * uvRepeatCount.x / worldSize.x , pos.z * uvRepeatCount.y / worldSize.y };

            verts->push_back({ pos, normal, uv });
        }
    }
}

void generateIndices(vec2 worldSize, ivec2 vertCount, std::vector<uint16>* indinces)
{
    for (int iy = 0; iy < vertCount.y - 1; iy++)
    {
        for (int ix = 0; ix < vertCount.x - 1; ix++)
        {
            int bottomLeft = iy * vertCount.x + ix;
            indinces->push_back(bottomLeft);
            indinces->push_back(bottomLeft + vertCount.x);
            indinces->push_back(bottomLeft + 1);
                    
            indinces->push_back(bottomLeft + vertCount.x);
            indinces->push_back(bottomLeft + vertCount.x + 1);
            indinces->push_back(bottomLeft + 1);
        }
    }
}

vec3 crossProd(vec3 A, vec3 B)
{
    A = vec3(A.x, A.z, A.y);
    B = vec3(B.x, B.z, B.y);

    vec3 result = vec3(
        A.y * B.z - A.z * B.y,
        A.z * B.x - A.x * B.z,
        A.x * B.y - A.y * B.x
    );

    return vec3(result.x, result.z, result.y);
}

void generateNormal(vec2 worldSize, ivec2 vertCount, std::vector<VertexFormat_PosNormalUV>* verts)
{
    for (int y = 0; y < vertCount.y; y++)
    {
        for (int x = 0; x < vertCount.x; x++)
        {
            int index = y * vertCount.x + x;

            int leftI = x - 1 < 0 ? index : index - 1;
            int rightI = x + 1 >= vertCount.x ? index : index + 1;
            int upI = y + 1 >= vertCount.x ? index : index + vertCount.x;
            int downI = y - 1 < 0 ? index : index - vertCount.x;

            vec3 Left = (*verts)[leftI].pos;
            vec3 Right = (*verts)[rightI].pos;
            vec3 Up = (*verts)[upI].pos;
            vec3 Down = (*verts)[downI].pos;

            (*verts)[index].normal = crossProd(Down - Up, Right - Left).normalize();
        }
    }
}

fw::Mesh* createPlaneMesh(vec2 worldSize, ivec2 vertCount, vec2 uvRepeatCount, float noiseScale)
{
    // Create the vertex list.
    int numVerts = vertCount.x * vertCount.y;

    std::vector<VertexFormat_PosNormalUV> worldVerts;
    worldVerts.reserve(numVerts);

    generateVert(worldSize, vertCount, uvRepeatCount, noiseScale, &worldVerts);

    int numIndices = numVerts * 6;
    std::vector<uint16> planeIndinces;
    planeIndinces.reserve(numIndices);

    generateIndices(worldSize, vertCount, &planeIndinces);

    generateNormal(worldSize, vertCount, &worldVerts);

    VertexFormat_PosNormalUV* vertsData = worldVerts.data();
    uint32 vertsSize = sizeof(VertexFormat_PosNormalUV) * (int)worldVerts.size();
    
    uint16* indincesData = planeIndinces.data();
    uint32 indincesSize = sizeof(uint16) * (int)planeIndinces.size();

    fw::Mesh* pMesh = new fw::Mesh(
        VertexFormat_PosNormalUV::format, vertsData, vertsSize, indincesData, indincesSize);
    return pMesh;
}

fw::Mesh* createHeightMapMesh(vec2 worldSize, ivec2 vertCount, vec2 uvRepeatCount, float noiseScale)
{
    // Create the vertex list.
    int numVerts = vertCount.x * vertCount.y;

    std::vector<vec3> positions;
    positions.reserve(numVerts);
    std::vector<VertexFormat_PosNormalUV> worldVerts;
    worldVerts.reserve(numVerts);

    generateVert(worldSize, vertCount, uvRepeatCount, noiseScale, &worldVerts);

    // Creating Island elevation
    int curveFalloff = 10;

    for (int y = 0; y < vertCount.y; y++)
    {
        for (int x = 0; x < vertCount.x; x++)
        {
            int index = y * vertCount.x + x;
            vec3 pos = worldVerts[index].pos;
            float newHeight = (cosf((pos.x + worldSize.x / 2) * PI / worldSize.x) * cosf((pos.z + worldSize.y / 2) * PI / worldSize.y) * curveFalloff) * pos.y;
            worldVerts[index].pos.y = newHeight;
        }
    }

    int numIndices = numVerts * 6;
    std::vector<uint16> planeIndinces;
    planeIndinces.reserve(numIndices);

    generateIndices(worldSize, vertCount, &planeIndinces);

    generateNormal(worldSize, vertCount, &worldVerts);

    // Add the position
    for (int i = 0; i < numVerts; i++)
    {
        positions.push_back(worldVerts[i].pos);
    }

    VertexFormat_PosNormalUV* vertsData = worldVerts.data();
    uint32 vertsSize = sizeof(VertexFormat_PosNormalUV) * (int)worldVerts.size();

    uint16* indincesData = planeIndinces.data();
    uint32 indincesSize = sizeof(uint16) * (int)planeIndinces.size();

    fw::Mesh* pMesh = new HeightmapMesh(
        VertexFormat_PosNormalUV::format, vertsData, vertsSize, indincesData, indincesSize,
        vertCount, worldSize, positions);
    return pMesh;
}


fw::Mesh* loadObjMesh(const char* objFileName)
{
    uint32 length = 0;
    char* buffer = fw::loadCompleteFile(objFileName, &length);
    if (buffer == 0 || length == 0)
    {
        delete[] buffer;
        return nullptr;
    }
    
    // Split the string at line endings
    char* next_token = 0;
    char* line = strtok_s(buffer, "\n", &next_token);
    
    // Saving Data
    std::vector<vec3> posData;
    posData.reserve(length / 4);
    std::vector<vec2> uvData;
    uvData.reserve(length / 4);
    std::vector<vec3> normalData;
    normalData.reserve(length / 4);

    std::vector<VertexFormat_PosNormalUV> verts;
    std::vector<uint16> indices;

    int indiceCounter = 0;
    
    // Keep looping as long as there are lines:
    while (line)
    {
        // Print out the line to see if it's working.
        //fw::outputMessage("%s\n", line);
    
        // TODO: Process the line.
        // %s looking for string, %f looking for float, %i OR %d looking for int, if there is a space must add it in the string to show there is a space there
        // %*... means ignore this ... (string/int/float/etc)
    
        if (line[0] == 'v')
        {
            if (line[1] == ' ')
            {
                vec3 pos;
                sscanf(line, "%*s %f %f %f", &pos.x,&pos.y,&pos.z);
                posData.push_back(pos);
            }
            else if (line[1] == 't')
            {
                vec2 uv;
                sscanf(line, "%*s %f %f", &uv.x, &uv.y);
                uvData.push_back(uv);
            }
            else if (line[1] == 'n')
            {
                vec3 normal;
                sscanf(line, "%*s %f %f %f", &normal.x, &normal.y, &normal.z);
                normalData.push_back(normal);
            }
        }
        else if (line[0] == 'f' && line[1] == ' ')
        {
            // f v/vt/vn
            vec3 index1, index2, index3;
            sscanf(line, "%*s %f/%f/%f %f/%f/%f %f/%f/%f", &index1.x, &index1.y, &index1.z, &index2.x, &index2.y, &index2.z, &index3.x, &index3.y, &index3.z);
            index1 -= vec3(1, 1, 1); // make into index
            index2 -= vec3(1, 1, 1); // make into index
            index3 -= vec3(1, 1, 1); // make into index
            verts.push_back({ posData[index1.x], normalData[index1.z], uvData[index1.y] });
            verts.push_back({ posData[index2.x], normalData[index2.z], uvData[index2.y] });
            verts.push_back({ posData[index3.x], normalData[index3.z], uvData[index3.y] });

            indices.push_back(indiceCounter);
            indices.push_back(indiceCounter + 1);
            indices.push_back(indiceCounter + 2);
            indiceCounter += 3;
        }
    
    
        //printf("%s -> %d\n", type, i);
    
        // Go to the next line
        line = strtok_s(0, "\n", &next_token);
    }
    
    // Create the Vertex
    // And push them in the counter clock wise order

    
    VertexFormat_PosNormalUV* vertData = verts.data();
    int vertDataSize = sizeof(VertexFormat_PosNormalUV) * (int)verts.size();
    uint16* indexData = indices.data();
    int indexDataSize = sizeof(uint16) * (int)indices.size();
    
    // Don't leak memory.
    delete[] buffer;

    fw::Mesh* pMesh = new fw::Mesh(VertexFormat_PosNormalUV::format, vertData, vertDataSize, indexData, indexDataSize);

    return pMesh;
}
