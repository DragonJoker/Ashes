/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Ashes/Sync/Event.hpp"

#include "Ashes/Core/Device.hpp"

namespace ashes
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
