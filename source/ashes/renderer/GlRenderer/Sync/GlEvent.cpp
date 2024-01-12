/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Sync/GlEvent.hpp"

#include "Core/GlDevice.hpp"

#include "ashesgl_api.hpp"

namespace ashes::gl
{
	Event::Event( [[maybe_unused]] VkAllocationCallbacks const * allocInfo
		, VkDevice device )
		: m_device{ device }
	{
		registerObject( m_device, *this );
	}

	Event::~Event()noexcept
	{
		unregisterObject( m_device, *this );
	}

	VkResult Event::getStatus()const
	{
		return m_status;
	}

	void Event::updateStatus( VkResult status )
	{
		m_status = status;
	}

	VkResult Event::set()
	{
		updateStatus( VK_EVENT_SET );
		return VK_SUCCESS;
	}

	VkResult Event::reset()
	{
		updateStatus( VK_EVENT_RESET );
		return VK_SUCCESS;
	}
}
