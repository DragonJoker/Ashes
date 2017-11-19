#include "Viewport.h"

#include <GlLib/Transform.h>

namespace render
{
	Viewport::Viewport( gl::IVec2 const & size )noexcept
		: m_size{ size }
	{
	}

	void Viewport::resize( gl::IVec2 const & size )noexcept
	{
		m_size = size;
	}

	void Viewport::ortho( float left
		, float right
		, float top
		, float bottom
		, float near
		, float far )noexcept
	{
		m_projection = gl::ortho( left, right, bottom, top, near, far );
	}

	void Viewport::fovY( gl::Angle const & fovy )noexcept
	{
		m_changed = m_fovy != fovy;

		if ( m_changed )
		{
			m_fovy = fovy;
			float aspect = float( m_size.x ) / m_size.y;
			m_projection = gl::infinitePerspective( gl::Radians{ m_fovy }, aspect, 0.1f );
		}
	}

	void Viewport::apply()const noexcept
	{
		glViewport( 0, 0, GLsizei( m_size.x ), GLsizei( m_size.y ) );
	}
}
