/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/TestRenderer/TestRendererPrerequisites.hpp"

namespace ashes::test
{
	class CommandBase
	{
	public:
		CommandBase( CommandBase const & rhs )noexcept = default;
		CommandBase & operator=( CommandBase const & rhs )noexcept = default;

		CommandBase( CommandBase && rhs )noexcept
			: m_device{ rhs.m_device }
		{
			rhs.m_device = nullptr;
		}

		CommandBase & operator=( CommandBase && rhs )noexcept
		{
			m_device = rhs.m_device;
			rhs.m_device = nullptr;
			return *this;
		}

		explicit CommandBase( VkDevice device );
		CommandBase() = delete;
		virtual ~CommandBase()noexcept = default;

		virtual CommandPtr clone()const = 0;
		virtual void apply()const = 0;
		virtual void remove()const
		{
		}

	protected:
		VkDevice getDevice()const noexcept
		{
			return m_device;
		}

	private:
		VkDevice m_device;
	};
}
