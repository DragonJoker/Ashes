/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Sync/D3D11Semaphore.hpp"

namespace ashes::d3d11
{
	Semaphore::Semaphore( VkDevice device )noexcept
		: m_device{ device }
	{
	}
}
