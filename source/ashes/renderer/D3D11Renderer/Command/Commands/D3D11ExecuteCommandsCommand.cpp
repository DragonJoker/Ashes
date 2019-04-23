/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11ExecuteCommandsCommand.hpp"

#include "Command/D3D11CommandBuffer.hpp"

namespace ashes::d3d11
{
	ExecuteCommandsCommand::ExecuteCommandsCommand( Device const & device
		, ashes::CommandBuffer const & commandBuffer )
		: CommandBase{ device }
		, m_commandBuffer{ static_cast< CommandBuffer const & >( commandBuffer ) }
	{
	}

	ExecuteCommandsCommand::~ExecuteCommandsCommand()
	{
	}

	void ExecuteCommandsCommand::apply( Context const & context )const
	{
		context.context->ExecuteCommandList( m_commandBuffer.getCommandList()
			, FALSE );
	}

	CommandPtr ExecuteCommandsCommand::clone()const
	{
		return std::make_unique< ExecuteCommandsCommand >( *this );
	}
}
