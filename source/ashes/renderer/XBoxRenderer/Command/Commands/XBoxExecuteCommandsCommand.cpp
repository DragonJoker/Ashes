/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/XBoxExecuteCommandsCommand.hpp"

#include "Command/XBoxCommandBuffer.hpp"

#include "ashesxbox_api.hpp"

namespace ashes::xbox
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
