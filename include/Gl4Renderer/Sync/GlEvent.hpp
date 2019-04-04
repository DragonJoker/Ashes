/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Gl4Renderer/GlRendererPrerequisites.hpp"

namespace ashes::gl4
{
	class Event
	{
	public:
		Event( VkDevice device );
		~Event();
		/**
		*\copydoc	Event::getStatus
		*/
		VkResult getStatus()const;
		/**
		*\copydoc	Event::getStatus
		*/
		VkResult set()const;
		/**
		*\copydoc	Event::getStatus
		*/
		VkResult reset()const;

	private:
		mutable VkResult m_status{ VK_EVENT_RESET };
	};
}
