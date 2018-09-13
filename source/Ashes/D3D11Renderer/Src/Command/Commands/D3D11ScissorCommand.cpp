/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "D3D11ScissorCommand.hpp"

#include "Core/D3D11Device.hpp"

namespace d3d11_renderer
{
	ScissorCommand::ScissorCommand( Device const & device
		, ashes::Scissor const & scissor )
		: CommandBase{ device }
		, m_scissor{ makeScissor( scissor ) }
	{
	}

	void ScissorCommand::apply( Context const & context )const
	{
		context.context->RSSetScissorRects( 1u, &m_scissor );
	}

	CommandPtr ScissorCommand::clone()const
	{
		return std::make_unique< ScissorCommand >( *this );
	}
}
