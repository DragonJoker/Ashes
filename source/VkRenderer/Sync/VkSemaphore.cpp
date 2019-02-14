/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Sync/VkSemaphore.hpp"

#include "Core/VkDevice.hpp"

namespace vk_renderer
{
	Semaphore::Semaphore( Device const & device )
		: ashes::Semaphore{ device }
		, m_device{ device }
	{
		VkSemaphoreCreateInfo createInfo
		{
			VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
			nullptr,
			0                                         // flags
		};
		DEBUG_DUMP( createInfo );
		auto res = m_device.vkCreateSemaphore( m_device, &createInfo, nullptr, &m_semaphore );
		checkError( res, "Semaphore creation" );
	}

	Semaphore::~Semaphore()
	{
		m_device.vkDestroySemaphore( m_device, m_semaphore, nullptr );
	}
}
