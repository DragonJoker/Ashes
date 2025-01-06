/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Sync/XBoxSemaphore.hpp"

namespace ashes::xbox
{
	Semaphore::Semaphore( VkDevice device )noexcept
		: m_device{ device }
	{
	}
}
