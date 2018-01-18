#include "Camera.h"

#include <Utils/Transform.hpp>

namespace render
{
	Camera::Camera( renderer::IVec2 const & size )
		: m_viewport{ size }
		, m_frustum{ m_viewport }
	{
	}

	bool Camera::update()noexcept
	{
		bool result = m_changed || m_viewport.changed();

		if ( result )
		{
			renderer::Vec3 right{ 1, 0, 0 };
			renderer::Vec3 front{ 0, 0, 1 };
			renderer::Vec3 up{ 0, 1, 0 };
			right = utils::rotate( m_orientation, right );
			front = utils::rotate( m_orientation, front );
			up = utils::rotate( m_orientation, up );
			m_transform = utils::lookAt( m_position
				, m_position + front
				, up );
			m_frustum.update( m_position, right, up, front );
			m_changed = false;
		}

		return result;
	}

	void Camera::resize( renderer::IVec2 const & size )noexcept
	{
		m_viewport.resize( size );
	}

	bool Camera::visible( renderer::Vec3 const & position )const
	{
		return m_frustum.visible( position );
	}
}
