#pragma once

#include "Prerequisites.hpp"

#include <util/Quaternion.hpp>

namespace common
{
	class Camera
	{
	public:
		void update();

		inline void reset()
		{
			m_position = utils::Vec3{};
			m_rotation = utils::Quaternion{};
		}

		inline void translate( utils::Vec3 const & value )
		{
			m_position += value;
		}

		inline void rotate( utils::Quaternion const & value )
		{
			m_rotation *= value;
		}

		inline utils::Mat4 const & getView()const
		{
			return m_transform;
		}

		inline utils::Vec3 const & getPosition()const
		{
			return m_position;
		}

		inline utils::Quaternion const & getRotation()const
		{
			return m_rotation;
		}

		inline utils::Quaternion & getRotation()
		{
			return m_rotation;
		}

	private:
		utils::Vec3 m_position;
		utils::Quaternion m_rotation;
		utils::Mat4 m_transform;
	};
}
