/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Sync/Semaphore.hpp"

#include "Core/Device.hpp"

namespace ashes
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
