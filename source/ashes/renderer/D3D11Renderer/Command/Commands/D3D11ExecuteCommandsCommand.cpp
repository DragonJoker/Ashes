/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11ExecuteCommandsCommand.hpp"

#include "Command/D3D11CommandBuffer.hpp"

#include "ashesd3d11_api.hpp"

namespace ashes::d3d11
{
	ExecuteCommandsCommand::ExecuteCommandsCommand( VkDevice device
		, VkCommandBuffer commandBuffer )
		: CommandBase{ device }
		, m_commandBuffer{ commandBuffer }
	{
	}

	ExecuteCommandsCommand::~ExecuteCommandsCommand()
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
