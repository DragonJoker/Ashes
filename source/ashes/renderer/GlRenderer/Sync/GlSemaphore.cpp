/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Sync/GlSemaphore.hpp"

#include "Core/GlDevice.hpp"

namespace ashes::gl
{
	Semaphore::Semaphore( VkDevice device )
		: m_device{ device }
	{
	}
}
