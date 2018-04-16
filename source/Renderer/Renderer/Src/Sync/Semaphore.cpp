/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "Sync/Semaphore.hpp"

#include "Core/Device.hpp"

namespace renderer
{
	Semaphore::Semaphore( Device const & device )
		: m_device{ device }
	{
		registerObject( m_device, "Semaphore", this );
	}

	Semaphore::~Semaphore()
	{
		unregisterObject( m_device, this );
	}
}
