//
// Copyright (c) 2022-2023 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "CoreHeaders.h"

#include "Camera.h"
#include "GameCore.h"
#include "GameObject.h"
#include "Resources/Mesh.h"
#include "Physics/PhysicsBody.h"
#include "Physics/PhysicsWorld.h"
#include "Utility/Uniforms.h"

namespace fw
{
    GameObject::GameObject(GameCore* pGameCore, std::string name, vec3 pos, Mesh* pMesh, Material* pMaterial)
        : m_pGameCore( pGameCore )
        , m_name( name )
        , m_position( pos )
        , m_pMesh( pMesh )
        , m_pMaterial( pMaterial )
    {
    }

    GameObject::~GameObject()
    {
        delete m_pPhysicsBody;
    }

    void GameObject::update(float deltaTime)
    {
        if( m_pPhysicsBody )
        {
            m_position = m_pPhysicsBody->getPosition();
            m_rotation = m_pPhysicsBody->getRotation();
        }
    }

    void GameObject::draw(int viewID, fw::Material* pOverrideMaterial)
    {
        Uniforms* pUniforms = m_pGameCore->getUniforms();

        mat4 world;
        world.createSRT( m_scale, m_rotation, m_position );
        pUniforms->set( "u_worldMatrix", world );

        fw::Material* pMat = pOverrideMaterial ? pOverrideMaterial : m_pMaterial;

        m_pMesh->draw( viewID, pUniforms, pMat );
    }

    void GameObject::setPosition(vec3 pos)
    {
        m_position = pos;
        if( m_pPhysicsBody )
        {
            m_pPhysicsBody->setPosition( pos );
        }
    }

    void GameObject::setRotation(vec3 rot)
    {
        m_rotation = rot;
        if( m_pPhysicsBody )
        {
            m_pPhysicsBody->setRotation( rot );
        }
    }

    void GameObject::createPhysicsBodyBox(bool dynamic, vec3 boxSize)
    {
        m_pPhysicsBody = m_pGameCore->getPhysicsWorld()->createBoxBody( this, dynamic, m_position, boxSize );
    }

    void GameObject::createPhysicsBodySphere(bool dynamic, float radius)
    {
        m_pPhysicsBody = m_pGameCore->getPhysicsWorld()->createSphereBody( this, dynamic, m_position, radius );
    }

    void GameObject::createPhysicsBodyTerrain(const std::vector<vec3>& positions, int sampleCount)
    {
        m_pPhysicsBody = m_pGameCore->getPhysicsWorld()->createTerrainBody( this, false, m_position, positions, sampleCount );
    }
} // namespace fw
