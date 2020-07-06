/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/D3D11CommandPool.hpp"

#include "Command/D3D11CommandBuffer.hpp"
#include "Core/D3D11Device.hpp"

#include "ashesd3d11_api.hpp"

namespace ashes::d3d11
{
	CommandPool::CommandPool( VkDevice device
		, VkCommandPoolCreateInfo createInfo )
		: m_device{ device }
		, m_createInfo{ std::move( createInfo ) }
	{
	}

	CommandPool::~CommandPool()
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

	VkResult CommandPool::reset( VkCommandPoolResetFlags flags )
	{
		for ( auto & command : m_commands )
		{
			deallocateNA( command );
		}

		m_commands.clear();
		return VK_SUCCESS;
	}

	VkResult CommandPool::free( ArrayView< VkCommandBuffer const > commands )
	{
		for ( auto & command : commands )
		{
			auto it = std::find( m_commands.begin()
				, m_commands.end()
				, command );

			if ( it != m_commands.end() )
			{
				deallocateNA( command );
				m_commands.erase( it );
			}
		}

		return VK_SUCCESS;
	}
}
