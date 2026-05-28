#pragma once

#include "DataTypes.h"


class Deer : public fw::GameObject
{
public:
    Deer(Game* pGame, std::string name, vec3 pos, fw::Mesh* pMesh, fw::Material* pMaterial);
    virtual ~Deer();

    virtual void update(float deltaTime) override;

private:

    fw::EventListener m_collisionEventListener;
    void onCollisionEvent(fw::Event* pEvent);

    fw::EventListener m_soundEventListener;
    void onSoundPlayedEvent(fw::Event* pEvent);

protected:
    vec2 m_controls = 0;
    bool m_canJump = true;

    float m_movementTimer = 0;
    float m_jumpTimer = 0;
    float m_scaredTimer = 0;
};