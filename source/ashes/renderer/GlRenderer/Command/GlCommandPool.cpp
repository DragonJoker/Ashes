/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/GlCommandPool.hpp"

#include "Command/GlCommandBuffer.hpp"
#include "Core/GlDevice.hpp"

#include "ashesgl_api.hpp"

namespace ashes::gl
{
	CommandPool::CommandPool( VkAllocationCallbacks const * allocInfo
		, VkDevice device
		, VkCommandPoolCreateInfo createInfo )
		: m_device{ device }
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
			deallocateNA( buffer );
		}

		return VK_SUCCESS;
	}
}
