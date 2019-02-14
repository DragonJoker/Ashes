/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Ashes/Command/Queue.hpp"

#include "Ashes/Core/Device.hpp"

namespace ashes
{
	Queue::Queue( Device const & device )
		: m_device{ device }
	{
		registerObject( m_device, "Queue", this );
	}

	Queue::~Queue()
	{
		unregisterObject( m_device, this );
	}
}
