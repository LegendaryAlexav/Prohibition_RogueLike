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

#include "Events/GameEvents.h"

#include "Game.h"

#include "LoadResources.h"

#include "Math/Random.h"
#include "Meshes/VertexFormats.h"

#include "Objects/HeightmapMesh.h"
#include "Objects/Player.h"
#include "Objects/PhysicsPlayer.h"

#include "Scene/IslandScene.h"

Game::Game(fw::FWCore& fwCore)
    : GameCore( fwCore )
    , m_eventListener(std::bind(&Game::onEvent, this, std::placeholders::_1))
    , m_pHeightMap(nullptr)
    , m_pDebugGrass(nullptr)
    , m_light(vec3(0, 22, 0), vec3(1, 1, 1), 65, 0.01f)
{
    // General renderer settings.
    int viewID = 0;
    bgfx::setViewClear( viewID, BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH, 0x000030ff, 1.0f, 0 );
    bgfx::setViewRect( viewID, 0, 0, m_FWCore.getWindowClientWidth(), m_FWCore.getWindowClientHeight() );

    // Create Event Manager
    m_pEventManager = new fw::EventManager();

    // Regestier Event Listiners
    m_pEventManager->registerListener<RemoveFromGameEvent>(&m_eventListener);
    m_pEventManager->registerListener<AreaDamageEvent>(&m_eventListener);
    m_pEventManager->registerListener<SpawnGrassEvent>(&m_eventListener);
    m_pEventManager->registerListener<SpawnGrassEvent>(&m_eventListener);

    // Create some manager objects.
    m_pResources = new fw::ResourceManager();
    m_pImGuiManager = new fw::ImGuiManager( &m_FWCore, 1, m_pEventManager);
    
    // Create uniforms.
    createUniforms();

    // Create vertex formats.
    initVertexFormats();

    // Load Resources.
    loadResources( m_pResources );

    // Create Physics World
    m_pPhysicsWorld = new fw::PhysicsWorldJolt(m_pEventManager);
    //m_pPhysicsWorld = new fw::PhysicsWorldBox2D();


    // Create some GameObjects.
    m_pCamera = new fw::Camera( this, vec3(0,5,-13) );

#define GETMESH m_pResources->get<fw::Mesh>
#define GETMATERIAL m_pResources->get<fw::Material>

    //m_pPlayer = new Player( this, "Player", vec3(0,10,0), GETMESH("Sprite"), GETMATERIAL("MegaMan") );
    //m_objects.push_back( m_pPlayer );

    //fw::GameObject* temp;

    fw::GameObject* temp;
    // Temp objects to test physics.
    {

        temp = new fw::GameObject(this, "Ground", vec3(0, 5, 0), GETMESH("Cube"), GETMATERIAL("YellowLit"));
        temp->setScale(vec3(10, 1, 10));
        temp->createPhysicsBody(m_pPhysicsWorld, fw::EPhysicsShapeType::BoxShape, false, vec3(10, 1, 10));
        m_objects.push_back(temp);

        temp = new fw::GameObject(this, "Block", vec3(0.8f, 6, 0), GETMESH("Cube"), GETMATERIAL("BlueLit"));
        temp->createPhysicsBody(m_pPhysicsWorld, fw::EPhysicsShapeType::BoxShape, true, vec3(1, 1, 1));
        m_objects.push_back(temp);

        m_pPlayer = new PhysicsPlayer(this, "PhysicsPlayer", vec3(0, 10, 0), GETMESH("Cylinder"), GETMATERIAL("BlueLit"));
        m_pPlayer->createPhysicsBody(m_pPhysicsWorld, fw::EPhysicsShapeType::SphereShape, true, vec3(1, 1, 1));
        m_objects.push_back(m_pPlayer);

        vec3 focusPoint = temp->getPosition() + vec3(0, -1, 0);

        m_pCamera->setPosition(focusPoint + vec3(0, 0, -15));
        m_pCamera->setLookAtTarget(m_pPlayer);
        m_pPlayer->followTargetFoward(m_pCamera);

        //m_light.m_position = focusPoint + vec3(0, -2, -3);
    }

    /*
    { // Physics Scene/

        temp = new fw::GameObject(this, "Moving Block", vec3(0, 3, 0), GETMESH("Square"), GETMATERIAL("Blue"), vec3(2, 2, 1));
        temp->addPhysicsObject(m_worldId, b2BodyType::b2_dynamicBody, 20.0f);
        m_objects.push_back(temp);

        temp = new fw::GameObject(this, "Static Floor", vec3(0, -5, 0), GETMESH("Square"), GETMATERIAL("Red"), vec3(10,5,1));
        temp->addPhysicsObject(m_worldId);
        m_objects.push_back(temp);
        temp = new fw::GameObject(this, "Static Block", vec3(1, -1, 0), GETMESH("Square"), GETMATERIAL("Green"), vec3(2, 2, 1));
        temp->addPhysicsObject(m_worldId);
        m_objects.push_back(temp);
    }
    */

    { // Island Scene

        //temp = new fw::GameObject(this, "Ground Plane", vec3(0, 0, 0), GETMESH("Water"), GETMATERIAL("ShitBoxThanos"));
        //temp->setScale(vec3(20, 20, 1));
        //temp->setRotation(vec3(-90, 0, 0));
        //m_objects.push_back(temp);
    
        //temp = new fw::GameObject(this, "Floor", vec3(0, 0, 0), GETMESH("Cube"), GETMATERIAL("WhiteLit"));
        //temp->setScale(vec3(10, 0.1f, 10));
        //m_objects.push_back(temp);
    
        //temp = new fw::GameObject(this, "LeftBox", vec3(-3, 0.5f, 0), GETMESH("Cube"), GETMATERIAL("YellowLit"));
        //m_objects.push_back(temp);
        //
        //temp = new fw::GameObject(this, "RightBox", vec3(3, 0.5f, 0), GETMESH("Cube"), GETMATERIAL("YellowLit"));
        //m_objects.push_back(temp);

        temp = new fw::GameObject(this, "IslandPlane", vec3(-10, 0, -10), GETMESH("IslandPlane"), GETMATERIAL("WhiteLit"));
        m_objects.push_back(temp);
        temp->createPhysicsBody(m_pPhysicsWorld, fw::EPhysicsShapeType::HeightFieldShape, false, vec3(1, 1, 1));
        m_pHeightMap = temp;


        //temp = new fw::GameObject(this, "WaterPlane", vec3(-35, 2.5, -35), GETMESH("Plane"), GETMATERIAL("Water"));
        //m_objects.push_back(temp);
    
        // Foliage
        {
            // Height Map Matrix
            mat4 hmWorldMatrix;
            hmWorldMatrix.createSRT(m_pHeightMap->getScale(), m_pHeightMap->getRotation(), m_pHeightMap->getPosition());
            mat4 invHMWorldMatrix = hmWorldMatrix.getInverse();

            // Tree Spawning Loop
            for (int i = 0; i < 10; i++)
            {
                vec3 position;
                do
                {
                    position = vec3(fw::Random::randomFloat(-10.0f, 40.0f), 0, fw::Random::randomFloat(-10.0f, 40.0f));

                    vec3 heightMapSpacePos = invHMWorldMatrix * position;

                    if (HeightmapMesh* pHeightmapMesh = dynamic_cast<HeightmapMesh*>(m_pHeightMap->getMesh()))
                    {
                        float finalHeight = pHeightmapMesh->getHeightForXZPosition(heightMapSpacePos);
                        position.y = finalHeight; //hmWorldMatrix * vec3(0, finalHeight, 0);
                    }

                } while (position.y < 2.0f);

                temp = new fw::GameObject(this, "Tree" + std::to_string(i), position, GETMESH("TreeLowPoly"), GETMATERIAL("WhiteLit"));
                temp->setRotation(vec3(0.0f, fw::Random::randomFloat(0.0f, 360), 0.0f));
                temp->setScale(vec3(fw::Random::randomFloat(0.4, 0.6f)));
                m_objects.push_back(temp);
            }

            // Grass
            vec3 heightMapSpacePos = invHMWorldMatrix * vec3(-3, 0, 0);
            if (HeightmapMesh* pHeightmapMesh = dynamic_cast<HeightmapMesh*>(m_pHeightMap->getMesh()))
            {
                float finalHeight = pHeightmapMesh->getHeightForXZPosition(heightMapSpacePos);
                temp = new fw::GameObject(this, "GrassDebug", vec3(-3, finalHeight, 0), GETMESH("TwoSidedSprite"), GETMATERIAL("Grass"));
                m_objects.push_back(temp);
                m_pDebugGrass = temp;
            }
        }
    }
    /**/
}

Game::~Game()
{
    m_pEventManager->unregisterListener<RemoveFromGameEvent>(&m_eventListener);
    m_pEventManager->unregisterListener<AreaDamageEvent>(&m_eventListener);
    m_pEventManager->unregisterListener<SpawnGrassEvent>(&m_eventListener);

    for( fw::GameObject* pObject : m_objects )
    {
        delete pObject;
    }
      
    delete m_pCamera;

    delete m_pResources;
    delete m_pUniforms;

    delete m_pPhysicsWorld;

    delete m_pImGuiManager;

    delete m_pEventManager;
}

void Game::createUniforms()
{
    m_pUniforms = new fw::Uniforms();
    
    m_pUniforms->createUniform("u_worldMatrix", bgfx::UniformType::Mat4);
    m_pUniforms->createUniform("u_viewMatrix", bgfx::UniformType::Mat4);
    m_pUniforms->createUniform("u_projMatrix", bgfx::UniformType::Mat4);
    m_pUniforms->createUniform("u_matNormalToWorld", bgfx::UniformType::Mat4);

    m_pUniforms->createUniform("u_diffuseColor", bgfx::UniformType::Vec4);

    m_pUniforms->createUniform("u_textureColor", bgfx::UniformType::Sampler);
    m_pUniforms->createUniform("u_specularPower", bgfx::UniformType::Vec4);
    m_pUniforms->createUniform("u_uvScale", bgfx::UniformType::Vec4);
    m_pUniforms->createUniform("u_uvOffset", bgfx::UniformType::Vec4);

    m_pUniforms->createUniform("u_cameraPos", bgfx::UniformType::Vec4);

#define c_maxLights 4

    m_pUniforms->createUniform("u_lightPos", bgfx::UniformType::Vec4, c_maxLights);
    m_pUniforms->createUniform("u_lightColor", bgfx::UniformType::Vec4, c_maxLights);
    m_pUniforms->createUniform("u_lightRange", bgfx::UniformType::Vec4, c_maxLights);
    m_pUniforms->createUniform("u_ambientPerc", bgfx::UniformType::Vec4, c_maxLights);

    m_pUniforms->createUniform("u_time", bgfx::UniformType::Vec4);
}

void Game::startFrame(float deltaTime)
{
    m_pImGuiManager->startFrame( deltaTime );
    m_pEventManager->processEvents();

    float timeStep = 1.0f / 60.0f;
    int subStepCount = 4;
}

void Game::onEvent(fw::Event* pEvent)
{ // Process events.

    // Remove object from GameObject list and delete it.
    if (pEvent->getType() == RemoveFromGameEvent::getStaticEventType())
    {
        RemoveFromGameEvent* pRemoveFromGameEvent = static_cast<RemoveFromGameEvent*>(pEvent);
        fw::GameObject* pObject = pRemoveFromGameEvent->getGameObject();

        auto it = std::find(m_objects.begin(), m_objects.end(), pObject);
        m_objects.erase(it);

        delete pObject;
    }

    // Apply area damage to all objects within the radius of the event.
    if (pEvent->getType() == AreaDamageEvent::getStaticEventType())
    {
        AreaDamageEvent* pAreaDamageEvent = static_cast<AreaDamageEvent*>(pEvent);

        for (fw::GameObject* pObject : m_objects)
        {
            // Skip the player.
            if (pObject == m_pPlayer)
                continue;

            // Check if object is within radius of event.
            float distance = pObject->getPosition().distanceFrom(pAreaDamageEvent->getLocation());
            if (distance <= pAreaDamageEvent->getRadius())
            {
                // Remove object from game.
                RemoveFromGameEvent* pRemoveEvent = new RemoveFromGameEvent(pObject);
                m_pEventManager->addEvent(pRemoveEvent);
            }
        }
    }

    if (pEvent->getType() == SpawnGrassEvent::getStaticEventType())
    {
        SpawnGrassEvent* pSpawnGrassEvent = static_cast<SpawnGrassEvent*>(pEvent);

        if (HeightmapMesh* pHeightmapMesh = dynamic_cast<HeightmapMesh*>(m_pHeightMap->getMesh()))
        {
            mat4 hmWorldMatrix;
            hmWorldMatrix.createSRT(m_pHeightMap->getScale(), m_pHeightMap->getRotation(), m_pHeightMap->getPosition());
            mat4 invHMWorldMatrix = hmWorldMatrix.getInverse();

            for (int i = 0; i<pSpawnGrassEvent->getNumGrass(); i++)
            {

                vec3 position = pSpawnGrassEvent->getLocation();
                vec2 rangeX = pSpawnGrassEvent->getRangeX();
                vec2 rangeZ = pSpawnGrassEvent->getRangeZ();

                float posX = position.x + fw::Random::randomFloat(rangeX.x, rangeX.y);
                float posZ = position.z + fw::Random::randomFloat(rangeZ.x, rangeZ.y);

                position = vec3(posX, 0, posZ);

                if (HeightmapMesh* pHeightmapMesh = dynamic_cast<HeightmapMesh*>(m_pHeightMap->getMesh()))
                {
                    vec3 heightMapSpacePos = invHMWorldMatrix * position;

                    float finalHeight = pHeightmapMesh->getHeightForXZPosition(heightMapSpacePos);
                    position.y = finalHeight;

                    fw::GameObject* grass = new fw::GameObject(this, "SpawnedGrass" + std::to_string(i), position, GETMESH("TwoSidedSprite"), GETMATERIAL("Grass"));
                    grass->setRotation(vec3(0.0f, fw::Random::randomFloat(0.0f, 360), 0.0f));
                    m_objects.push_back(grass);
                }
            }
        }
    }

    if (pEvent->getType() == fw::CollisionEvent::getStaticEventType())
    {
        fw::CollisionEvent* pCollisionEvent = static_cast<fw::CollisionEvent*>(pEvent);

        fw::GameObject* pObject1 = pCollisionEvent->getObject1();
        fw::GameObject* pObject2 = pCollisionEvent->getObject2();

        // Hacky stuff.
        if (pObject1->getName() == "Block" && pObject2->getName() == "PhysicsPlayer")
        {
            // Remove object from game.
            //RemoveFromGameEvent* pRemoveEvent = new RemoveFromGameEvent( pObject1 );
            //m_pEventManager->addEvent( pRemoveEvent );
        }
    }

    if (pEvent->getType() == BreakObjectRayCastEvent::getStaticEventType())
    {
        BreakObjectRayCastEvent* breakObjectRayCastEvent = static_cast<BreakObjectRayCastEvent*>(pEvent);
        fw::GameObject* pHitObject = m_pPhysicsWorld->rayCastNearestObject(breakObjectRayCastEvent->getLocation(), breakObjectRayCastEvent->getDirection());
        if (pHitObject)
        {
            // Call the addItemToInvetory(pHitObject).
                // If it was a Tree, spawn the cylinder with the height they had.
            // Call RemoveFromGameEvent with pHitObject as the object
        }
    }
}

void Game::update(float deltaTime)
{
    if (deltaTime > 1 / 3.0f)
        deltaTime = 1 / 60.0f;
    
    m_pPhysicsWorld->update(deltaTime);

    ImGui::Checkbox("Wireframe", &m_wireframeMode);
    if (m_wireframeMode)
    {
        bgfx::setDebug(BGFX_DEBUG_WIREFRAME);
    }
    else
    {
        bgfx::setDebug(0);
    }

    for( fw::GameObject* pObject : m_objects )
    {
        pObject->update( deltaTime );
    }

    if (m_pDebugGrass)
    {
        float debugGrassYRot = m_pDebugGrass->getRotation().y;
        float speed = 5.0f;
        m_pDebugGrass->setRotation(vec3(0, debugGrassYRot + 1.0f * speed, 0));
    }

    m_pCamera->update( deltaTime );

    editor_displayObjectList();
    editor_displayResources();
}

void Game::draw()
{
    int viewID = 0;

    // Setup time uniforms.
    float time = (float)fw::getSystemTimeSinceGameStart();
    m_pUniforms->set( "u_time", time );
    vec4 camPos = vec4(m_pCamera->getPosition(), 0);
    m_pUniforms->set("u_cameraPos", camPos);

    // Program the view and proj uniforms from the camera.
    m_pCamera->enable( viewID );

    // Draw all objects.
    for( fw::GameObject* pObject : m_objects )
    {
        pObject->draw();
    }

    // Set Lights
    editor_displayLight();
    m_pUniforms->set("u_lightPos",      m_light.m_position,   0);
    m_pUniforms->set("u_lightColor",    m_light.m_color,      0);
    m_pUniforms->set("u_lightRange",    m_light.m_range,      0);
    m_pUniforms->set("u_ambientPrc",    m_light.m_ambient,    0);

    //m_pUniforms->set("u_lightPos",      m_light.position + vec3 (-5,0,0), 1);
    //m_pUniforms->set("u_lightColor",    fw::color4f(0.0f,1.0f,0.0f,1.0f), 1);
    //m_pUniforms->set("u_lightRange",    m_light.lightRange, 1);
    //m_pUniforms->set("u_ambientPrc",    m_light.ambiancePrc,1);

    m_pUniforms->set("u_lightColor", vec3(0, 0, 0), 1);
    m_pUniforms->set("u_lightColor", vec3(0, 0, 0), 2);
    m_pUniforms->set("u_lightColor", vec3(0, 0, 0), 3);

    m_pImGuiManager->endFrame();
}

fw::GameObject* Game::getHeightmap()
{
    return m_pHeightMap;
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

void Game::editor_displayLight()
{
    if (ImGui::Begin("Light Object"))
    {
        ImGui::ColorPicker3("Light Color", &m_light.m_color.x);
        ImGui::DragFloat3("Light Position", &m_light.m_position.x, 0.02f);
        ImGui::DragFloat("Light Range", &m_light.m_range, 0.02f);
        ImGui::DragFloat("Ambiant Percentage", &m_light.m_ambient, 0.02f);
    }
    ImGui::End(); // "Light Object"
}
