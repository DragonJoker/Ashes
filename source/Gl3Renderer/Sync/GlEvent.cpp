/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Sync/GlEvent.hpp"

#include "Core/GlDevice.hpp"

namespace gl_renderer
{
	Event::Event( Device const & device )
		: ashes::Event{ device }
	{
	}

	Event::~Event()
	{
	}

	ashes::EventStatus Event::getStatus()const
	{
		return m_status;
	}

	void Event::set()const
	{
		m_status = ashes::EventStatus::eSet;
	}

	void Event::reset()const
	{
		m_status = ashes::EventStatus::eReset;
	}
}
