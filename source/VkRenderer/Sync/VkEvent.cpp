/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Sync/VkEvent.hpp"

#include "Core/VkDevice.hpp"

namespace vk_renderer
{
	Event::Event( Device const & device )
		: ashes::Event{ device }
		, m_device{ device }
	{
		VkEventCreateInfo createInfo
		{
			VK_STRUCTURE_TYPE_EVENT_CREATE_INFO,
			nullptr,
			0u,// flags
		};
		DEBUG_DUMP( createInfo );
		auto res = m_device.vkCreateEvent( device
			, &createInfo
			, nullptr
			, &m_event );
		checkError( res, "Event creation" );
	}

	Event::~Event()
	{
		m_device.vkDestroyEvent( m_device
			, m_event
			, nullptr );
	}

	ashes::EventStatus Event::getStatus()const
	{
		auto res = m_device.vkGetEventStatus( m_device
			, m_event );
		return res == VK_EVENT_SET
			? ashes::EventStatus::eSet
			: (res == VK_EVENT_RESET
				? ashes::EventStatus::eReset
				: ( checkError( res, "Event get status" ), ashes::EventStatus::eError ) );
	}

	void Event::set()const
	{
		auto res = m_device.vkSetEvent( m_device
			, m_event );
		checkError( res, "Event set" );
	}

	void Event::reset()const
	{
		auto res = m_device.vkResetEvent( m_device
			, m_event );
		checkError( res, "Event reset" );
	}
}
