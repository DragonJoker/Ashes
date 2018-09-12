/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "D3D11ExecuteCommandsCommand.hpp"

#include "Command/D3D11CommandBuffer.hpp"

namespace d3d11_renderer
{
	ExecuteCommandsCommand::ExecuteCommandsCommand( Device const & device
		, renderer::CommandBuffer const & commandBuffer )
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
