/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Gl4Renderer/GlRendererPrerequisites.hpp"

#include <Ashes/Sync/Event.hpp>

namespace gl_renderer
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
