/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Sync/D3D11Fence.hpp"

namespace ashes::d3d11
{
	Fence::Fence( VkDevice device
		, VkFenceCreateFlags flags )
	{
	}

	Fence::~Fence()
	{
	}

	VkResult Fence::wait( uint64_t timeout )const
	{
		return VK_SUCCESS;
	}

	void Fence::reset()const
	{
	}

	VkResult Fence::getStatus()const
	{
		return VK_SUCCESS;
	}
}
