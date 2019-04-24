/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11ClearColourCommand.hpp"

#include "Image/D3D11ImageView.hpp"

namespace ashes::d3d11
{
	ClearColourCommand::ClearColourCommand( VkDevice device
		, ashes::ImageView const & image
		, VkClearColorValue const & colour )
		: CommandBase{ device }
		, m_image{ static_cast< ImageView const & >( image ) }
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
