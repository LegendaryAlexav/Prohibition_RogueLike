#pragma once

#include "Math/Vector.h"

namespace fw
{
    class GameObject;
    class PhysicsWorld;

    class PhysicsBody
    {
    public:
        PhysicsBody() {}
        virtual ~PhysicsBody() {}

        virtual vec3 getPosition() = 0;
        virtual vec3 getRotation() = 0;

        virtual void setPosition(vec3 pos) = 0;
        virtual void setRotation(vec3 rot) = 0;

        virtual void applyForceToCenter(vec3 force) = 0;
        virtual void applyImpulseToCenter(vec3 impulse) = 0;
    };
}
