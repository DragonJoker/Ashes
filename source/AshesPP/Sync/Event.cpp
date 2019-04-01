/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "AshesPP/Sync/Event.hpp"

#include "AshesPP/Core/Device.hpp"

namespace ashespp
{
	Event::Event( Device const & device )
		: m_device{ device }
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
			, &m_internal );
		checkError( res, "Event creation" );
		registerObject( m_device, "Event", this );
	}

	Event::~Event()
	{
		unregisterObject( m_device, this );
		m_device.vkDestroyEvent( m_device
			, m_internal
			, nullptr );
	}

	EventStatus Event::getStatus()const
	{
		auto res = m_device.vkGetEventStatus( m_device
			, m_internal );
		return res == VK_EVENT_SET
			? EventStatus::eSet
			: ( res == VK_EVENT_RESET
				? EventStatus::eReset
				: ( checkError( res, "Event get status" ), EventStatus::eError ) );
	}

	void Event::set()const
	{
		auto res = m_device.vkSetEvent( m_device
			, m_internal );
		checkError( res, "Event set" );
	}

	void Event::reset()const
	{
		auto res = m_device.vkResetEvent( m_device
			, m_internal );
		checkError( res, "Event reset" );
	}
}
