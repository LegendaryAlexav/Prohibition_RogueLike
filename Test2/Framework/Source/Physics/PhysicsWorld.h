#pragma once

#include "box2d/include/box2d/box2d.h"
#include "Math/Vector.h"

namespace fw
{
    class EventManager;
    class GameObject;
    class JoltWorldBundle;
    class PhysicsBody;
    class PhysicsWorld;

    enum class PhysicsEngine
    {
        Box2D,
        Jolt
    };

    PhysicsWorld* createPhysicsWorld(PhysicsEngine engine, EventManager* pEventManager);

    class PhysicsWorld
    {
    public:
        PhysicsWorld() {}
        virtual ~PhysicsWorld() {}

        virtual void update(float deltaTime) = 0;
        virtual PhysicsBody* createBoxBody(GameObject* pGameObject, bool dynamic, vec3 position, vec3 boxSize) = 0;
        virtual PhysicsBody* createSphereBody(GameObject* pGameObject, bool dynamic, vec3 position, float radius) { return nullptr; }
        virtual PhysicsBody* createTerrainBody(GameObject* pGameObject, bool dynamic, vec3 position, const std::vector<vec3>& positions, int sampleCount) { return nullptr; }

        virtual GameObject* raycastForNearestObject(vec3 start, vec3 end) { return nullptr; }
        virtual std::vector<GameObject*> raycastForAllObjects(vec3 start, vec3 end) { return {}; }
    };
}
