/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11ClearDepthStencilCommand.hpp"

#include "Image/D3D11TextureView.hpp"

namespace d3d11_renderer
{
	ClearDepthStencilCommand::ClearDepthStencilCommand( Device const & device
		, ashes::TextureView const & image
		, ashes::DepthStencilClearValue const & value )
		: CommandBase{ device }
		, m_image{ static_cast< TextureView const & >( image ) }
		, m_value{ value }
		, m_flags{ ( isDepthFormat( image.getFormat() )
				? D3D11_CLEAR_DEPTH
				: 0u )
			| ( isStencilFormat( image.getFormat() )
				? D3D11_CLEAR_STENCIL
				: 0u ) }
	{
	}

	void ClearDepthStencilCommand::apply( Context const & context )const
	{
		context.context->ClearDepthStencilView( m_image.getDepthStencilView()
			, m_flags
			, m_value.depth
			, m_value.stencil );
	}

	CommandPtr ClearDepthStencilCommand::clone()const
	{
		return std::make_unique< ClearDepthStencilCommand >( *this );
	}
}
