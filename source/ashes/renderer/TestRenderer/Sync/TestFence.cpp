/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Sync/TestFence.hpp"

namespace ashes::test
{
	Fence::Fence( VkDevice device
		, VkFenceCreateFlags flags )
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
