/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11RendererPrerequisites.hpp"

#include <Sync/Event.hpp>

namespace d3d11_renderer
{
	class Event
		: public ashes::Event
	{
	public:
		Event( Device const & device );
		~Event();
		/**
		*\copydoc	ashes::Event::getStatus
		*/
		ashes::EventStatus getStatus()const override;
		/**
		*\copydoc	ashes::Event::getStatus
		*/
		void set()const override;
		/**
		*\copydoc	ashes::Event::getStatus
		*/
		void reset()const override;

	private:
		mutable ashes::EventStatus m_status{ ashes::EventStatus::eReset };
	};
}
