/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Sync/D3D11Event.hpp"

#include "Core/D3D11Device.hpp"

namespace ashes::d3d11
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
