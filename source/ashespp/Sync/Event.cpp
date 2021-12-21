/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "ashespp/Sync/Event.hpp"

#include "ashespp/Core/Device.hpp"

namespace ashes
{
	Event::Event( Device const & device )
		: Event{ device, "Event" }
	{
	}

	Event::Event( Device const & device
		, std::string const & debugName )
		: Event{ device
			, debugName
			, { VK_STRUCTURE_TYPE_EVENT_CREATE_INFO
				, nullptr
				, 0u } }
	{
	}

	Event::Event( Device const & device
		, VkEventCreateInfo createInfo )
		: Event{ device, "Event", std::move( createInfo ) }
	{
	}

	Event::Event( Device const & device
		, std::string const & debugName
		, VkEventCreateInfo createInfo )
		: VkObject{ debugName }
		, m_device{ device }
	{
		DEBUG_DUMP( createInfo );
		auto res = m_device.vkCreateEvent( device
			, &createInfo
			, m_device.getAllocationCallbacks()
			, &m_internal );
		checkError( res, "Event creation" );
		registerObject( m_device, debugName, *this );
	}

	Event::~Event()
	{
		unregisterObject( m_device, *this );
		m_device.vkDestroyEvent( m_device
			, m_internal
			, m_device.getAllocationCallbacks() );
	}

	EventStatus Event::getStatus()const
	{
		auto res = m_device.vkGetEventStatus( m_device
			, m_internal );
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuseless-cast"
		return res == VK_EVENT_SET
			? EventStatus::eSet
			: ( res == VK_EVENT_RESET
				? EventStatus::eReset
				: ( (void)checkError( res, "Event get status" ), EventStatus::eError ) );
#pragma GCC diagnostic pop
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
