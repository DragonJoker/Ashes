#include "CameraState.h"

namespace render
{
	namespace
	{
		template< typename T >
		utils::RangedValue< T > doUpdateVelocity( utils::RangedValue< T > velocity )
		{
			auto ret = velocity.value() / 2.0f;

			if ( std::abs( float( ret ) ) < 0.2f )
			{
				ret = 0.0_degrees;
			}

			return utils::RangedValue< T >( ret, velocity.range() );
		}
	}

	void CameraState::reset()
	{
		m_zoomVelocity = 0.0_degrees;
		m_velocityX = 0.0_degrees;
		m_velocityY = 0.0_degrees;
		m_fovy = utils::Angle{ 45.0_degrees };
		m_angleX = 0.0_radians;
		m_angleY = 0.0_radians;
		m_quatX = renderer::Quaternion{};
		m_quatY = renderer::Quaternion{};
	}

	void CameraState::update()
	{
#if 1//!defined( NDEBUG )

		m_angleRange = makeRange( utils::Radians( -utils::PiMult2 )
			, utils::Radians( utils::PiMult2 ) );
		m_angleX.updateRange( m_angleRange );
		m_angleY.updateRange( m_angleRange );

#endif

		auto scale = m_fovy.percent() + 0.1f;
		auto angleX = m_angleX;
		auto angleY = m_angleY;
		auto rotY = utils::toRadians( m_velocityY.value() ) * scale / 4.0f;
		auto rotX = utils::toRadians( m_velocityX.value() ) * scale / 4.0f;
		m_angleX += rotY;
		m_angleY += rotX;

		if ( angleX != m_angleX )
		{
			m_quatX = utils::pitch( m_quatX, rotY );
		}

		if ( angleY != m_angleY )
		{
			m_quatY = utils::yaw( m_quatY, rotX );
		}

		m_velocityX = doUpdateVelocity( m_velocityX );
		m_velocityY = doUpdateVelocity( m_velocityY );
		m_zoomVelocity = doUpdateVelocity( m_zoomVelocity );
		auto zoom = utils::toRadians( m_zoomVelocity.value() / 2.0f );

		if ( utils::Radians( m_fovy.value() ) > zoom )
		{
			m_fovy = utils::Radians{ m_fovy.value() } - zoom;
		}
	}

	renderer::Quaternion CameraState::rotation()const noexcept
	{
		return renderer::Quaternion{ renderer::Vec3T< utils::Radians >{ m_angleX.value(), m_angleY.value(), 0.0_radians } };// m_quatY * m_quatX;
	}
}
