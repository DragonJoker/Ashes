/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Sync/D3D11Fence.hpp"

namespace ashes::D3D11_NAMESPACE
{
	Fence::Fence( VkDevice device )noexcept
		: m_device{ device }
	{
	}

	VkResult Fence::wait()const noexcept
	{
		return VK_SUCCESS;
	}

	void Fence::reset()const noexcept
	{
	}

	VkResult Fence::getStatus()const noexcept
	{
		return VK_SUCCESS;
	}
}
