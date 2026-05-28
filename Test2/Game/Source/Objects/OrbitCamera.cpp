#include "Framework.h"
#include "OrbitCamera.h"

OrbitCamera::OrbitCamera(fw::GameCore* pGameCore, fw::GameObject* pTarget)
    : Camera( pGameCore, vec3(0,0,0) )
    , m_pTarget( pTarget )
    , m_distance( 5.0f )
{
    m_rotation.x = -20.0f;
}

OrbitCamera::~OrbitCamera()
{
}

void OrbitCamera::update(float deltaTime)
{
    fw::FWCore* pFramework = m_pGameCore->getFramework();

    vec2 rotationSpeed = vec2( 1.0f, 0.4f );

    // This example uses direct key polling
    // This should be abstracted away via a virtual gamepad or other system.
    if( pFramework->isKeyDown('J') )
        m_rotation.y += 90 * deltaTime * rotationSpeed.x;
    if( pFramework->isKeyDown('L') )
        m_rotation.y -= 90 * deltaTime * rotationSpeed.x;
    if( pFramework->isKeyDown('I') )
        m_rotation.x -= 90 * deltaTime * rotationSpeed.y;
    if( pFramework->isKeyDown('K') )
        m_rotation.x += 90 * deltaTime * rotationSpeed.y;

    if( pFramework->isKeyDown('U') )
        m_distance -= 10 * deltaTime;
    if( pFramework->isKeyDown('M') )
        m_distance += 10 * deltaTime;

    fw::myClamp( m_distance, 0.1f, 100.0f );
    fw::myClamp( m_rotation.x, -89.9f, 89.9f );

    // Calculate eye position.
    // Method 1.
    float sx = sin( m_rotation.x / 180*PI );
    float sy = sin( m_rotation.y / 180*PI );
    float cx = cos( m_rotation.x / 180*PI );
    float cy = cos( m_rotation.y / 180*PI );
    vec3 offset = vec3( cx*sy*m_distance, -sx*m_distance, -1*cx*cy*m_distance );

    m_position = m_pTarget->getPosition() + offset;
    m_lookAtPosition = m_pTarget->getPosition();
}
