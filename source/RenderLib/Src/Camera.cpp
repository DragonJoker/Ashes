#include "Camera.h"

#include <Transform.h>

namespace render
{
	Camera::Camera( utils::IVec2 const & size )
		: m_viewport{ size }
		, m_frustum{ m_viewport }
	{
	}

	bool Camera::update()noexcept
	{
		bool result = m_changed || m_viewport.changed();

		if ( result )
		{
			utils::Vec3 right{ 1, 0, 0 };
			utils::Vec3 front{ 0, 0, 1 };
			utils::Vec3 up{ 0, 1, 0 };
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

	void Camera::resize( utils::IVec2 const & size )noexcept
	{
		m_viewport.resize( size );
	}

	bool Camera::visible( utils::Vec3 const & position )const
	{
		return m_frustum.visible( position );
	}
}
