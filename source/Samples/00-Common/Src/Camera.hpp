#pragma once

#include "Prerequisites.hpp"

namespace common
{
	class Camera
	{
	public:
		void update();

		inline void reset()
		{
			m_position = renderer::Vec3{};
			m_rotation = renderer::Quaternion{};
		}

		inline void translate( renderer::Vec3 const & value )
		{
			m_position += value;
		}

		inline void rotate( renderer::Quaternion const & value )
		{
			m_rotation *= value;
		}

		inline renderer::Mat4 const & getView()const
		{
			return m_transform;
		}

		inline renderer::Vec3 const & getPosition()const
		{
			return m_position;
		}

		inline renderer::Quaternion const & getRotation()const
		{
			return m_rotation;
		}

		inline renderer::Quaternion & getRotation()
		{
			return m_rotation;
		}

	private:
		renderer::Vec3 m_position;
		renderer::Quaternion m_rotation;
		renderer::Mat4 m_transform;
	};
}
