/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/TestCommandPool.hpp"

#include "Command/TestCommandBuffer.hpp"
#include "Core/TestDevice.hpp"

#include "ashestest_api.hpp"

namespace ashes::test
{
	CommandPool::CommandPool( VkDevice device
		, VkCommandPoolCreateInfo createInfo )
		: m_device{ device }
		, m_createInfo{ std::move( createInfo ) }
	{
	}

	CommandPool::~CommandPool()noexcept
	{
		for ( auto & command : m_commands )
		{
			deallocate( command, nullptr );
		}
	}

	void CommandPool::registerCommands( VkCommandBuffer commands )
	{
		m_commands.push_back( commands );
	}

	VkResult CommandPool::reset()
	{
		for ( auto & command : m_commands )
		{
			deallocate( command, nullptr );
		}

		m_commands.clear();
		return VK_SUCCESS;
	}

	VkResult CommandPool::freeCommands( VkCommandBufferArray commands )
	{
		for ( auto & command : commands )
		{
			auto it = std::find( m_commands.begin()
				, m_commands.end()
				, command );

			if ( it != m_commands.end() )
			{
				deallocate( command, nullptr );
				m_commands.erase( it );
			}
		}

		return VK_SUCCESS;
	}
}
