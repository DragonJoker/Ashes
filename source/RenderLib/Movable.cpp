#include "Movable.h"

#include <Utils/Transform.hpp>

namespace render
{
	Movable::Movable( std::string const & name )noexcept
		: m_name{ name }
	{
	}

	void Movable::update()noexcept
	{
		utils::Mat4 translate{ 1.0f };
		translate = utils::translate( translate, m_position );
		utils::Mat4 scale{ 1.0f };
		scale = utils::scale( scale, m_scale );
		utils::Mat4 rotate = utils::toMat4( m_orientation );
		m_transform = translate * rotate * scale;
	}
}
