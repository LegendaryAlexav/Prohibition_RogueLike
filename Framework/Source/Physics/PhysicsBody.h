#pragma once

#include "box2d/include/box2d/box2d.h"
#include "Physics/JoltHelpers.h"
#include "Math/Vector.h"

namespace fw
{
    class PhysicsWorld;
    class PhysicsWorldBox2D;
    class PhysicsWorldJolt;
    class GameObject;
    class Mesh;

    class PhysicsBody
    {
    public:
        PhysicsBody();
        ~PhysicsBody() {}

        virtual vec3 getPosition() = 0;
        virtual vec3 getRotation() = 0;

        virtual void applyForceToCenter(vec3 force) = 0;
        virtual void applyTorque(vec3 force) = 0;

    protected:
    };

    class PhysicsBodyJolt : public PhysicsBody
    {
    public:
        PhysicsBodyJolt(GameObject* pGameObject, Mesh* pMesh, PhysicsWorldJolt* pPhysicsWorld, EPhysicsShapeType shapeType, bool dynamic, vec3 position, vec3 size);
        ~PhysicsBodyJolt();

        virtual vec3 getPosition() override;
        virtual vec3 getRotation() override;

        virtual void applyForceToCenter(vec3 force) override;
        virtual void applyTorque(vec3 force) override;

    protected:
        PhysicsWorldJolt* m_pPhysicsWorld = nullptr;
        JPH::Body* m_pPhysicsBody = nullptr;
    };

    class PhysicsBodyBox2D : public PhysicsBody
    {
    public:
        PhysicsBodyBox2D(GameObject* pGameObject, PhysicsWorldBox2D* pPhysicsWorld, bool dynamic, vec3 position, vec3 size);
        ~PhysicsBodyBox2D();

        virtual vec3 getPosition() override;
        virtual vec3 getRotation() override;

        virtual void applyForceToCenter(vec3 force) override;
        virtual void applyTorque(vec3 force) override;


    protected:
        PhysicsWorldBox2D* m_pPhysicsWorld = nullptr;
        b2BodyId m_physicsBody;
    };
}