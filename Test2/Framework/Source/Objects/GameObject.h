//
// Copyright (c) 2022-2023 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#pragma once

#include "Math/Vector.h"
#include "Physics/JoltHelpers.h"

namespace fw
{
    class Camera;
    class GameCore;
    class Material;
    class Mesh;
    class PhysicsBody;
    class Uniforms;

    class GameObject
    {
    public:
        GameObject(GameCore* pGameCore, std::string name, vec3 pos, Mesh* pMesh, Material* pMaterial);
        virtual ~GameObject();

        virtual void update(float deltaTime);
        virtual void draw(int viewID, fw::Material* pOverrideMaterial = nullptr);

        // Getters.
        std::string getName() { return m_name; }

        Mesh* getMesh() { return m_pMesh; }
        Material* getMaterial() { return m_pMaterial; }

        vec3 getPosition() { return m_position; }
        vec3 getRotation() { return m_rotation; }
        vec3 getScale() { return m_scale; }

        // Setters.
        void setPosition(vec3 pos);
        void setRotation(vec3 rot);
        void setScale(vec3 scale) { m_scale = scale; }

        void createPhysicsBodyBox(bool dynamic, vec3 boxSize);
        void createPhysicsBodySphere(bool dynamic, float radius);
        void createPhysicsBodyTerrain(const std::vector<vec3>& positions, int sampleCount);

    protected:
        GameCore* m_pGameCore = nullptr;

        std::string m_name;

        Mesh* m_pMesh = nullptr;
        Material* m_pMaterial = nullptr;

        vec3 m_position = vec3(0, 0, 0);
        vec3 m_rotation = vec3(0, 0, 0);
        vec3 m_scale = vec3(1, 1, 1);

        PhysicsBody* m_pPhysicsBody = nullptr;
    };

} // namespace fw
