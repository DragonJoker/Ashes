/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Sync/D3D11Semaphore.hpp"

namespace ashes::D3D11_NAMESPACE
{
	Semaphore::Semaphore( VkDevice device )noexcept
		: m_device{ device }
	{
	}
}
