/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11ViewportCommand.hpp"

#include "Core/D3D11Device.hpp"

namespace d3d11_renderer
{
	ViewportCommand::ViewportCommand( Device const & device
		, uint32_t first
		, ashes::ViewportArray const & viewports )
		: CommandBase{ device }
		, m_viewports{ makeViewports( viewports.begin() + first, viewports.end() ) }
	{
	}

	void ViewportCommand::apply( Context const & context )const
	{
		context.context->RSSetViewports( UINT( m_viewports.size() )
			, m_viewports.data() );
	}

	CommandPtr ViewportCommand::clone()const
	{
		return std::make_unique< ViewportCommand >( *this );
	}
}
