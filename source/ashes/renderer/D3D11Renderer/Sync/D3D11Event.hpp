/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/D3D11Renderer/D3D11RendererPrerequisites.hpp"

namespace ashes::d3d11
{
	class Event
	{
	public:
		Event( VkDevice device );
		~Event();
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

		inline VkDevice getDevice()const
		{
			return m_device;
		}

	private:
		mutable VkResult m_status{ VK_EVENT_RESET };

		VkDevice m_device;
	};
}
