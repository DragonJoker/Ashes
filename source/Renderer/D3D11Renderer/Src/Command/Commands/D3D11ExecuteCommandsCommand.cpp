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
		device.getDevice()->CreateDeferredContext( 0u, &m_deferredContext );
		Context context{ m_deferredContext };
		m_commandBuffer.execute( context );
		m_deferredContext->FinishCommandList( FALSE, &m_commandList );
	}

	ExecuteCommandsCommand::~ExecuteCommandsCommand()
	{
		safeRelease( m_commandList );
		safeRelease( m_deferredContext );
	}

	void ExecuteCommandsCommand::apply( Context const & context )const
	{
		context.context->ExecuteCommandList( m_commandList
			, FALSE );
	}

	CommandPtr ExecuteCommandsCommand::clone()const
	{
		return std::make_unique< ExecuteCommandsCommand >( *this );
	}
}
