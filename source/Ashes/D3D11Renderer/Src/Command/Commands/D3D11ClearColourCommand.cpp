/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "D3D11ClearColourCommand.hpp"

#include "Image/D3D11TextureView.hpp"

namespace d3d11_renderer
{
	ClearColourCommand::ClearColourCommand( Device const & device
		, ashes::TextureView const & image
		, ashes::ClearColorValue const & colour )
		: CommandBase{ device }
		, m_image{ static_cast< TextureView const & >( image ) }
		, m_colour{ colour }
	{
	}

	void ClearColourCommand::apply( Context const & context )const
	{
		context.context->ClearRenderTargetView( m_image.getRenderTargetView()
			, m_colour.float32.data() );
	}

	CommandPtr ClearColourCommand::clone()const
	{
		return std::make_unique< ClearColourCommand >( *this );
	}
}
