/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "Sync/Event.hpp"

#include "Core/Device.hpp"

namespace renderer
{
	Event::Event( Device const & device )
		: m_device{ device }
	{
		registerObject( m_device, "Event", this );
	}

	Event::~Event()
	{
		unregisterObject( m_device, this );
	}
}
