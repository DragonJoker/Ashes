#include "Movable.h"

#include <GlLib/Transform.h>

namespace render
{
	Movable::Movable( std::string const & name )noexcept
		: m_name{ name }
	{
	}

	void Movable::update()noexcept
	{
		gl::Mat4 translate{ 1.0f };
		translate = gl::translate( translate, m_position );
		gl::Mat4 scale{ 1.0f };
		scale = gl::scale( scale, m_scale );
		gl::Mat4 rotate = gl::toMat4( m_orientation );
		m_transform = translate * rotate * scale;
	}
}
