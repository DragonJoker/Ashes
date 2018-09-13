/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Sync/D3D11Event.hpp"

#include "Core/D3D11Device.hpp"

namespace d3d11_renderer
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
