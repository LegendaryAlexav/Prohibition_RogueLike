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
    pResources->add<fw::Mesh>( "Triangle", createTriangleMesh() );
    pResources->add<fw::Mesh>( "Square", createSquareMesh() );
    pResources->add<fw::Mesh>( "Sprite", createSpriteMesh() );
    pResources->add<fw::Mesh>( "Cube", createLazyCubeMesh() );
    pResources->add<fw::Mesh>( "Water", createPlaneMesh(vec2(50.0f,50.0f), ivec2(150,150), vec2(10,10)) );
    pResources->add<fw::Mesh>( "Heightmap", createHeightmap(vec2(50.0f,50.0f), 40, ivec2(32,32), vec2(10,10)) );
    pResources->add<fw::Mesh>( "Tree", loadMesh("Data/Meshes/Tree.obj", 0.2f) );
    pResources->add<fw::Mesh>( "YellowChick", loadMesh("Data/Meshes/YellowChick.obj", 0.2f, vec3(0, 1.25f, 0)) );
    pResources->add<fw::Mesh>( "Deer", loadMesh("Data/Meshes/deer.obj", 0.6f, vec3(0,0.5f,0)) );
    pResources->add<fw::Mesh>( "Cylinder", createCylinder(1, 0.2f, 16, 0) );

    // Load some shaders.
    pResources->add<fw::ShaderProgram>( "SolidColor", new fw::ShaderProgram( "Data/Shaders/", "SolidColor.vert.bin", "SolidColor.frag.bin" ) );
    pResources->add<fw::ShaderProgram>( "SolidColorLit", new fw::ShaderProgram( "Data/Shaders/", "SolidColorLit.vert.bin", "SolidColorLit.frag.bin" ) );
    pResources->add<fw::ShaderProgram>( "VertexColor", new fw::ShaderProgram( "Data/Shaders/", "VertexColor.vert.bin", "VertexColor.frag.bin" ) );
    pResources->add<fw::ShaderProgram>( "Texture", new fw::ShaderProgram( "Data/Shaders/", "Texture.vert.bin", "Texture.frag.bin" ) );
    pResources->add<fw::ShaderProgram>( "TextureLit", new fw::ShaderProgram( "Data/Shaders/", "TextureLit.vert.bin", "TextureLit.frag.bin" ) );
    pResources->add<fw::ShaderProgram>( "Water", new fw::ShaderProgram( "Data/Shaders/", "Water.vert.bin", "Water.frag.bin" ) );
    pResources->add<fw::ShaderProgram>( "Terrain", new fw::ShaderProgram( "Data/Shaders/", "Terrain.vert.bin", "Terrain.frag.bin" ) );

    // Load some textures.
    pResources->add<fw::Texture>( "MegaMan", new fw::Texture( "Data/Textures/MegaMan.png" ) );
    pResources->add<fw::Texture>( "Water", new fw::Texture( "Data/Textures/Water.png" ) );
    pResources->add<fw::Texture>( "Magica", new fw::Texture( "Data/Textures/MagicaTexture1.png" ) );
    pResources->add<fw::Texture>( "DeerTexture", new fw::Texture( "Data/Textures/deer.png" ) );
    pResources->add<fw::Texture>( "LightProjectionTex", new fw::Texture() );

    // Create some materials.
    pResources->add<fw::Material>( "White", new fw::Material( getShader("SolidColor"), nullptr, fw::color4f::white(), false ) );
    pResources->add<fw::Material>( "Red", new fw::Material( getShader("SolidColor"), nullptr, fw::color4f::red(), false ) );
    pResources->add<fw::Material>( "Blue", new fw::Material( getShader("SolidColor"), nullptr, fw::color4f::blue(), false ) );
    pResources->add<fw::Material>( "Green", new fw::Material( getShader("SolidColor"), nullptr, fw::color4f::green(), false ) );
    pResources->add<fw::Material>( "VertexColor", new fw::Material( getShader("VertexColor"), nullptr, fw::color4f::white(), false ) );
    pResources->add<fw::Material>( "MegaMan", new fw::Material( getShader("Texture"), getTexture("MegaMan"), fw::color4f::white(), true ) );
    pResources->add<fw::Material>( "DebugDots", new fw::Material(getShader("Terrain"), getTexture("Water"), fw::color4f::green(), false));
    pResources->add<fw::Material>( "Water", new fw::Material(getShader("TextureLit"), getTexture("Water"), fw::color4f::green(), false));
    pResources->add<fw::Material>( "Terrain", new fw::Material(getShader("Terrain"), nullptr, fw::color4f::green(), false));

    pResources->add<fw::Material>( "Magica", new fw::Material( getShader("TextureLit"), getTexture("Magica"), fw::color4f::white(), false ) );
    pResources->add<fw::Material>( "Deer", new fw::Material( getShader("TextureLit"), getTexture("DeerTexture"), fw::color4f::white(), false ) );
    pResources->add<fw::Material>( "MegaManLit", new fw::Material( getShader("TextureLit"), getTexture("MegaMan"), fw::color4f::white(), true ) );
    pResources->add<fw::Material>( "WhiteLit", new fw::Material( getShader("SolidColorLit"), nullptr, fw::color4f::white(), false ) );
    pResources->add<fw::Material>( "BlueLit", new fw::Material( getShader("SolidColorLit"), nullptr, fw::color4f(0.2f,0.2f,1,1), false ) );
    pResources->add<fw::Material>( "GreenLit", new fw::Material( getShader("SolidColorLit"), nullptr, fw::color4f(0.2f,1,0.2f,1), false ) );
    pResources->add<fw::Material>( "YellowLit", new fw::Material( getShader("SolidColorLit"), nullptr, fw::color4f(1,1,0.2f,1), false ) );

    pResources->add<fw::Material>( "LightProjectionMat", new fw::Material( getShader("Texture"), getTexture("LightProjectionTex"), fw::color4f(1,1,1.0f,1), false ) );

    fw::Material* pDebugDots = pResources->get<fw::Material>("DebugDots");
    int32 currentFlags = pDebugDots->getRenderStateFlags();
    pDebugDots->setRenderStateFlags(currentFlags | fw::Material::RenderStateFlag::RenderAsDots);
}
