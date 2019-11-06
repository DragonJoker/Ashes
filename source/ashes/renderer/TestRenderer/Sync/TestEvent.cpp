/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Sync/TestEvent.hpp"

#include "Core/TestDevice.hpp"

namespace ashes::test
{
	Event::Event( VkDevice device )
	{
	}

	VkResult Event::getStatus()const
	{
		return m_status;
	}

	VkResult Event::set()const
	{
		m_status = VK_EVENT_SET;
		return VK_SUCCESS;
	}

	VkResult Event::reset()const
	{
		m_status = VK_EVENT_RESET;
		return VK_SUCCESS;
	}
}
