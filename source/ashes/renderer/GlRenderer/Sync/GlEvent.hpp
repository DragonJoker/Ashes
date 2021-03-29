/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/GlRenderer/GlRendererPrerequisites.hpp"

namespace ashes::gl
{
	class Event
		: public AutoIdIcdObject< Event >
	{
	public:
		Event( VkAllocationCallbacks const * allocInfo
			, VkDevice device );
		~Event();

		VkResult getStatus()const;
		void updateStatus( VkResult status );
		VkResult set();
		VkResult reset();

		inline VkDevice getDevice()const
		{
			return m_device;
		}

	private:
		VkDevice m_device;
		VkResult m_status{ VK_EVENT_RESET };
	};
}
