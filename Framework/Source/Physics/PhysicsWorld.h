#pragma once

#include "box2d/include/box2d/box2d.h"

namespace fw
{
    enum class PhysicsLibraries
    {
        Jolt,
        Box2D,
    };

    class JoltWorldBundle;
    class EventManager;
    class PhysicsBody;
    enum class EPhysicsShapeType;

    class PhysicsWorld
    {
    public:
        PhysicsWorld(PhysicsLibraries physicsLibType);
        ~PhysicsWorld() {}

        virtual PhysicsBody* createPhysicsBody(GameObject* pGameObject, PhysicsWorld* pPhysicsWorld, EPhysicsShapeType shapeType, bool dynamic, vec3 position, vec3 size) = 0;

        virtual void update(float deltaTime) = 0;

        // Get a game object the rayCast hits, by getting the location of a point, and the direction (with the desired length).
        virtual GameObject* rayCastNearestObject(vec3 location, vec3 dir) = 0;

    protected:
        PhysicsLibraries m_physicsLibType;
    };

    class PhysicsWorldJolt : public PhysicsWorld
    {
    public:
        PhysicsWorldJolt(EventManager* pEventManager);
        ~PhysicsWorldJolt();

        virtual PhysicsBody* createPhysicsBody(GameObject* pGameObject, PhysicsWorld* pPhysicsWorld, EPhysicsShapeType shapeType, bool dynamic, vec3 position, vec3 size) override;

        virtual void update(float deltaTime) override;

        JoltWorldBundle* getWorld() { return m_physicsWorld; }

        virtual GameObject* rayCastNearestObject(vec3 location, vec3 dir) override;

    private:
        JoltWorldBundle* m_physicsWorld;
    };

    class PhysicsWorldBox2D : public PhysicsWorld
    {
    public:
        PhysicsWorldBox2D();
        ~PhysicsWorldBox2D();

        virtual PhysicsBody* createPhysicsBody(GameObject* pGameObject, PhysicsWorld* pPhysicsWorld, EPhysicsShapeType shapeType, bool dynamic, vec3 position, vec3 size) override;

        virtual void update(float deltaTime) override;

        b2WorldId getWorldId() { return m_physicsWorld; }

        virtual GameObject* rayCastNearestObject(vec3 location, vec3 dir) override;

    private:
        b2WorldId m_physicsWorld;
    };
}