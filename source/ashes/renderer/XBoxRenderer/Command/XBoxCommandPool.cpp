/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/XBoxCommandPool.hpp"

#include "Command/XBoxCommandBuffer.hpp"
#include "Core/XBoxDevice.hpp"

#include "ashesxbox_api.hpp"

namespace ashes::xbox
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
			deallocateNA( command );
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
			deallocateNA( command );
		}

		m_commands.clear();
		return VK_SUCCESS;
	}

	VkResult CommandPool::freeCommands( ArrayView< VkCommandBuffer const > commands )noexcept
	{
		for ( auto & command : commands )
		{
			if ( auto it = std::find( m_commands.begin()
					, m_commands.end()
					, command );
				it != m_commands.end() )
			{
				deallocateNA( command );
				m_commands.erase( it );
			}
		}

		return VK_SUCCESS;
	}
}
