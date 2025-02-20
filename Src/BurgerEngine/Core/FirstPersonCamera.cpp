#include "FirstPersonCamera.h"
#include "BurgerEngine/Graphics/CommonGraphics.h"
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------

FirstPersonCamera::FirstPersonCamera( float fFOV, const vec3& f3Pos, const vec2& f2Rotation, const vec4& f4DofParams, const vec2& fSpeed  )
	: AbstractCamera( fFOV, f3Pos, f2Rotation, f4DofParams, fSpeed )
{
};

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void FirstPersonCamera::_UpdatePosition( float fDeltaTime )
{
	vec3& rf3Pos = _GrabPos();
	float fMovingSpeed = m_fPositionSpeed * fDeltaTime;
	
	if( m_iFlags & E_CAMERA_FORWARD)
	{
		rf3Pos += fMovingSpeed * m_f3Direction;
	}
	if( m_iFlags & E_CAMERA_BACKWARD )
	{
		rf3Pos -= fMovingSpeed * m_f3Direction;
	}
	if( m_iFlags & E_CAMERA_LEFT )
	{
		rf3Pos.x += fMovingSpeed * m_f3Right.x;
		rf3Pos.z += fMovingSpeed * m_f3Right.z;
	}
	if( m_iFlags & E_CAMERA_RIGHT )
	{
		rf3Pos.x -= fMovingSpeed * m_f3Right.x;
		rf3Pos.z -= fMovingSpeed * m_f3Right.z;
	}
	if( m_iFlags & E_DOF_NEAR_FORWARD )
	{
		m_fDofOffset += fMovingSpeed;
	}
	if( m_iFlags & E_DOF_NEAR_BACKWARD )
	{
		m_fDofOffset -= fMovingSpeed;
	}

	m_bNeedsUpdate = true;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void FirstPersonCamera::_UpdatePositionAnalog( float fDeltaTime )
{
	vec3& rf3Pos = _GrabPos();
	float fMovingSpeed = m_fPositionSpeed * fDeltaTime;
	rf3Pos += fMovingSpeed * m_f3Direction * m_fAnalogY;

	rf3Pos.x -= fMovingSpeed * m_f3Right.x * m_fAnalogX;
	rf3Pos.z -= fMovingSpeed * m_f3Right.z * m_fAnalogX;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void FirstPersonCamera::_InternalUpdate()
{
	float fCosX = cosf( m_fRX );
	m_f3Direction.x = fCosX * -sinf( m_fRY );
	m_f3Direction.y = sinf( m_fRX );
	m_f3Direction.z = fCosX * -cosf( m_fRY );

	//Cross product
	vec3& rf3Up = _GrabUp();
	
	if( m_fRX > -PI_BY_2 && m_fRX < PI_BY_2 )
	{
		m_f3Right = cross( rf3Up, m_f3Direction);
	}
	else
	{
		m_f3Right = cross( m_f3Direction, rf3Up);
	}

	//normalize
	m_f3Right = normalize( m_f3Right );

	m_mViewMatrix = rotateXY( -m_fRX, m_fRY ) * translate( -m_f3Pos.x, -m_f3Pos.y, -m_f3Pos.z );
}

const float4x4& FirstPersonCamera::GetViewMatrix() const
{
	return m_mViewMatrix;
}