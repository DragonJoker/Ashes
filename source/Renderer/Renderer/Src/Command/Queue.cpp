/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "Command/Queue.hpp"

#include "Core/Device.hpp"

namespace renderer
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
