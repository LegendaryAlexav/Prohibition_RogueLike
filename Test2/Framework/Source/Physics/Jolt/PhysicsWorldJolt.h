#pragma once

#include "Jolt/Jolt.h"
#include "Physics/PhysicsWorld.h"

namespace JPH
{
    class PhysicsSystem;
}

namespace fw
{
    class EventManager;
    class JoltWorldBundle;

    class PhysicsWorldJolt : public PhysicsWorld
    {
    public:
        PhysicsWorldJolt(EventManager* pEventManager);
        ~PhysicsWorldJolt();

        virtual void update(float deltaTime) override;
        virtual PhysicsBody* createBoxBody(GameObject* pGameObject, bool dynamic, vec3 position, vec3 boxSize) override;
        virtual PhysicsBody* createSphereBody(GameObject* pGameObject, bool dynamic, vec3 position, float radius) override;
        virtual PhysicsBody* createTerrainBody(GameObject* pGameObject, bool dynamic, vec3 position, const std::vector<vec3>& positions, int sampleCount) override;

        virtual GameObject* raycastForNearestObject(vec3 start, vec3 end) override;
        virtual std::vector<GameObject*> raycastForAllObjects(vec3 start, vec3 end) override;

        JoltWorldBundle* getWorldBundle();
        JPH::PhysicsSystem* getWorld();

    private:
        JoltWorldBundle* m_physicsWorldBundle = nullptr;
    };
}
