/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "D3D11ViewportCommand.hpp"

#include "Core/D3D11Device.hpp"

namespace d3d11_renderer
{
	ViewportCommand::ViewportCommand( Device const & device
		, ashes::Viewport const & viewport )
		: CommandBase{ device }
		, m_viewport{ makeViewport( viewport ) }
	{
	}

	void ViewportCommand::apply( Context const & context )const
	{
		context.context->RSSetViewports( 1u, &m_viewport );
	}

	CommandPtr ViewportCommand::clone()const
	{
		return std::make_unique< ViewportCommand >( *this );
	}
}
