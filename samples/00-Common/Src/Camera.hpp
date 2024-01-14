#pragma once

#include "Prerequisites.hpp"

#include <util/Quaternion.hpp>

namespace common
{
	class Camera
	{
	public:
		void update();

		void reset()noexcept
		{
			m_position = {};
			m_rotation = {};
		}

		void translate( utils::Vec3 const & value )noexcept
		{
			m_position += value;
		}

		void rotate( utils::Quaternion const & value )noexcept
		{
			m_rotation *= value;
		}

		utils::Mat4 const & getView()const noexcept
		{
			return m_transform;
		}

		utils::Vec3 const & getPosition()const noexcept
		{
			return m_position;
		}

		utils::Quaternion const & getRotation()const noexcept
		{
			return m_rotation;
		}

		utils::Quaternion & getRotation()noexcept
		{
			return m_rotation;
		}

	private:
		utils::Vec3 m_position{};
		utils::Quaternion m_rotation{};
		utils::Mat4 m_transform{};
	};
}
