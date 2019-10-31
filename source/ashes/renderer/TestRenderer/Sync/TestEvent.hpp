/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/TestRenderer/TestRendererPrerequisites.hpp"

namespace ashes::test
{
	class Event
	{
	public:
		Event( VkDevice device );
		/**
		*\copydoc	ashes::Event::getStatus
		*/
		VkResult getStatus()const;
		/**
		*\copydoc	ashes::Event::getStatus
		*/
		VkResult set()const;
		/**
		*\copydoc	ashes::Event::getStatus
		*/
		VkResult reset()const;

	private:
		mutable VkResult m_status{ VK_EVENT_RESET };
	};
}
