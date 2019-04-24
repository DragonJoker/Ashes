/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11Renderer/D3D11RendererPrerequisites.hpp"

#include <Ashes/Sync/Event.hpp>

namespace ashes::d3d11
{
	class Event
		: public ashes::Event
	{
	public:
		Event( VkDevice device );
		~Event();
		/**
		*\copydoc	ashes::Event::getStatus
		*/
		ashes::EventStatus getStatus()const;
		/**
		*\copydoc	ashes::Event::getStatus
		*/
		void set()const;
		/**
		*\copydoc	ashes::Event::getStatus
		*/
		void reset()const;

	private:
		mutable ashes::EventStatus m_status{ ashes::EventStatus::eReset };
	};
}
