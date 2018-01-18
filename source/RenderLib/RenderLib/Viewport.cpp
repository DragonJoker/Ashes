#include "Viewport.h"

#include <Utils/Transform.hpp>

namespace render
{
	Viewport::Viewport( renderer::UIVec2 const & size )noexcept
		: m_size{ size }
		, m_viewport{ size.x, size.y, 0, 0 }
		, m_scissor{ 0, 0, size.x, size.y }
	{
	}

	void Viewport::resize( renderer::UIVec2 const & size )noexcept
	{
		m_size = size;
		m_viewport = renderer::Viewport{ size.x, size.y, 0, 0 };
		m_scissor = renderer::Scissor{ 0, 0, size.x, size.y };
	}

	void Viewport::ortho( float left
		, float right
		, float top
		, float bottom
		, float near
		, float far )noexcept
	{
		m_projection = utils::ortho( left, right, bottom, top, near, far );
	}

	void Viewport::fovY( utils::Angle const & fovy )noexcept
	{
		m_changed = m_fovy != fovy;

		if ( m_changed )
		{
			m_fovy = fovy;
			float aspect = float( m_size.x ) / m_size.y;
			m_projection = utils::infinitePerspective( utils::Radians{ m_fovy }, aspect, 0.1f );
		}
	}
}
