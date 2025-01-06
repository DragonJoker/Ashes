/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Sync/XBoxEvent.hpp"

#include "Core/XBoxDevice.hpp"

namespace ashes::xbox
{
	Event::Event( VkDevice device )noexcept
		: m_device{ device }
	{
	}

	VkResult Event::getStatus()const noexcept
	{
		return m_status;
	}

	VkResult Event::set()const noexcept
	{
		m_status = VK_EVENT_SET;
		return VK_SUCCESS;
	}

	VkResult Event::reset()const noexcept
	{
		m_status = VK_EVENT_RESET;
		return VK_SUCCESS;
	}
}
