#include "CameraState.h"

namespace render
{
	namespace
	{
		template< typename T >
		RangedValue< T > doUpdateVelocity( RangedValue< T > velocity )
		{
			auto ret = velocity.value() / 2.0f;

			if ( std::abs( float( ret ) ) < 0.2f )
			{
				ret = 0.0_degrees;
			}

			return RangedValue< T >( ret, velocity.range() );
		}
	}

	void CameraState::reset()
	{
		m_zoomVelocity = 0.0_degrees;
		m_velocityX = 0.0_degrees;
		m_velocityY = 0.0_degrees;
		m_fovy = gl::Angle{ 45.0_degrees };
		m_angleX = 0.0_radians;
		m_angleY = 0.0_radians;
		m_quatX = gl::Quaternion{};
		m_quatY = gl::Quaternion{};
	}

	void CameraState::update()
	{
#if 1//!defined( NDEBUG )

		m_angleRange = makeRange( gl::Radians( -gl::PiMult2 ), gl::Radians( gl::PiMult2 ) );
		m_angleX.updateRange( m_angleRange );
		m_angleY.updateRange( m_angleRange );

#endif

		auto scale = m_fovy.percent() + 0.1f;
		auto angleX = m_angleX;
		auto angleY = m_angleY;
		auto rotY = gl::toRadians( m_velocityY.value() ) * scale / 4.0f;
		auto rotX = gl::toRadians( m_velocityX.value() ) * scale / 4.0f;
		m_angleX += rotY;
		m_angleY += rotX;

		if ( angleX != m_angleX )
		{
			m_quatX = gl::pitch( m_quatX, rotY );
		}

		if ( angleY != m_angleY )
		{
			m_quatY = gl::yaw( m_quatY, rotX );
		}

		m_velocityX = doUpdateVelocity( m_velocityX );
		m_velocityY = doUpdateVelocity( m_velocityY );
		m_zoomVelocity = doUpdateVelocity( m_zoomVelocity );
		auto zoom = gl::toRadians( m_zoomVelocity.value() / 2.0f );

		if ( gl::Radians( m_fovy.value() ) > zoom )
		{
			m_fovy = gl::Radians{ m_fovy.value() } - zoom;
		}
	}

	gl::Quaternion CameraState::rotation()const noexcept
	{
		return gl::Quaternion{ gl::Vec3T< gl::Radians >{ m_angleX.value(), m_angleY.value(), 0.0_radians } };// m_quatY * m_quatX;
	}
}
