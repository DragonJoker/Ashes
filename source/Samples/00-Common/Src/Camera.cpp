#include "Camera.hpp"

#include <Transform.hpp>
#include <Utils/Mat4.hpp>

namespace common
{
	void Camera::update()
	{
		auto rotate = utils::toMat4( m_rotation );
		auto translate = renderer::Mat4( { 1, 0, 0, 0 },
			{ 0, 1, 0, 0 },
			{ 0, 0, 1, 0 },
			{ m_position[0], m_position[1], m_position[2], 1 } );
		m_transform = translate * rotate;
	}
}
