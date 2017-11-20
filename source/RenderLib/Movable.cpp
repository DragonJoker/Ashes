#include "Movable.h"

#include <Renderer/Transform.hpp>

namespace render
{
	Movable::Movable( std::string const & name )noexcept
		: m_name{ name }
	{
	}

	void Movable::update()noexcept
	{
		renderer::Mat4 translate{ 1.0f };
		translate = renderer::translate( translate, m_position );
		renderer::Mat4 scale{ 1.0f };
		scale = renderer::scale( scale, m_scale );
		renderer::Mat4 rotate = renderer::toMat4( m_orientation );
		m_transform = translate * rotate * scale;
	}
}
