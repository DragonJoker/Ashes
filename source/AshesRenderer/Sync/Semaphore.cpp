/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "AshesRenderer/Sync/Semaphore.hpp"

#include "AshesRenderer/Core/Device.hpp"

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
