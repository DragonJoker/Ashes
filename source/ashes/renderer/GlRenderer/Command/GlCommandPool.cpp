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
		, m_allocInfo{ allocInfo }
	{
		registerObject( m_device, *this );
	}

	CommandPool::~CommandPool()
	{
		free( m_commandBuffers );
		unregisterObject( m_device, *this );
	}

	VkResult CommandPool::createCommandBuffer( VkCommandBuffer & commandBuffer
		, VkCommandBufferAllocateInfo const & info )
	{
		VkResult result = allocate( commandBuffer
			, m_allocInfo
			, getDevice()
			, info.level );
		m_commandBuffers.push_back( commandBuffer );
		return result;
	}

	void CommandPool::destroyCommandBuffer( VkCommandBuffer commandBuffer )
	{
		auto it = std::find( m_commandBuffers.begin()
			, m_commandBuffers.end()
			, commandBuffer );
		assert( it != m_commandBuffers.end() );
		deallocate( commandBuffer, m_allocInfo );
		m_commandBuffers.erase( it );
	}

	VkResult CommandPool::reset( VkCommandPoolResetFlags flags )
	{
		return VK_SUCCESS;
	}

	VkResult CommandPool::free( VkCommandBufferArray commandBuffers )
	{
		for ( auto & buffer : commandBuffers )
		{
			destroyCommandBuffer( buffer );
		}

		return VK_SUCCESS;
	}
}
