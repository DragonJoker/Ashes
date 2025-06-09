/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11ExecuteCommandsCommand.hpp"

#include "Command/D3D11CommandBuffer.hpp"

#include "ashesd3d11_api.hpp"

namespace ashes::D3D11_NAMESPACE
{
	ExecuteCommandsCommand::ExecuteCommandsCommand( VkDevice device
		, VkCommandBuffer commandBuffer )
		: CommandBase{ device }
		, m_commandBuffer{ commandBuffer }
	{
	}

	void ExecuteCommandsCommand::apply( Context const & context )const
	{
		context.context->ExecuteCommandList( get( m_commandBuffer )->getCommandList()
			, FALSE );
	}

	CommandPtr ExecuteCommandsCommand::clone()const
	{
		return std::make_unique< ExecuteCommandsCommand >( *this );
	}
}
