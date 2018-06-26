/*
This file belongs to RendererLib.
See LICENSE file in root folder
*/
#pragma once

#include "VkRendererPrerequisites.hpp"

#include <Sync/Event.hpp>

namespace vk_renderer
{
	class Event
		: public renderer::Event
	{
	public:
		Event( Device const & device );
		~Event();
		/**
		*\copydoc	renderer::Event::getStatus
		*/ 
		renderer::EventStatus getStatus()const override;
		/**
		*\copydoc	renderer::Event::getStatus
		*/ 
		void set()const override;
		/**
		*\copydoc	renderer::Event::getStatus
		*/ 
		void reset()const override;
		/**
		*\~french
		*\brief
		*	Conversion implicite vers VkEvent.
		*\~english
		*\brief
		*	VkEvent implicit cast operator.
		*/
		inline operator VkEvent const &()const
		{
			return m_event;
		}

	private:
		Device const & m_device;
		VkEvent m_event{ VK_NULL_HANDLE };
	};
}
