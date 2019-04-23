/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "AshesPP/Sync/Semaphore.hpp"

#include "AshesPP/Core/Device.hpp"

namespace ashes
{
	Semaphore::Semaphore( Device const & device )
		: m_device{ device }
	{
		VkSemaphoreCreateInfo createInfo
		{
			VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
			nullptr,
			0                                         // flags
		};
		DEBUG_DUMP( createInfo );
		auto res = m_device.vkCreateSemaphore( m_device
			, &createInfo
			, nullptr
			, &m_internal );
		checkError( res, "Semaphore creation" );
		registerObject( m_device, "Semaphore", this );
	}

	Semaphore::~Semaphore()
	{
		unregisterObject( m_device, this );
		m_device.vkDestroySemaphore( m_device
			, m_internal
			, nullptr );
	}
}
