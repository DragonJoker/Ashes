/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/GlCommandPool.hpp"

#include "Command/GlCommandBuffer.hpp"
#include "Core/GlDevice.hpp"

#include "ashesgl4_api.hpp"

namespace ashes::gl4
{
	CommandPool::CommandPool( VkDevice device
		, VkCommandPoolCreateInfo createInfo )
	{
	}

	VkResult CommandPool::reset( VkCommandPoolResetFlags flags )
	{
		return VK_SUCCESS;
	}

	VkResult CommandPool::free( VkCommandBufferArray sets )
	{
		for ( auto & buffer : sets )
		{
			deallocate( buffer, nullptr );
		}

		return VK_SUCCESS;
	}
}
