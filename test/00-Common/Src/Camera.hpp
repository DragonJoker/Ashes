#pragma once

#include "Prerequisites.hpp"

namespace vkapp
{
	class Camera
	{
	public:
		void update();

		void reset()
		{
			m_position = utils::Vec3{};
			m_rotation = utils::Quaternion{};
		}

		void translate( utils::Vec3 const & value )
		{
			m_position += value;
		}

		void rotate( utils::Quaternion const & value )
		{
			m_rotation *= value;
		}

		utils::Mat4 const & getView()const
		{
			return m_transform;
		}

		utils::Vec3 const & getPosition()const
		{
			return m_position;
		}

		utils::Quaternion const & getRotation()const
		{
			return m_rotation;
		}

		utils::Quaternion & getRotation()
		{
			return m_rotation;
		}

	private:
		utils::Vec3 m_position{};
		utils::Quaternion m_rotation{};
		utils::Mat4 m_transform{};
	};
}
