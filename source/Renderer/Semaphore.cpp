/*
This file belongs to VkLib.
See LICENSE file in root folder.
*/
#include "Semaphore.hpp"

#include "Device.hpp"

namespace renderer
{
	Semaphore::Semaphore( Device const & device )
		: m_semaphore{ device.getDevice() }
	{
	}
}
