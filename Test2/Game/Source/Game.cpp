//
// Copyright (c) 2022-2023 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "Framework.h"

#include "DataTypes.h"
#include "Game.h"
#include "LoadResources.h"

#include "Objects/Deer.h"
#include "Objects/HeightmapMesh.h"
#include "Objects/OrbitCamera.h"
#include "Objects/PhysicsPlayer.h"
#include "Objects/Player.h"

#include "Events/GameEvents.h"
#include "Meshes/VertexFormats.h"
#include "box2d/include/box2d/box2d.h"

Game::Game(fw::FWCore& fwCore)
    : GameCore( fwCore )
    , m_eventListener( std::bind(&Game::onEvent, this, std::placeholders::_1) )
    , m_lightDirectional( vec3(-50,30,-50), vec3(-25,-40,0), vec3(1,1,1), 0.01f )
    , m_lightPoint( vec3(1,-1,1), vec3(0.3f,0.3f,0.3f), 6.0f, 0.01f )
{
    // General renderer settings.
    int viewID = 1;
    bgfx::setViewClear( viewID, BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH, 0x000030ff, 1.0f, 0 );
    bgfx::setViewRect( viewID, 0, 0, m_FWCore.getWindowClientWidth(), m_FWCore.getWindowClientHeight() );

    // Create event manager and register for events.
    m_pEventManager = new fw::EventManager();
    m_pEventManager->registerListener<RemoveFromGameEvent>( &m_eventListener );
    m_pEventManager->registerListener<AreaDamageEvent>( &m_eventListener );
    m_pEventManager->registerListener<fw::CollisionEvent>( &m_eventListener );
    m_pEventManager->registerListener<SpawnBallEvent>( &m_eventListener );

    // Create some manager objects.
    m_pImGuiManager = new fw::ImGuiManager( &m_FWCore, 1, m_pEventManager );
    m_pResources = new fw::ResourceManager();

    // Create uniforms.
    createUniforms();

    // Create vertex formats.
    initVertexFormats();

    // Load Resources.
    loadResources( m_pResources );

    // Create texture and FBO for shadow casting light.
    {
        m_FBOTexture_Color = bgfx::createTexture2D( 4096,4096, false, 1, bgfx::TextureFormat::BGRA8, BGFX_TEXTURE_RT );
        m_FBOTexture_Depth = bgfx::createTexture2D( 4096,4096, false, 1, bgfx::TextureFormat::D32, BGFX_TEXTURE_RT );

        bgfx::TextureHandle FBOTextures[] = { m_FBOTexture_Color, m_FBOTexture_Depth };
        m_FBO = bgfx::createFrameBuffer( 2, FBOTextures, true );

        m_pResources->get<fw::Texture>("LightProjectionTex")->setHandle( m_FBOTexture_Depth );
    }

    // Create physics world.
    m_pPhysicsWorld = fw::createPhysicsWorld( fw::PhysicsEngine::Jolt, m_pEventManager );

    // Create some GameObjects.
#define GETMESH m_pResources->get<fw::Mesh>
#define GETMATERIAL m_pResources->get<fw::Material>

    m_pPlayer = new PhysicsPlayer( this, "Player", vec3(25,2.5f,25), GETMESH("YellowChick"), GETMATERIAL("YellowLit") );
    m_pPlayer->createPhysicsBodyBox( true, vec3(0.5f,0.5f,0.5f) );
    m_objects.push_back( m_pPlayer );

    m_pCamera = new OrbitCamera( this, m_pPlayer );
    m_pCamera->setLookAtPosition( vec3(25,0,25) );

    //m_objects.push_back( new fw::GameObject(this, "Floor", vec3(0,0,0), GETMESH("Cube"), GETMATERIAL("WhiteLit")) );
    //m_objects.back()->setScale( vec3(10,0.1f,10) );
    //m_objects.push_back( new fw::GameObject(this, "LeftBox",  vec3(-3,0.5f,0), GETMESH("Cube"), GETMATERIAL("WhiteLit")) );
    //m_objects.push_back( new fw::GameObject(this, "RightBox", vec3( 3,0.5f,0), GETMESH("Cube"), GETMATERIAL("WhiteLit")) );
    fw::GameObject* pTemp;

    pTemp = new Deer(this, "Deer", vec3(0.8f, 7, -1), GETMESH("Deer"), GETMATERIAL("Deer"));
    pTemp->createPhysicsBodyBox(true, vec3(0.5f, 0.5f, 0.5f));
    m_objects.push_back(pTemp);

    m_objects.push_back( new fw::GameObject( this, "Water", vec3(0,0.2f,0), GETMESH("Water"), GETMATERIAL("Water") ) );
    m_pHeightmap = new fw::GameObject( this, "Heightmap", vec3(0,0,0), GETMESH("Heightmap"), GETMATERIAL("WhiteLit") );
    m_objects.push_back( m_pHeightmap );

    // Plant trees.
    for( int i=0; i<30; i++ )
    {
        float x = fw::Random::randomFloat( 0, 50 );
        float z = fw::Random::randomFloat( 0, 50 );

        HeightmapMesh* pHeightmapMesh = static_cast<HeightmapMesh*>( GETMESH("Heightmap") );
        float height = pHeightmapMesh->getHeightForXZPosition( vec3(x,0,z) );

        m_pHeightmap->createPhysicsBodyTerrain( pHeightmapMesh->getPositions(), pHeightmapMesh->getVertCount().x );

        if( height < 0.5f )
        {
            i--; // Try again.
            continue;
        }

        fw::GameObject* pTemp = new fw::GameObject( this, "Tree", vec3(x,height,z), GETMESH("Tree"), GETMATERIAL("Magica") );
        m_objects.push_back( pTemp );

        float yRot = fw::Random::randomFloat( 0, 360 );
        float scale = fw::Random::randomFloat( 0.8f, 1.2f );
        pTemp->setRotation( vec3(0,yRot,0) );
        pTemp->setScale( vec3(scale) );
    }

    for (int i = 0; i < 10; i++)
    {
        float x = fw::Random::randomFloat(0, 50);
        float z = fw::Random::randomFloat(0, 50);

        HeightmapMesh* pHeightmapMesh = static_cast<HeightmapMesh*>(GETMESH("Heightmap"));
        float height = pHeightmapMesh->getHeightForXZPosition(vec3(x, 0, z));

        m_pHeightmap->createPhysicsBodyTerrain(pHeightmapMesh->getPositions(), pHeightmapMesh->getVertCount().x);

        if (height < 0.5f)
        {
            i--; // Try again.
            continue;
        }

        Deer* pTemp = new Deer(this, "Deer", vec3(x, height, z), GETMESH("Deer"), GETMATERIAL("Deer"));
        pTemp->createPhysicsBodyBox(true, vec3(0.5f, 0.5f, 0.5f));
        m_objects.push_back(pTemp);

        float yRot = fw::Random::randomFloat(0, 360);
        float scale = fw::Random::randomFloat(0.8f, 1.2f);
        pTemp->setRotation(vec3(0, yRot, 0));
        pTemp->setScale(vec3(scale));
    }

    //m_objects.push_back( new fw::GameObject( this, "Object 2", vec3(-3,1,-6), GETMESH("Triangle"), GETMATERIAL("Blue") ) );
    //m_objects.push_back( new fw::GameObject( this, "Object 3", vec3(-1,1,-6), GETMESH("Square"), GETMATERIAL("VertexColor") ) );
    //m_objects.push_back( new fw::GameObject( this, "Object 4", vec3( 1,1,-6), GETMESH("Square"), GETMATERIAL("VertexColor") ) );
    //m_objects.push_back( new fw::GameObject( this, "Object 5", vec3( 3,1,-6), GETMESH("Square"), GETMATERIAL("Blue") ) );

    // Temp objects to test physics.
    if( true )
    {
        fw::GameObject* pTemp;

        pTemp = new fw::GameObject( this, "Ground", vec3(0,5,0), GETMESH("Cube"), GETMATERIAL("YellowLit") );
        pTemp->setScale( vec3(10,1,10) );
        pTemp->createPhysicsBodyBox( false, vec3(10,1,10) );
        m_objects.push_back( pTemp );

        pTemp = new fw::GameObject( this, "Floating Deer", vec3(0.8f,7,0), GETMESH("Deer"), GETMATERIAL("Deer") );
        pTemp->createPhysicsBodyBox( false, vec3(0.5f, 0.5f, 0.5f) );
        m_objects.push_back( pTemp );

        pTemp = new fw::GameObject( this, "Trunk", vec3(-1,5.5f,0), GETMESH("Cylinder"), GETMATERIAL("MegaManLit") );
        m_objects.push_back( pTemp );

        m_pPlayer->setPosition( vec3(0.8f,7,0) );
    }
    else
    {
        fw::GameObject* pTemp;

        pTemp = new fw::GameObject( this, "Block", vec3(25.8f,6,25), GETMESH("Cube"), GETMATERIAL("BlueLit") );
        pTemp->createPhysicsBodyBox( true, vec3(1,1,1) );
        m_objects.push_back( pTemp );
    }
}

Game::~Game()
{
    m_pEventManager->unregisterListener<RemoveFromGameEvent>( &m_eventListener );
    m_pEventManager->unregisterListener<AreaDamageEvent>( &m_eventListener );
    m_pEventManager->unregisterListener<fw::CollisionEvent>( &m_eventListener );
    m_pEventManager->unregisterListener<SpawnBallEvent>( &m_eventListener );

    for( fw::GameObject* pObject : m_objects )
    {
        delete pObject;
    }

    delete m_pCamera;

    delete m_pPhysicsWorld;

    delete m_pImGuiManager;

    delete m_pResources;
    delete m_pUniforms;

    delete m_pEventManager;
}

void Game::createUniforms()
{
    m_pUniforms = new fw::Uniforms();

    m_pUniforms->createUniform( "u_worldMatrix", bgfx::UniformType::Mat4 );
    m_pUniforms->createUniform( "u_viewMatrix", bgfx::UniformType::Mat4 );
    m_pUniforms->createUniform( "u_projMatrix", bgfx::UniformType::Mat4 );
    m_pUniforms->createUniform( "u_lightMatrix", bgfx::UniformType::Mat4 );

    m_pUniforms->createUniform( "u_diffuseColor", bgfx::UniformType::Vec4 );
    m_pUniforms->createUniform( "u_textureColor", bgfx::UniformType::Sampler );
    m_pUniforms->createUniform( "u_textureLightDepth", bgfx::UniformType::Sampler );
    m_pUniforms->createUniform( "u_specularPower", bgfx::UniformType::Vec4 );
    m_pUniforms->createUniform( "u_uvScale", bgfx::UniformType::Vec4 );
    m_pUniforms->createUniform( "u_uvOffset", bgfx::UniformType::Vec4 );

    m_pUniforms->createUniform( "u_cameraPosition", bgfx::UniformType::Vec4 );

#define c_maxLights 4

    m_pUniforms->createUniform( "u_lightPos", bgfx::UniformType::Vec4, c_maxLights );
    m_pUniforms->createUniform( "u_lightColor", bgfx::UniformType::Vec4, c_maxLights );
    m_pUniforms->createUniform( "u_lightRange", bgfx::UniformType::Vec4, c_maxLights );
    m_pUniforms->createUniform( "u_ambientPerc", bgfx::UniformType::Vec4, c_maxLights );

    m_pUniforms->createUniform( "u_time", bgfx::UniformType::Vec4 );
}

void Game::startFrame(float deltaTime)
{
    m_pImGuiManager->startFrame( deltaTime );
    m_pEventManager->processEvents();
}

void Game::onEvent(fw::Event* pEvent)
{
    // Process events.

    // Remove object from GameObject list and delete it.
    if( pEvent->getType() == RemoveFromGameEvent::getStaticEventType() )
    {
        RemoveFromGameEvent* pRemoveFromGameEvent = static_cast<RemoveFromGameEvent*>( pEvent );
        fw::GameObject* pObject = pRemoveFromGameEvent->getGameObject();

        auto it = std::find( m_objects.begin(), m_objects.end(), pObject );
        m_objects.erase( it );

        delete pObject;
    }

    // Apply area damage to all objects within the radius of the event.
    if( pEvent->getType() == AreaDamageEvent::getStaticEventType() )
    {
        AreaDamageEvent* pAreaDamageEvent = static_cast<AreaDamageEvent*>( pEvent );

        for( fw::GameObject* pObject : m_objects )
        {
            // Skip the player.
            if( pObject == m_pPlayer )
                continue;

            // Check if object is within radius of event.
            float distance = pObject->getPosition().distanceFrom( pAreaDamageEvent->getLocation() );
            if( distance <= pAreaDamageEvent->getRadius() )
            {
                // Remove object from game.
                RemoveFromGameEvent* pRemoveEvent = new RemoveFromGameEvent( pObject );
                m_pEventManager->addEvent( pRemoveEvent );
            }
        }
    }

    if( pEvent->getType() == fw::CollisionEvent::getStaticEventType() )
    {
        fw::CollisionEvent* pCollisionEvent = static_cast<fw::CollisionEvent*>( pEvent );

        fw::GameObject* pObject1 = pCollisionEvent->getObject1();
        fw::GameObject* pObject2 = pCollisionEvent->getObject2();

        // Hacky stuff.
        if( pObject1->getName() == "Block" && pObject2->getName() == "PhysicsPlayer" )
        {
            // Remove object from game.
            //RemoveFromGameEvent* pRemoveEvent = new RemoveFromGameEvent( pObject1 );
            //m_pEventManager->addEvent( pRemoveEvent );
        }
    }

    if( pEvent->getType() == SpawnBallEvent::getStaticEventType() )
    {
        SpawnBallEvent* pSpawnBallEvent = static_cast<SpawnBallEvent*>( pEvent );
        fw::GameObject* pTemp = new fw::GameObject( this, "Ball", pSpawnBallEvent->getLocation(), m_pResources->get<fw::Mesh>("Cube"), m_pResources->get<fw::Material>("BlueLit") );
        pTemp->setScale( vec3(pSpawnBallEvent->getRadius()) );
        pTemp->createPhysicsBodySphere( true, pSpawnBallEvent->getRadius() );
        m_objects.push_back( pTemp );
    }
}

void Game::update(float deltaTime)
{
    // Hack to limit deltaTime on breakpoints.
#if _DEBUG
    if( deltaTime > 1/3.0f )
        deltaTime = 1/60.0f;
#endif

    m_pPhysicsWorld->update( deltaTime );

    ImGui::Checkbox( "Wireframe", &m_wireframeMode );
    if( m_wireframeMode )
    {
        bgfx::setDebug( BGFX_DEBUG_WIREFRAME );
    }
    else
    {
        bgfx::setDebug( 0 );
    }

    for( fw::GameObject* pObject : m_objects )
    {
        pObject->update( deltaTime );
    }

    m_pCamera->update( deltaTime );

    editor_displayObjectList();
    editor_displayResources();
    m_lightDirectional.showDebugUI();
    m_lightPoint.showDebugUI();

    m_lightPoint.m_position = m_pPlayer->getPosition() + vec3(0,2,0);

    fw::GameObject* pNearestObject = m_pPhysicsWorld->raycastForNearestObject( vec3(0,10,0), vec3(0,-10,0) );
    ImGui::Text( "Nearest Object: %s", pNearestObject ? pNearestObject->getName().c_str() : "None" );
}

void Game::draw()
{
    int viewID = 0;

    // Draw the scene from the light point of view.
    vec3 light0Dir;
    {
        bgfx::setViewFrameBuffer( viewID, m_FBO );
        bgfx::setViewClear( viewID, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x000000ff, 1.0f, 0 );
        bgfx::setViewRect( viewID, 0, 0, 4096, 4096 );

        // Deal with light "camera"
        {
            mat4 view;
            view.createSRT( 1, m_lightDirectional.m_rotation, m_lightDirectional.m_position );
            light0Dir = view.getAt();
            view.inverse();

            mat4 proj;
            proj.createOrtho( -30, 30, -30, 30, 30, 1000 );

            mat4 vp = proj * view;

            m_pUniforms->set( "u_viewMatrix", view );
            m_pUniforms->set( "u_projMatrix", proj );
            m_pUniforms->set( "u_lightMatrix", vp );
        }

        // Draw all objects using a basic white shader.
        for (fw::GameObject* pObject : m_objects)
        {
            pObject->draw( viewID, GETMATERIAL("White") );
        }
    }

    viewID = 1;

    // Setup time uniforms.
    float time = (float)fw::getSystemTimeSinceGameStart();
    m_pUniforms->set( "u_time", time );

    // Program the view and proj uniforms from the camera.
    m_pCamera->enable( viewID );

    // Set light uniforms.
    m_pUniforms->set( "u_lightPos", light0Dir, 0 );
    m_pUniforms->set( "u_lightColor", m_lightDirectional.m_color, 0 );
    m_pUniforms->set( "u_lightRange", m_lightDirectional.m_range, 0 );
    m_pUniforms->set( "u_ambientPerc", m_lightDirectional.m_ambient, 0 );

    m_pUniforms->set( "u_lightPos", m_lightPoint.m_position, 1 );
    m_pUniforms->set( "u_lightColor", m_lightPoint.m_color, 1 );
    m_pUniforms->set( "u_lightRange", m_lightPoint.m_range, 1 );
    m_pUniforms->set( "u_ambientPerc", m_lightPoint.m_ambient, 1 );

    m_pUniforms->set( "u_lightColor", vec3(0,0,0), 2 );
    m_pUniforms->set( "u_lightColor", vec3(0,0,0), 3 );

    {
        int textureSlot = 1;
        m_pUniforms->set( "u_textureLightDepth", textureSlot );
        bgfx::setTexture( 1, m_pUniforms->getUniform("u_textureLightDepth"), m_FBOTexture_Depth);
    }

    // Draw all objects.
    for( fw::GameObject* pObject : m_objects )
    {
        pObject->draw( viewID );
    }

    m_pImGuiManager->endFrame();
}

void Game::editor_displayObjectList()
{
    ImGui::Begin( "Object List" );

    for( fw::GameObject* pObject : m_objects )
    {
        ImGui::Text( "%s", pObject->getName().c_str() );
    }

    ImGui::End(); // "Object List"
}

void Game::editor_displayResources()
{
    ImGui::Begin("Resources");

    m_pResources->editor_displayResourceLists();

    ImGui::End(); // "Resources"
}
