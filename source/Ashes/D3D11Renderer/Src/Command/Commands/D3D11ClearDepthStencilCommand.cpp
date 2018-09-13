/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "D3D11ClearDepthStencilCommand.hpp"

#include "Image/D3D11TextureView.hpp"

namespace d3d11_renderer
{
	ClearDepthStencilCommand::ClearDepthStencilCommand( Device const & device
		, ashes::TextureView const & image
		, ashes::DepthStencilClearValue const & value )
		: CommandBase{ device }
		, m_image{ static_cast< TextureView const & >( image ) }
		, m_value{ value }
		, m_format{ convert( m_image.getFormat() ) }
	{
	}

	void ClearDepthStencilCommand::apply( Context const & context )const
	{
	}

	CommandPtr ClearDepthStencilCommand::clone()const
	{
		return std::make_unique< ClearDepthStencilCommand >( *this );
	}
}
