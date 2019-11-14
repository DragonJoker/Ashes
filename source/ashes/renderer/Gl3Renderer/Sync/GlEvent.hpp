/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/Gl3Renderer/GlRendererPrerequisites.hpp"

namespace ashes::gl3
{
	class Event
	{
	public:
		Event( VkDevice device );
		~Event();

		VkResult getStatus()const;
		VkResult set()const;
		VkResult reset()const;

	private:
		mutable VkResult m_status{ VK_EVENT_RESET };
	};
}
