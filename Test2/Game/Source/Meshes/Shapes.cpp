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

//=======================
// Triangle
//=======================
static const VertexFormat_PosNormalColor g_TriangleVerts[] =
{
    { vec3( 0.0f, 0.5f, 0.0f), vec3(0,0,-1), 255, 0, 0, 255 }, // Top
    { vec3(-0.5f,-0.5f, 0.0f), vec3(0,0,-1), 0, 255, 0, 255 }, // Bottom-left
    { vec3( 0.5f,-0.5f, 0.0f), vec3(0,0,-1), 0, 0, 255, 255 }, // Bottom-right
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
// Square
//=======================
static const VertexFormat_PosNormalColor g_SquareVerts[] =
{
    { vec3(-0.5f, 0.5f, 0.0f), vec3(0,0,-1), 0, 255, 0, 255 }, // Top-left
    { vec3(-0.5f,-0.5f, 0.0f), vec3(0,0,-1), 0,   0, 0, 255 }, // Bottom-left
    { vec3( 0.5f,-0.5f, 0.0f), vec3(0,0,-1), 0,   0, 0, 255 }, // Bottom-right

    { vec3(-0.5f, 0.5f, 0.0f), vec3(0,0,-1), 0, 255, 0, 255 }, // Top-left
    { vec3( 0.5f,-0.5f, 0.0f), vec3(0,0,-1), 0,   0, 0, 255 }, // Bottom-right
    { vec3( 0.5f, 0.5f, 0.0f), vec3(0,0,-1), 0, 255, 0, 255 }, // Top-right
};
static const uint16 g_SquareIndices[] =
{
    0,2,1, 3,5,4
};
fw::Mesh* createSquareMesh()
{
    return new fw::Mesh( VertexFormat_PosNormalColor::format, g_SquareVerts, sizeof(g_SquareVerts), g_SquareIndices, sizeof(g_SquareIndices) );
}

//=======================
// Sprite
//=======================
static const VertexFormat_PosNormalUV g_SpriteVerts[] =
{
    // Front
    { vec3(-0.5f, 0.5f,0.0f), vec3(0,0,-1), vec2(0,1) }, // Top-left
    { vec3(-0.5f,-0.5f,0.0f), vec3(0,0,-1), vec2(0,0) }, // Bottom-left
    { vec3( 0.5f,-0.5f,0.0f), vec3(0,0,-1), vec2(1,0) }, // Bottom-right
    { vec3( 0.5f, 0.5f,0.0f), vec3(0,0,-1), vec2(1,1) }, // Top-right
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
// Lazy Cube
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

void createPlaneVerts(vec2 worldSize, ivec2 vertCount, vec2 uvRepeatCount, std::vector<vec3>& positions, std::vector<VertexFormat_PosNormalUV>& verts, std::vector<uint16>& indices)
{
    // Create the vertex list.
    int numVerts = vertCount.x * vertCount.y;

    positions.reserve( numVerts );
    verts.reserve( numVerts );

    vec2 posStepSize = { worldSize.x / (vertCount.x-1), worldSize.y / (vertCount.y-1) };
    vec2 uvStepSize = { uvRepeatCount.x / (vertCount.x-1), uvRepeatCount.y / (vertCount.y-1) };

    for( int y=0; y<vertCount.y; y++ )
    {
        for( int x=0; x<vertCount.x; x++ )
        {
            vec3 pos = { x * posStepSize.x, 0, y * posStepSize.y };
            vec3 normal = { 0, 1, 0 };
            vec2 uv = { x * uvStepSize.x, y * uvStepSize.y };

            positions.push_back( pos );
            verts.push_back( {pos, normal, uv} );
        }
    }

    // Temp code to create the index list as a single list of points.
    int numIndices = (vertCount.x-1) * (vertCount.y-1) * 6;

    indices.reserve( numIndices );

    for( int y = 0; y < vertCount.y - 1; y++ )
    {
        for( int x = 0; x < vertCount.x - 1; x++ )
        {
            int bottomLeftIndex = y*vertCount.x + x;

            indices.push_back( bottomLeftIndex + 0 );
            indices.push_back( bottomLeftIndex + vertCount.x );
            indices.push_back( bottomLeftIndex + vertCount.x+1 );

            indices.push_back( bottomLeftIndex + 0 );
            indices.push_back( bottomLeftIndex + vertCount.x+1 );
            indices.push_back( bottomLeftIndex + 1 );
        }
    }
}

fw::Mesh* createPlaneMesh(vec2 worldSize, ivec2 vertCount, vec2 uvRepeatCount)
{
    std::vector<vec3> positions;
    std::vector<VertexFormat_PosNormalUV> verts;
    std::vector<uint16> indices;

    createPlaneVerts( worldSize, vertCount, uvRepeatCount, positions, verts, indices );

    fw::Mesh* pMesh = new fw::Mesh(
        VertexFormat_PosNormalUV::format,
        verts.data(), sizeof(VertexFormat_PosNormalUV) * (int)verts.size(),
        indices.data(), sizeof(uint16) * (int)indices.size() );
    return pMesh;
}

fw::Mesh* createHeightmap(vec2 worldSize, float maxHeight, ivec2 vertCount, vec2 uvRepeatCount)
{
    std::vector<vec3> positions;
    std::vector<VertexFormat_PosNormalUV> verts;
    std::vector<uint16> indices;

    createPlaneVerts( worldSize, vertCount, uvRepeatCount, positions, verts, indices );

    vec2 noiseRange = 100;
    vec2 noiseStep = { noiseRange.x / (vertCount.x - 1), noiseRange.y / (vertCount.y - 1) };

    // Figure out the height of each vertex using Perlin noise.
    // Taper the noise so it goes to zero around the boundary.
    for( int y=0; y<vertCount.y; y++ )
    {
        for( int x=0; x<vertCount.x; x++ )
        {
            int vertIndex = y * vertCount.x + x;

            // Taper noise to zero at edges of heightmap.
            float distanceFromCenter = ivec2(x,y).distanceFrom( ivec2(vertCount.x, vertCount.y)/2 );
            float maxDistance = vertCount.x / 2.0f;
            float taper = 1.0f-(distanceFromCenter / maxDistance);
            fw::myClamp( taper, 0.0f, 1.0f );
            verts[vertIndex].pos.y *= taper;

            float height = (float)fw::perlin::ValueNoise_2D(400 + x * noiseStep.x, y * noiseStep.y) * maxHeight;
            height *= taper;

            positions[vertIndex].y = height;
            verts[vertIndex].pos.y = height;
        }
    }

    bool useShortcutNormals = true;

    // Shortcut normals.
    if( useShortcutNormals )
    {
        for( int y=0; y<vertCount.y; y++ )
        {
            for( int x=0; x<vertCount.x; x++ )
            {
                int vertIndex = y * vertCount.x + x;

                int right = x+1 < vertCount.x ? y * vertCount.x + (x+1) : vertIndex;
                int left =  x-1 >= 0          ? y * vertCount.x + (x-1) : vertIndex;
                int up =    y+1 < vertCount.y ? (y+1) * vertCount.x + x : vertIndex;
                int down =  y-1 >= 0          ? (y-1) * vertCount.x + x : vertIndex;

                vec3 h = positions[right] - positions[left];
                vec3 v = positions[up] - positions[down];

                vec3 normal = v.cross(h).getNormalized();
                verts[vertIndex].normal = normal;
            }
        }
    }
    else // Calculate normals using all 6 triangles.
    {
        for( int y=0; y<vertCount.y; y++ )
        {
            for( int x=0; x<vertCount.x; x++ )
            {
                // My beautiful ASCII art.
                // *---*---*
                // |  /|0 /|
                // | / | / |
                // |/ 2|/ 1|
                // *---*---*
                // |3 /|5 /|
                // | / | / |
                // |/ 4|/  |
                // *---*---*

                int maxX = vertCount.x - 1;
                int maxY = vertCount.y - 1;

                int vertIndex = y * vertCount.x + x;

                int center = vertIndex;
                int right = vertIndex + 1;
                int left = vertIndex - 1;
                int up = vertIndex + vertCount.x;
                int down = vertIndex - vertCount.x;
                int upright = vertIndex + vertCount.x + 1;
                int downleft = vertIndex - vertCount.x - 1;

                // Get the normal for all 6 triangles.
                vec3 normalSum = 0;
                int numTris = 0;

                for( int triIndex=0; triIndex<6; triIndex++ )
                {
                    // Get the indices.
                    int indices[3];
                    if(      triIndex == 0 && x < maxX && y < maxY ) { indices[0] = center;   indices[1] = up;      indices[2] = upright; }
                    else if( triIndex == 1 && x < maxX && y < maxY ) { indices[0] = center;   indices[1] = upright; indices[2] = right; }
                    else if( triIndex == 2 && x > 0    && y < maxY ) { indices[0] = left;     indices[1] = up;      indices[2] = center; }
                    else if( triIndex == 3 && x > 0    && y > 0 )    { indices[0] = downleft; indices[1] = left;    indices[2] = center; }
                    else if( triIndex == 4 && x > 0    && y > 0 )    { indices[0] = downleft; indices[1] = center;  indices[2] = down; }
                    else if( triIndex == 5 && x < maxX && y > 0 )    { indices[0] = down;     indices[1] = center;  indices[2] = right; }
                    else continue;

                    // Get the vert positions.
                    vec3 v0 = positions[indices[0]];
                    vec3 v1 = positions[indices[1]];
                    vec3 v2 = positions[indices[2]];

                    // Calculate the normal.
                    vec3 edge1 = v1 - v0;
                    vec3 edge2 = v2 - v0;
                    vec3 triNormal = edge1.cross(edge2).getNormalized();

                    // Sum up the normals.
                    normalSum += triNormal;
                    numTris++;
                }

                assert( numTris != 0 );

                // Average the normals.
                verts[vertIndex].normal = normalSum / (float)numTris;
            }
        }
    }

    // Create the heightmap mesh.
    fw::Mesh* pMesh = new HeightmapMesh(
        VertexFormat_PosNormalUV::format,
        verts.data(), sizeof(VertexFormat_PosNormalUV) * (int)verts.size(),
        indices.data(), sizeof(uint16) * (int)indices.size(),
        vertCount, worldSize, positions );
    return pMesh;
}

fw::Mesh* createCylinder(float height, float radius, int ringVertCount, vec3 pivot)
{
    // vertCount is the number of vertices around the circumference of the cylinder.
    // We need 4 sets, 2 sets for the top and bottom rings of vertices facing up and down,
    // and 2 sets for the sides, facing outwards, ordered top, bottom, top, bottom, etc.
    // The side sets have 2 extra points for the UVs to go from 0 to 1 around the cylinder.
    std::vector<VertexFormat_PosNormalUV> verts( ringVertCount + ringVertCount + (ringVertCount+1)*2 );
    std::vector<uint16> indices( (ringVertCount-2)*3 + (ringVertCount-2)*3 + ringVertCount*2*3 );

    float angleStep = 2.0f * PI / ringVertCount;

    // Create two rings of vertices, one at the top and one at the bottom.
    for( int i=0; i<ringVertCount+1; i++ )
    {
        float angle = i * angleStep;
        float x = cosf( angle );
        float z = sinf( angle );

        vec3 point = vec3(x, 0, z) * radius - pivot;

        vec3 posTop = point + vec3(0,height,0);
        vec3 posBtm = point;
        vec3 normal = { x, 0, z };
        vec2 uvTop = { (float)i / ringVertCount, 1 };
        vec2 uvBtm = { (float)i / ringVertCount, 0 };

        if( i < ringVertCount )
        {
            verts[ringVertCount*0 + i] = { posTop, vec3(0,1,0), vec2(x,z)/2.0f+0.5f };
            verts[ringVertCount*1 + i] = { posBtm, vec3(0,-1,0), vec2(x,z)/2.0f+0.5f };
        }
        verts[ringVertCount*2 + i*2+0] = { posTop, normal, uvTop };
        verts[ringVertCount*2 + i*2+1] = { posBtm, normal, uvBtm };
    }

    // Create the index list.
    for( int i=0; i<ringVertCount; i++ )
    {
        // Indices for the top face.
        if( i < ringVertCount - 2 )
        {
            indices[i*3 + 0] = ringVertCount*0 + 0;
            indices[i*3 + 1] = ringVertCount*0 + i + 2;
            indices[i*3 + 2] = ringVertCount*0 + i + 1;
        }

        // Indices for the bottom face.
        if( i < ringVertCount - 2 )
        {
            indices[(ringVertCount-2)*3 + i*3 + 0] = ringVertCount*1 + 0;
            indices[(ringVertCount-2)*3 + i*3 + 1] = ringVertCount*1 + i + 1;
            indices[(ringVertCount-2)*3 + i*3 + 2] = ringVertCount*1 + i + 2;
        }

        // Indices for the sides, alternating top and bottom vertices.
        int topIndex1 = ringVertCount*2 + i*2;
        int btmIndex1 = ringVertCount*2 + i*2 + 1;
        int topIndex2 = ringVertCount*2 + (i+1)*2;
        int btmIndex2 = ringVertCount*2 + (i+1)*2 + 1;

        indices[(ringVertCount-2)*3*2 + i*6 + 0] = topIndex1;
        indices[(ringVertCount-2)*3*2 + i*6 + 1] = btmIndex2;
        indices[(ringVertCount-2)*3*2 + i*6 + 2] = btmIndex1;

        indices[(ringVertCount-2)*3*2 + i*6 + 3] = topIndex2;
        indices[(ringVertCount-2)*3*2 + i*6 + 4] = btmIndex2;
        indices[(ringVertCount-2)*3*2 + i*6 + 5] = topIndex1;
    }

    fw::Mesh* pMesh = new fw::Mesh(
        VertexFormat_PosNormalUV::format,
        verts.data(), sizeof(VertexFormat_PosNormalUV) * (int)verts.size(),
        indices.data(), sizeof(uint16) * (int)indices.size() );
    return pMesh;
}

fw::Mesh* loadMesh(const char* objFilename, vec3 scale, vec3 pivotPoint)
{
    uint32 length = 0;
    char* buffer = fw::loadCompleteFile( objFilename, &length );
    if( buffer == 0 || length == 0 )
    {
        delete[] buffer;
        return nullptr;
    }

    // Split the string at line endings
    //char* next_token = 0;
    //char* line = strtok_s( buffer, "\n", &next_token );
    char* line = strtok( buffer, "\n" );

    std::vector<vec3> positions;
    std::vector<vec3> normals;
    std::vector<vec2> uvs;
    std::vector<VertexFormat_PosNormalUV> verts;
    std::vector<uint16> indices;

    vec3 temp;

    // Keep looping as long as there are lines:
    while( line )
    {
        // Print out the line to see if it's working.
        //fw::outputMessage( "%s\n", line );

        // Process the line.
        if( line[0] == 'v' && line[1] == ' ' )
        {
            //sscanf_s( line, "v %f %f %f", &temp.x, &temp.y, &temp.z );
            sscanf( line, "v %f %f %f", &temp.x, &temp.y, &temp.z );
            temp.z *= -1;
            temp -= pivotPoint;
            positions.push_back( temp * scale );
        }
        else if( line[0] == 'v' && line[1] == 't' )
        {
            //sscanf_s( line, "vt %f %f", &temp.x, &temp.y );
            sscanf( line, "vt %f %f", &temp.x, &temp.y );
            uvs.push_back( temp.xy() );
        }
        else if( line[0] == 'v' && line[1] == 'n' )
        {
            //sscanf_s( line, "vn %f %f %f", &temp.x, &temp.y, &temp.z );
            sscanf( line, "vn %f %f %f", &temp.x, &temp.y, &temp.z );
            temp.z *= -1;

            normals.push_back( temp );
        }
        else if( line[0] == 'f' && line[1] == ' ' )
        {
            int vertexIndex[3];
            int uvIndex[3];
            int normalIndex[3];

            //sscanf_s( line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
            sscanf( line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                      &vertexIndex[0], &uvIndex[0], &normalIndex[0],
                      &vertexIndex[1], &uvIndex[1], &normalIndex[1],
                      &vertexIndex[2], &uvIndex[2], &normalIndex[2] );

            for( int i=2; i>=0; i-- )
            {
                VertexFormat_PosNormalUV vert;
                vert.pos = positions[vertexIndex[i] - 1];
                vert.normal = normals[normalIndex[i] - 1];
                vert.uv = uvs[uvIndex[i] - 1];
                verts.push_back( vert );
                indices.push_back( (uint16)indices.size() );
            }
        }

        // Go to the next line.
        //line = strtok_s( 0, "\n", &next_token );
        line = strtok( 0, "\n" );
    }

    // Don't leak memory.
    delete[] buffer;

    // Create the mesh.
    fw::Mesh* pMesh = new fw::Mesh(
        VertexFormat_PosNormalUV::format,
        verts.data(), sizeof(VertexFormat_PosNormalUV) * (int)verts.size(),
        indices.data(), sizeof(uint16) * (int)indices.size() );
    return pMesh;
}
