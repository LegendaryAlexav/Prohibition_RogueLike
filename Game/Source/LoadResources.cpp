//
// Copyright (c) 2022-2023 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "Framework.h"

#include "Meshes/Shapes.h"

#define getMesh pResources->get<fw::Mesh>
#define getShader pResources->get<fw::ShaderProgram>
#define getTexture pResources->get<fw::Texture>
#define getMaterial pResources->get<fw::Material>

void loadResources(fw::ResourceManager* pResources)
{
    // Create some meshes.
#pragma region - Mesh -

    pResources->add<fw::Mesh>( "Triangle", createTriangleMesh() );
    pResources->add<fw::Mesh>( "Square", createSquareMesh() );
    pResources->add<fw::Mesh>( "Sprite", createSpriteMesh() );
    pResources->add<fw::Mesh>( "TwoSidedSprite", createTwoSidedSpriteMesh() );
    pResources->add<fw::Mesh>( "Square3x3", create3x3SquareMesh() );
    pResources->add<fw::Mesh>( "Cylinder", createCylinderMesh(1.0f, 0.5f));

    pResources->add<fw::Mesh>( "CubeSprite", createCubeSpriteMesh() );
    pResources->add<fw::Mesh>( "Cube", createLazyCubeMesh());
    
    pResources->add<fw::Mesh>( "Plane", createPlaneMesh(vec2(100.0f, 100.0f), ivec2(32, 32), vec2(100.0f)));
    pResources->add<fw::Mesh>( "IslandPlane", createHeightMapMesh(vec2(50.0f, 50.0f), ivec2(100, 100), vec2(10, 10), 2.5f));

    pResources->add<fw::Mesh>( "TriangleObj", loadObjMesh("Data/Objects/Triangle.obj"));
    pResources->add<fw::Mesh>( "SphereObj", loadObjMesh("Data/Objects/SphereObj.obj"));
    pResources->add<fw::Mesh>( "TreeLowPoly", loadObjMesh("Data/Objects/TreeLowPoly.obj"));

#pragma endregion

    // Load some shaders.
#pragma region - ShaderProgram -

    pResources->add<fw::ShaderProgram>( "SolidColor", new fw::ShaderProgram( "Data/Shaders/", "SolidColor.vert.bin", "SolidColor.frag.bin" ) );
    pResources->add<fw::ShaderProgram>( "SolidColorLit", new fw::ShaderProgram( "Data/Shaders/", "SolidColorLit.vert.bin", "SolidColorLit.frag.bin" ) );
    pResources->add<fw::ShaderProgram>( "VertexColor", new fw::ShaderProgram( "Data/Shaders/", "VertexColor.vert.bin", "VertexColor.frag.bin" ) );
    pResources->add<fw::ShaderProgram>( "NormalColor", new fw::ShaderProgram( "Data/Shaders/", "NormalColor.vert.bin", "NormalColor.frag.bin" ) );
    pResources->add<fw::ShaderProgram>( "Texture", new fw::ShaderProgram( "Data/Shaders/", "Texture.vert.bin", "Texture.frag.bin" ) );
    pResources->add<fw::ShaderProgram>( "TextureLit", new fw::ShaderProgram( "Data/Shaders/", "TextureLit.vert.bin", "TextureLit.frag.bin" ) );
    pResources->add<fw::ShaderProgram>( "Water", new fw::ShaderProgram("Data/Shaders/", "Water.vert.bin", "Water.frag.bin"));
    pResources->add<fw::ShaderProgram>( "GrassLit", new fw::ShaderProgram("Data/Shaders/", "GrassLit.vert.bin", "GrassLit.frag.bin"));
    pResources->add<fw::ShaderProgram>( "Terrain", new fw::ShaderProgram("Data/Shaders/", "Terrain.vert.bin", "Terrain.frag.bin"));

#pragma endregion

    // Load some textures.
#pragma region - Texture -
    pResources->add<fw::Texture>( "MegaMan", new fw::Texture( "Data/Textures/MegaMan.png" ) );
    pResources->add<fw::Texture>( "Grass", new fw::Texture( "Data/Textures/Grass.png" ) );
    pResources->add<fw::Texture>( "Thanos", new fw::Texture( "Data/Textures/Thanos.png" ) );
    pResources->add<fw::Texture>( "ShitBoxThanos", new fw::Texture( "Data/Textures/ShitBoxThanos.png" ) );
    pResources->add<fw::Texture>( "6Sides", new fw::Texture( "Data/Textures/6Sides.png" ) );
    pResources->add<fw::Texture>( "Water", new fw::Texture("Data/Textures/Water.png"));

#pragma endregion

    // Create some materials.
#pragma region - Material -
    pResources->add<fw::Material>( "Red", new fw::Material( getShader("SolidColor"), nullptr, fw::color4f::red(), false ) );
    pResources->add<fw::Material>( "Blue", new fw::Material( getShader("SolidColor"), nullptr, fw::color4f::blue(), false ) );
    pResources->add<fw::Material>( "Green", new fw::Material( getShader("SolidColor"), nullptr, fw::color4f::green(), false ) );

    pResources->add<fw::Material>( "WhiteLit", new fw::Material( getShader("SolidColorLit"), nullptr, fw::color4f::white(), false, 100.0f ) );
    pResources->add<fw::Material>( "YellowLit", new fw::Material( getShader("SolidColorLit"), nullptr, fw::color4f::yellow(), false, 100.0f ) );
    pResources->add<fw::Material>( "BlueLit", new fw::Material( getShader("SolidColorLit"), nullptr, fw::color4f::blue(), false, 100.0f ) );

    pResources->add<fw::Material>( "VertexColor", new fw::Material( getShader("VertexColor"), nullptr, fw::color4f::white(), false ) );
    pResources->add<fw::Material>( "NormalColor", new fw::Material( getShader("NormalColor"), nullptr, fw::color4f::white(), true ) );
    
    pResources->add<fw::Material>( "MegaMan", new fw::Material( getShader("Texture"), getTexture("MegaMan"), fw::color4f::white(), true ) );
    pResources->add<fw::Material>( "MegaManLit", new fw::Material( getShader("TextureLit"), getTexture("MegaMan"), fw::color4f::white(), true ) );
    pResources->add<fw::Material>( "Grass", new fw::Material( getShader("GrassLit"), getTexture("Grass"), fw::color4f::green(), true ) );
    pResources->add<fw::Material>( "Thanos", new fw::Material( getShader("Texture"), getTexture("Thanos"), fw::color4f::white(), true ) );
    pResources->add<fw::Material>( "ShitBoxThanos", new fw::Material( getShader("Texture"), getTexture("ShitBoxThanos"), fw::color4f::white(), true ) );
   
    pResources->add<fw::Material>( "6Sides", new fw::Material( getShader("Texture"), getTexture("6Sides"), fw::color4f::white(), true ) );
    
    pResources->add<fw::Material>( "Terrain", new fw::Material( getShader("Terrain"), nullptr, fw::color4f::white(), true ) );
    pResources->add<fw::Material>( "Water", new fw::Material( getShader("Water"), getTexture("Water"), fw::color4f::white(), true ) );
    pResources->add<fw::Material>( "NormalTerrain", new fw::Material( getShader("NormalColor"), nullptr, fw::color4f(1.0f, 0.0f, 1.0f, 1.0f), true ) );
    //pResources->add<fw::Material>( "DebugDots", new fw::Material(getShader("SolidColor"), nullptr, fw::color4f::green(), false));
    pResources->add<fw::Material>("DebugDots", new fw::Material(getShader("Water"), getTexture("Water"), fw::color4f::white(), false));

    fw::Material* pDebugDots = pResources->get<fw::Material>("DebugDots");
    int32 currentFlags = pDebugDots->getRenderStateFlags();
    pDebugDots->setRenderStateFlags(currentFlags | fw::Material::RenderStateFlag::RenderAsDots);

#pragma endregion

}
