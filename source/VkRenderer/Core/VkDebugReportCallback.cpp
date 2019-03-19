/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "VkRenderer/Core/VkDebugReportCallback.hpp"

#include "VkRenderer/Core/VkInstance.hpp"

namespace vk_renderer
{
	DebugReportCallback::DebugReportCallback( Instance const & instance
		, ashes::DebugReportCallbackCreateInfo createInfo )
		: ashes::DebugReportCallback{ instance, std::move( createInfo ) }
		, m_vkInstance{ instance }
		, m_vkCreateInfo{ convert( m_createInfo ) }
	{
		VkResult res = m_vkInstance.vkCreateDebugReportCallbackEXT(
			m_vkInstance,
			&m_vkCreateInfo,
			nullptr,
			&m_callback );
		checkError( res, "Debug initialisation" );
	}

	DebugReportCallback::~DebugReportCallback()
	{
		if ( m_callback != VK_NULL_HANDLE )
		{
			m_vkInstance.vkDestroyDebugReportCallbackEXT( m_vkInstance
				, m_callback
				, nullptr );
		}
	}
}
