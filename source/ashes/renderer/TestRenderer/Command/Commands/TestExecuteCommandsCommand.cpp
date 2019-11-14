/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/TestExecuteCommandsCommand.hpp"

#include "Command/TestCommandBuffer.hpp"

#include "ashestest_api.hpp"

namespace ashes::test
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

	void ExecuteCommandsCommand::apply()const
	{
		for ( auto & command : get( m_commandBuffer )->getCommands() )
		{
			command->apply();
		}
	}

	CommandPtr ExecuteCommandsCommand::clone()const
	{
		return std::make_unique< ExecuteCommandsCommand >( *this );
	}
}
